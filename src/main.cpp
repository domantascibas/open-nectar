#include "mbed.h"
#include "measurements.h"
#include "pwm_adjust.h"
#include "comms.h"
#include "hardware.h"
#include "data.h"

//TODO
//- move code from main.cpp into separate files for better readability
//  + pwm_adjust.c
//  + measurements.c
//  + storage.c

// Create a DigitalOut object for the LED
DigitalOut led(PA_15);
Ticker measure;

extern Data data;
extern Serial pc;
extern Serial main_board;
extern I2C i2c;
extern DigitalIn mosfet_overheat;
extern DigitalOut shutdown;

uint8_t get_command(uint8_t, uint8_t, uint8_t);
void blink_code(uint8_t, uint8_t);
void update_measurements_ISR(void);

char buffer[128];
volatile uint8_t new_measurement = false;
extern volatile uint8_t calibrating;

//TODO move temperature::update() from the ISR to main. introduce a bool flag in main for temp updates

//TODO create namespace for States
//TODO move States into a separate file

enum States {
    IDLE,
    STARTUP,
    RUNNING,
    STOP,
    SERVICE
};

int main() {
    float voltage = 0.0;
    float current = 0.0;
    float power = 0.0;

    uint8_t curr_state = 0;
    uint8_t next_state = 0;
    uint8_t response;
    shutdown = DRIVER_OFF;
    
    hardware::init();
    response = hardware::self_check();
    
    __WFI();
    get_command(main_board.getc(), curr_state, response);
    
    while(1) {
        switch(curr_state) {
            case IDLE: //IDLE state
                led = LED_OFF;
                pc.printf("IDLE\r\n");
                calibrating = false;
                measure.attach(&update_measurements_ISR, 0.5);
                while(!main_board.readable()) {
                    if(calibrating) {
                        calibrating = false;
                        response = sensors::calibrate();
                        if(response == NS_OK) {
                            pc.printf("[OK] CALIBRATION DONE\r\n");
                            pc.printf("V = %7.3f I = %7.3f\r\n", data.reference_voltage, data.reference_current);
                        } else {
                            pc.printf("[ERROR] CALIBRATION ERROR: %d\r\n", response);
                            //blink_code(curr_state, response);
                        }
                    }
                    if(new_measurement) {
                        led = !led;
                        new_measurement = false;
                        
                        measurements::getVoltage();
                        measurements::getCurrent();
                        
                        pc.printf("%7.2fV %7.2fA\r\n", voltage, current);
                    }
                    __WFI();
                }
                next_state = get_command(main_board.getc(), curr_state, response);
            break;
            
            case STARTUP: //STARTUP state
                //int result;
                pc.printf("STARTING\r\n");
                
                //result = hardware::self_check();
                //result = STARTUP_OK;
                //if(result == STARTUP_OK) {
                //    pc.printf("[STARTUP OK]\r\n\n");
                //    shutdown = DRIVER_ON;
                next_state = RUNNING;
                //} else {
                    //send error code to screen
                    //TODO
                    //something like print(result), but have an ENUM for result to convert it to readable error codes or text
                //    pc.printf("[STARTUP ERROR]\r\n\n");
                    //blink_code(curr_state, result);
                //    next_state = IDLE;
                //}
            break;
            
            case RUNNING: //RUNNING state
                //TODO add temperature and overvoltage/overcurrent checks
                pc.printf("RUNNING\r\n");
                while(!main_board.readable()) {
                    if(new_measurement) {
                        led = !led;
                        new_measurement = false;
                        
                        measurements::getVoltage();
                        measurements::getCurrent();
                        data.moment_power = data.moment_current * data.moment_voltage;
                        
                        pc.printf("%7.2fW %7.2fV %7.2fA\r\n", data.moment_power, data.moment_voltage, data.moment_current);
                        pwm::adjust();
                        //pwm::swipe(0.15, 0.85, 0.05);
                    }
                    __WFI();
                }
                next_state = get_command(main_board.getc(), curr_state, response);
            break;
            
            case STOP:
                //measure.detach();
                shutdown = DRIVER_OFF;
                led = LED_OFF;
                pc.printf("STOPPING\r\n");
                pwm::reset();
                next_state = IDLE;
            break;
            
            case SERVICE:
                //TODO add temperature and overvoltage/overcurrent checks
                pc.printf("SERVICE\r\n");
                //measure.attach(&update_measurements_ISR, 0.5);
                while(!main_board.readable()) {
                    if(new_measurement) {
                        led = !led;
                        new_measurement = false;
                        
                        measurements::getVoltage();
                        measurements::getCurrent();
                        data.moment_power = data.moment_current * data.moment_voltage;
                        
                        pc.printf("%7.2fW %7.2fV %7.2fA\r\n", data.moment_power, data.moment_voltage, data.moment_current);
                    }
                    __WFI();
                }
                next_state = get_command(main_board.getc(), curr_state, response);
            break;
            
            default: //GOTO IDLE state
                next_state = IDLE;
            break;
        }
        curr_state = next_state;
    }
}

uint8_t get_command(uint8_t command, uint8_t state, uint8_t response) {
    uint8_t overheat = mosfet_overheat;
    
    if(command != NULL) {
        switch(command) {
            case AUTO_MODE:
            case CMD_POWER_BOARD_START:
            case KEYBOARD_START:
                if(response != NS_OK) {
                    main_board.putc(NACK);
                    main_board.putc(response);
                } else {
                    main_board.putc(ACK);
                    pc.printf("[RECEIVED] START\r\n");
                    if(state == IDLE) {
                        return STARTUP;
                    } else {
                        if(state == SERVICE) {
                            //measure.detach();
                            shutdown = DRIVER_OFF;
                            pwm::set(0.1);
                            return STARTUP;
                        } else {
                            return state;
                        }
                    }
                }
            break;
                
            case CMD_POWER_BOARD_STOP:
            case KEYBOARD_STOP:
                main_board.putc(ACK);
                pc.printf("[RECEIVED] STOP\r\n");
                if(state == RUNNING) {
                    return STOP;
                } else {
                    return state;
                }
            break;
            
            case CMD_GET_VOLTAGE:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%f\n", data.moment_voltage);
                pc.printf("[SENT] Voltage: %f\r\n", data.moment_voltage);
                return state;
            break;
            
            case CMD_GET_CURRENT:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%f\n", data.moment_current);
                pc.printf("[SENT] Current: %f\r\n", data.moment_current);
                return state;
            break;
            
            case KEYBOARD_GET_DATA:
                //main_board.putc(INCOMING_DATA);
                //while(!main_board.writeable()) {}
                if(response != NS_OK) {
                    main_board.putc(NACK);
                    main_board.putc(response);
                } else {
                    main_board.printf("#%f,%f,%f,%f,%d,%f$\n", data.moment_voltage, data.moment_current, data.pwm_duty, data.radiator_temperature, overheat, data.airgap_temperature); //print to ESP serial
                    pc.printf("[SENT] %f, %f, %f, %f, %d, %f\r\n", data.moment_voltage, data.moment_current, data.pwm_duty, data.radiator_temperature, overheat, data.airgap_temperature);
                }
                return state;
            break;
                    
            case KEYBOARD_GET_CALIB_DATA:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%f,%f\n", data.reference_voltage, data.reference_current);
                pc.printf("[SENT] %f, %f\r\n", data.reference_voltage, data.reference_current);
                return state;
            break;
            
            case CMD_SET_PWM_DUTY:
                if(state == IDLE) {
                    main_board.putc(NACK);
                    return IDLE;
                } else {
                    main_board.putc(WAITING_FOR_DATA);
                    while(!main_board.readable()) {}
                    main_board.scanf("%f", &data.pwm_duty);
                    main_board.getc();
                    pc.printf("[RECEIVED] Duty\r\n");
                    pwm::set(data.pwm_duty);
                    return state;       //back in main set PWM duty
                }
            break;
            
            case CMD_PWM_ON:
                main_board.putc(ACK);
                if(state == IDLE) {
                    return IDLE;
                } else {
                    //turn on PWM driver
                    shutdown = DRIVER_ON;
                    pc.printf("[RECEIVED] Driver ON\r\n");
                    return state;       //back in main turn off power
                }
            break;
            
            case CMD_PWM_OFF:
                main_board.putc(ACK);
                if(state == IDLE) {
                    return IDLE;
                } else {
                    //turn off PWM driver
                    shutdown = DRIVER_OFF;
                    pc.printf("[RECEIVED] Driver OFF\r\n");
                    return state;
                    }
            break;
                    
            case CMD_GET_REF_VOLTAGE:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%f\n", data.reference_voltage); //print to ESP serial
                pc.printf("[SENT] Calibration: %f\r\n", data.reference_voltage);
                return state;
            break;
            
            case CMD_GET_REF_CURRENT:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%f\n", data.reference_current);
                pc.printf("[SENT] Calibration: %f\r\n", data.reference_voltage);
                return state;
            break;
            
            case CMD_GET_PWM_DUTY:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%f\n", data.pwm_duty);
                pc.printf("[SENT] Duty: %fD\r\n", data.pwm_duty);
                return state;
            break;
            
            case CMD_GET_MOSFET_OHEAT:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%d\n", overheat);
                pc.printf("[SENT] Overheat: %d\r\n", overheat);
                return state;
            break;
            
            /*
            case CMD_GET_CAP_TEMP:
                main_board.putc(INCOMING_DATA);
                while(!main_board.writeable()) {}
                main_board.printf("%f\n", power_board_data.airgap_temperature);
                return state;
            break;
            */
                    
            case MANUAL_MODE:
                main_board.putc(ACK);
                if(state != IDLE) {
                    //measure.detach();
                }
                pc.printf("[RECEIVED] AUTO_MODE\r\n");
                return SERVICE;
            break;
                    
            default:
                main_board.putc(NACK);
                main_board.putc(BAD_CMD);
                pc.printf("Unrecognized CMD 0x%X\r\n", command);
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

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
