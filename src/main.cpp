#include "mbed.h"
#include "measurements.h"
#include "pwm_adjust.h"
#include "comms.h"
#include "utils.h"
#include <string>
#include <sstream>
#include <iostream>

//TODO
//- move code from main.cpp into separate files for better readability
//  + pwm_adjust.c
//  + measurements.c
//  + storage.c

// Create a DigitalOut object for the LED
DigitalOut led(PA_15);
DigitalOut shutdown(PB_0);
InterruptIn calibrationButton(PC_12);
DigitalIn mosfet_overheat(PD_2);

// Create a Serial objects to communicate via USB
Serial pc(PA_2, PA_3);
Serial main_board(PC_10, PC_11);
extern I2C i2c;

Ticker measure;

volatile uint8_t calibrating = false;
volatile uint8_t new_measurement = false;

extern data_collection power_board_data;
char buffer[128];

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
void    blink_code(uint8_t, uint8_t);
void    calibrate_ISR(void);
void    update_measurements_ISR(void);

int main() {
    float voltage = 0.0;
    float current = 0.0;
    float power = 0.0;

    uint8_t curr_state = 0;
    uint8_t next_state = 0;
    uint8_t response;
    shutdown = DRIVER_OFF;
    
    board_config(0.5, 19200, 400000, 5);
    pc.printf("\n\r");
    
    while(1) {
        switch(curr_state) {
            case IDLE: //IDLE state
                led = LED_OFF;
                pc.printf("IDLE\n\r");
                calibrating = false;
                while(!main_board.readable()) {
                    if(calibrating) {
                        calibrating = false;
                        response = sensors::calibrate();
                        if(response == CALIBRATION_OK) {
                            pc.printf("[ OK ] CALIBRATION DONE\n\r");
                            pc.printf("V = %7.3f I = %7.3f\n\r", power_board_data.reference_voltage, power_board_data.reference_current);
                        } else {
                            pc.printf("[ ERROR ] CALIBRATION ERROR: %d\n\r", response);
                            blink_code(curr_state, response);
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
                    measure.attach(&update_measurements_ISR, 0.5);
                    shutdown = DRIVER_ON;
                    next_state = RUNNING;
                } else {
                    //send error code to screen
                    //TODO
                    //something like print(result), but have an ENUM for result to convert it to readable error codes or text
                    pc.printf("[ STARTUP ERROR ]\n\n\r");
                    blink_code(curr_state, result);
                    next_state = IDLE;
                }
            break;
            
            case RUNNING: //RUNNING state
                //TODO add temperature and overvoltage/overcurrent checks
                pc.printf("RUNNING\n\r");
                while(!main_board.readable()) {
                    if(new_measurement) {
                        led = !led;
                        new_measurement = false;
                        
                        voltage = measurements::getVoltage();
                        current = measurements::getCurrent();
                        power = current * voltage;
                        
                        pc.printf("%7.3fW %7.3fV %7.3fA\n\r", power, voltage, current);
                        power_board_data.pwm_duty = pwm::set(voltage, current);
                        //pwm::swipe(0.15, 0.85, 0.05);
                    }
                    __WFI();
                }
                next_state = get_command(main_board.getc(), curr_state);
            break;
            
            case STOP:
                measure.detach();
                shutdown = DRIVER_OFF;
                led = LED_OFF;
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
    float voltage, current;

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
    main_board.baud(serial_baud);
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
    uint8_t overheat = mosfet_overheat;
    uint8_t length;
    ostringstream message;
    string s;
    const char* p;
    
    if(command != NULL) {
        switch(command) {
            case CMD_POWER_BOARD_START:
            case KEYBOARD_START:
                main_board.putc(ACK);
                pc.printf("[ RECEIVED ] START\n\r");
                if(state == IDLE) {
                    return STARTUP;
                } else {
                    return state;
                }
            break;
                
            case CMD_POWER_BOARD_STOP:
            case KEYBOARD_STOP:
                main_board.putc(ACK);
                pc.printf("[ RECEIVED ] STOP\n\r");
                if(state == RUNNING) {
                    return STOP;
                } else {
                    return state;
                }
            break;
            
            case CMD_GET_VOLTAGE:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%f\n", power_board_data.moment_voltage);
                pc.printf("[ SENT ] Voltage: %f\n\r", power_board_data.moment_voltage);
                return state;
            break;
            
            case CMD_GET_CURRENT:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%f\n", power_board_data.moment_current);
                pc.printf("[ SENT ] Current: %f\n\r", power_board_data.moment_current);
                return state;
            break;
            
            case KEYBOARD_GET_DATA:
                main_board.printf("%f,%f,%f,%f,%d\n", power_board_data.moment_voltage, power_board_data.moment_current, power_board_data.pwm_duty, power_board_data.capacitor_temperature, overheat); //print to ESP serial
                pc.printf("[ SENT ] Data: %f, %f, %f, %f, %d\n", power_board_data.moment_voltage, power_board_data.moment_current, power_board_data.pwm_duty, power_board_data.capacitor_temperature, overheat);
                return state;
            break;
            
            case CMD_SET_PWM_DUTY:
                if(state == IDLE) {
                    return IDLE;
                } else {
                    float duty;
                    main_board.scanf("%f", &duty);
                    main_board.getc();
                    pc.printf("[ RECEIVED ] Duty\n\r");
                    //set PWM duty
                    return RUNNING;       //back in main set PWM duty
                }
            break;
            
            case CMD_PWM_ON:
                main_board.putc(ACK);
                if(state == IDLE) {
                    return IDLE;
                } else {
                    //turn on PWM driver
                    shutdown = DRIVER_ON;
                    pc.printf("[ RECEIVED ] Driver ON\n\r");
                    return RUNNING;       //back in main turn off power
                }
            break;
            
            case CMD_PWM_OFF:
                main_board.putc(ACK);
                if(state == IDLE) {
                    return IDLE;
                } else {
                    //turn off PWM driver
                    shutdown = DRIVER_OFF;
                    pc.printf("[ RECEIVED ] Driver OFF\n\r");
                    return RUNNING;
                    }
            break;
                    
            case CMD_GET_REF_VOLTAGE:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%f\n", power_board_data.reference_voltage); //print to ESP serial
                pc.printf("[ SENT ] Calibration: %f\n\r", power_board_data.reference_voltage);
                return state;
            break;
            
            case CMD_GET_REF_CURRENT:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%f\n", power_board_data.reference_current);
                pc.printf("[ SENT ] Calibration: %f\n\r", power_board_data.reference_voltage);
                return state;
            break;
            
            case CMD_GET_PWM_DUTY:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%f\n", pwm::get_duty());
                pc.printf("[ SENT ] Duty: %fD\n\r", pwm::get_duty());
                return state;
            break;
            
            case CMD_GET_MOSFET_OHEAT:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%d\n", overheat);
                pc.printf("[ SENT ] Overheat: %d\n\r", overheat);
                return state;
            break;
                    
            case CMD_GET_CAP_TEMP:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%f\n", power_board_data.capacitor_temperature);
                return state;
            break;
                    
            default:
                main_board.putc(NACK);
                main_board.putc(BAD_CMD);
                pc.printf("Unrecognized CMD 0x%X\n\r", command);
                return state;
            break;
        }
    } else {
        main_board.putc(NACK);
        main_board.putc(BAD_CMD);
    }
    return 0;
}

void blink_code(uint8_t state, uint8_t code) {
    uint8_t i;
    led = LED_OFF;
    for(i=0; i<3; i++) {
        led = LED_ON;
        wait(0.2);
        led = LED_OFF;
        wait(0.2);
    }
    wait(0.2);
    for(i=0; i<3; i++) {
        led = LED_ON;
        wait(0.1);
        led = LED_OFF;
        wait(0.1);
    }
    wait(0.3);
    for(i=0; i<3; i++) {
        led = LED_ON;
        wait(0.2);
        led = LED_OFF;
        wait(0.2);
    }
    wait(1.0);
    for(i=0; i<=state; i++) {
        led = LED_ON;
        wait(0.3);
        led = LED_OFF;
        wait(0.3);
    }
    wait(0.5);
    for(i=0; i<=code; i++) {
        led = LED_ON;
        wait(0.3);
        led = LED_OFF;
        wait(0.3);
    }
    wait(0.5);
}

void update_measurements_ISR(void) {
    new_measurement = true;
}

void calibrate_ISR(void) {
    calibrating = true;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
