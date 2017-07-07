#include "mbed.h"
#include "measurements.h"
#include "pwm_adjust.h"
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
extern I2C i2c;

Ticker measure;

volatile int calibrating = false;
volatile int new_measurement = false;

double voltage = 0.0;
double current = 0.0;
double power = 0.0;

namespace command {
    char start = '1';
    char stop = '0';
}

//TODO create namespace for States
//TODO move States into a separate file

enum States {
    IDLE,
    STARTUP,
    RUNNING,
    STOP
};

int     board_config(float, int, int, int, bool calibrate=false);
int     self_check(void);
void    calibrate_ISR(void);
void    update_measurements_ISR(void);

int main() {
    int curr_state = 0;
    int next_state = 0;
    int response;
    shutdown = 1;
    
    board_config(0.5, 115200, 400000, 5);
    pc.printf("\n\r");
    
    while(1) {
        switch(curr_state) {
            case IDLE: //IDLE state
                pc.printf("IDLE\n\r");
                calibrating = false;
                while(!pc.readable()) {
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
                //master_cmd = pc.getc();
                
                //pc.putc(master_cmd);
                if(pc.getc() == command::start) {
                    next_state = STARTUP;
                } else {
                    next_state = IDLE;
                }
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
                while(!pc.readable()) {
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
                if(pc.getc() == command::stop) {
                    measure.detach();
                    shutdown = 1;
                    next_state = STOP;
                } else {
                    next_state = RUNNING;
                }
            break;
            
            case STOP:
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

int self_check(void) {
    int response;
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

int board_config(float measure_interval, int serial_baud, int i2c_freq, int pwm_freq, bool) {
    pc.baud(serial_baud);
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

void update_measurements_ISR(void) {
    new_measurement = true;
}

void calibrate_ISR(void) {
    calibrating = true;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
