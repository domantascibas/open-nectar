#include "mbed.h"
#include "measurements.h"
#include "pwm_adjust.h"
#include "comms.h"
#include "utils.h"

//TODO
//- move code from main.cpp into separate files for better readability
//  + pwm_adjust.c
//  + measurements.c
//  + storage.c

// Create a DigitalOut object for the LED
DigitalOut led(D13);
DigitalOut shutdown(PC_8);
InterruptIn calibrationButton(PC_13);

// Create a Serial objects to communicate via USB
Serial pc(USBTX, USBRX);
Serial main_board(PC_10, PC_11);
extern I2C i2c;

Ticker measure;

volatile uint8_t calibrating = false;
volatile uint8_t new_measurement = false;

float voltage = 0.0;
float current = 0.0;
float power = 0.0;

//TODO create namespace for States
//TODO move States into a separate file

enum States {
    IDLE,
    STARTUP,
    RUNNING,
    STOP
};

uint8_t board_config(float, uint32_t, uint32_t, uint16_t, bool calibrate=false);
uint8_t self_check(void);
uint8_t get_command(uint8_t, uint8_t);
void    calibrate_ISR(void);
void    update_measurements_ISR(void);

int main() {
    uint8_t curr_state = 0;
    uint8_t next_state = 0;
    uint8_t response;
    shutdown = 1;
    
    board_config(0.5, 115200, 400000, 5);
    pc.printf("\n\r");
    
    while(1) {
        switch(curr_state) {
            case IDLE: //IDLE state
                pc.printf("IDLE\n\r");
                calibrating = false;
                while(!main_board.readable()) {
                    if(calibrating) {
                        calibrating = false;
                        response = sensors::calibrate();
                        if(response == CALIBRATION_OK) {
                            pc.printf("[ OK ] CALIBRATION DONE\n\r");
                            pc.printf("V = %7.3f I = %7.3f\n\r", measurements::getReferenceVoltage(), measurements::getReferenceCurrent());
                        } else {
                            pc.printf("[ ERROR ] CALIBRATION ERROR: %d\n\r", response);
                        }
                    }
                    __WFI();
                }
                next_state = get_command(main_board.getc(), curr_state);
            break;
            
            case STARTUP: //STARTUP state
                int result;
                pc.printf("STARTING\n\r");
                
                result = self_check();
                //result = STARTUP_OK;
                if(result == STARTUP_OK) {
                    pc.printf("[ STARTUP OK ]\n\n\r");
                    next_state = RUNNING;
                } else {
                    //send error code to screen
                    //TODO
                    //something like print(result), but have an ENUM for result to convert it to readable error codes or text
                    pc.printf("[ STARTUP ERROR ]\n\n\r");
                    next_state = IDLE;
                }
            break;
            
            case RUNNING: //RUNNING state
                //TODO add temperature and overvoltage/overcurrent checks
                pc.printf("RUNNING\n\r");
                measure.attach(&update_measurements_ISR, 0.5);
                shutdown = 0;
                while(!main_board.readable()) {
                    if(new_measurement) {
                        led = !led;
                        new_measurement = false;
                        
                        voltage = measurements::getVoltage();
                        current = measurements::getCurrent();
                        power = current * voltage;
                        
                        pc.printf("%7.3fW %7.3fV %7.3fA\n\r", voltage * current, voltage, current);
                        pwm::set(voltage, current);
                        //pwm::swipe(0.15, 0.85, 0.05);
                    }
                    __WFI();
                }
                next_state = get_command(main_board.getc(), curr_state);
            break;
            
            case STOP:
                measure.detach();
                shutdown = 1;
                pc.printf("STOPPING\n\r");
                pwm::reset();
                next_state = IDLE;
            break;
            
            default: //GOTO IDLE state
                next_state = IDLE;
            break;
        }
        curr_state = next_state;
    }
}

uint8_t self_check(void) {
    uint8_t response;
    double voltage, current;

    //check for calibration data from EEPROM
    response = calibration::check();
    if(response != CALIBRATION_OK){
        pc.printf("[ ERROR ] Device Not Calibrated\n\rPLEASE CALIBRATE\n\r");
        return CALIBRATION_ERROR;
    }
    pc.printf("[ OK ] Device Calibrated\n\r");
    
    
    //initialise ADCs
    response = measurements::init();
    //wait(0.2);  //maybe necessary
    
    //measure Vdc < 350
    voltage = measurements::getVoltage();
    if(voltage > OVERVOLTAGE) {
        pc.printf("[ ERROR ] DC Over-Voltage %7.3fV\n\r", voltage);
        return DC_OVER_VOLTAGE;
    }
    pc.printf("[ DATA ] V = %7.3fV\n\r", voltage);
    //maybe should also check for Vdc == 0 - solar disconnected
    
    //measure Idc == ~0.0
    current = measurements::getCurrent();
    if(current > 0.1) {
        pc.printf("[ ERROR ] DC Current Leaks %7.3fA\n\r", current);
        return DC_CURRENT_LEAKS;
    }
    pc.printf("[ DATA ] I = %7.3fA\n\r", current);
    
    //check Tcap and Tmosfet
    //TODO
    
    //check PWM generator
    //TODO
    
    return STARTUP_OK;
}

uint8_t board_config(float measure_interval, uint32_t serial_baud, uint32_t i2c_freq, uint16_t pwm_freq, bool) {
    pc.baud(serial_baud);
    //comms::config(serial_baud);
    pc.printf("\n\r[ START ]\n\r");
    pc.printf("********************\n\r");
    pc.printf("Serial started\n\rBaud rate: %d\n\n\r", serial_baud);
    wait(0.2);
    
    //check calibration flag in flash
    pwm::init(pwm_freq);
    pc.printf("PWM frequency: %dkHz\n\r", pwm_freq);
    wait(0.2);
    
    sensors::setFrequency(i2c_freq);
    pc.printf("I2C frequency: %dHz\n\r", i2c_freq);
    wait(0.2);
    
    //pc.printf("\n\r");
    calibrationButton.fall(&calibrate_ISR);
    pc.printf("Calibration ISR set\n\rMeasurement ISR set\n\r");
    //pc.printf("Interval: %5.2fs\n\r", measure_interval);
    //pc.printf("ADC read: %d samples, calibration: %d samples\n\n\r", SAMPLES, CALIB_SAMPLES);
    
    //pc.printf("********************\n\r");
    pc.printf("[ SETUP OK ]\n\r");
    wait(0.5);
    return BOARD_CONFIG_OK;
}

uint8_t get_command(uint8_t command, uint8_t state) {
    switch(command) {
        case POWER_BOARD_START:
            if(state == IDLE) {
                return STARTUP;
            } else {
                return state;
            }
        break;
            
        case POWER_BOARD_STOP:
            if(state == RUNNING) {
                return STOP;
            } else {
                return state;
            }
        break;
        
        case SEND_VOLTAGE:
            main_board.printf("%f\n", voltage);
            //printf("[ SENT ] V\n\r");
            return 0;
        break;
        
        case SEND_CURRENT:
            main_board.printf("%f\n", current);
            //printf("[ SENT ] I\n\r");
            return 0;
        break;
        
        case SET_PWM_DUTY:
            float duty;
            main_board.scanf("%f", &duty);
            main_board.getc();
            //printf("[ RECEIVED ] D: %f\n\r", duty);
            //set PWM duty
            return 1;       //back in main set PWM duty
        break;
        
        case SET_SHUTDOWN:
            //shutdown power board
            shutdown = 1;
            return STOP;       //back in main turn off power
        break;
    }
    return 0;
}

void update_measurements_ISR(void) {
    new_measurement = true;
}

void calibrate_ISR(void) {
    calibrating = true;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
