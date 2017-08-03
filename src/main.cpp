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
Ticker measure_temperatures;

extern Data data;
extern Serial pc;
extern Serial main_board;
extern I2C i2c;
extern DigitalIn mosfet_overheat;
extern DigitalOut shutdown;

uint8_t get_command(uint8_t, uint8_t, uint8_t);
void blink_code(uint8_t, uint8_t);
void update_measurements_ISR(void);
void update_temperatures_ISR(void);

char buffer[128];
volatile uint8_t new_measurement = false;
volatile uint8_t update_temperature = false;
extern volatile uint8_t calibrating;

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
    
    //__WFI();
    //get_command(main_board.getc(), curr_state, response);
    measure.attach(&update_measurements_ISR, 0.5);
    measure_temperatures.attach(&update_temperatures_ISR, 10.0);
        
    while(1) {
        switch(curr_state) {
            case IDLE: //IDLE state
                led = LED_OFF;
                pc.printf("IDLE\r\n");
                calibrating = false;
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
                    if(update_temperature) {
                        update_temperature = false;
                        temperature::update();
                    }
                    if(new_measurement) {
                        led = !led;
                        new_measurement = false;
                        
                        response = measurements::getVoltage();
                        if(response != NS_OK) {
                            shutdown = DRIVER_OFF;
                        }
                        if(response == NS_OK) {
                            response = measurements::getCurrent();
                            if(response != NS_OK) {
                                shutdown = DRIVER_OFF;
                            }
                        } else {
                            measurements::getCurrent();
                        }
                        
                        pc.printf("%7.2fV %7.2fA\r\n", voltage, current);
                    }
                    __WFI();
                }
                next_state = get_command(main_board.getc(), curr_state, response);
            break;
            
            case STARTUP: //STARTUP state
                pc.printf("STARTING\r\n");
                next_state = RUNNING;
            break;
            
            case RUNNING: //RUNNING state
                //TODO add temperature and overvoltage/overcurrent checks
                pc.printf("RUNNING\r\n");
                while(!main_board.readable()) {
                    if(update_temperature) {
                        update_temperature = false;
                        temperature::update();
                    }
                    if(new_measurement) {
                        led = !led;
                        new_measurement = false;
                        shutdown = DRIVER_ON;
                        
                        response = measurements::getVoltage();
                        if(response != NS_OK) {
                            shutdown = DRIVER_OFF;
                            next_state = STOP;
                        }
                        if(response == NS_OK) {
                            response = measurements::getCurrent();
                            if(response != NS_OK) {
                                shutdown = DRIVER_OFF;
                                next_state = STOP;
                            }
                            data.moment_power = data.moment_current * data.moment_voltage;
                            
                            pc.printf("%7.2fW %7.2fV %7.2fA\r\n", data.moment_power, data.moment_voltage, data.moment_current);
                            pwm::adjust();
                        } else {
                            break;
                        }
                    }
                    __WFI();
                }
                next_state = get_command(main_board.getc(), curr_state, response);
            break;
            
            case STOP:
                shutdown = DRIVER_OFF;
                //led = LED_OFF;
                pc.printf("STOPPING\r\n");
                pwm::reset();
                next_state = IDLE;
            break;
            
            case SERVICE:
                //TODO add temperature and overvoltage/overcurrent checks
                pc.printf("SERVICE\r\n");
                while(!main_board.readable()) {
                    if(new_measurement) {
                        led = !led;
                        new_measurement = false;
                        
                        response = measurements::getVoltage();
                        if(response != NS_OK) {
                            shutdown = DRIVER_OFF;
                        }
                        if(response == NS_OK) {
                            response = measurements::getCurrent();
                            if(response != NS_OK) {
                                shutdown = DRIVER_OFF;
                            }
                            data.moment_power = data.moment_current * data.moment_voltage;
                        }
                        
                        pc.printf("%7.2fW %7.2fV %7.2fA\r\n", data.moment_power, data.moment_voltage, data.moment_current);
                        pwm::set();
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
        if(response != NS_OK) {
            main_board.putc(NACK);
            main_board.putc(response);
            //main_board.putc(INCOMING_DATA);
            //main_board.printf("#%f,%f,%f,%f,%d,%f$\n", data.moment_voltage, data.moment_current, data.pwm_duty, data.radiator_temperature, overheat, data.airgap_temperature); //print to ESP serial
        } else {
            switch(command) {
                case AUTO_MODE:
                case CMD_POWER_BOARD_START:
                case KEYBOARD_START:
                    main_board.putc(ACK);
                    pc.printf("[RECEIVED] START\r\n");
                    if(state == IDLE) {
                        return STARTUP;
                    } else {
                        if(state == SERVICE) {
                            shutdown = DRIVER_OFF;
                            pwm::reset();
                            return STARTUP;
                        } else {
                            return state;
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
                
                /*
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
                */
                    
                case KEYBOARD_GET_DATA:
                    //main_board.putc(INCOMING_DATA);
                    //while(!main_board.writeable()) {}
                    main_board.putc(INCOMING_DATA);
                    main_board.printf("#%.2f,%.2f,%.2f,%.2f,%d,%.2f$\n", data.moment_voltage, data.moment_current, data.pwm_duty, data.radiator_temperature, overheat, data.airgap_temperature); //print to ESP serial
                    pc.printf("[SENT] %f, %f, %f, %f, %d, %f\r\n", data.moment_voltage, data.moment_current, data.pwm_duty, data.radiator_temperature, overheat, data.airgap_temperature);
                    return state;
                break;
                        
                case KEYBOARD_GET_CALIB_DATA:
                    main_board.putc(INCOMING_DATA);
                    main_board.printf("#%f,%f$\n", data.reference_voltage, data.reference_current);
                    pc.printf("[SENT] %f, %f\r\n", data.reference_voltage, data.reference_current);
                    return state;
                break;
                
                case CMD_SET_PWM_DUTY:
                    main_board.putc(WAITING_FOR_DATA);
                    main_board.scanf("#%f$", &data.pwm_duty);
                    main_board.getc();
                    pc.printf("[RECEIVED] Duty\r\n");
                    return state;   //back in main set PWM duty
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
                
                /*
                case CMD_GET_REF_VOLTAGE:
                    main_board.putc(INCOMING_DATA);
                    main_board.printf("#%f$\n", data.reference_voltage); //print to ESP serial
                    pc.printf("[SENT] Calibration: %f\r\n", data.reference_voltage);
                    return state;
                break;
                
                case CMD_GET_REF_CURRENT:
                    main_board.putc(INCOMING_DATA);
                    main_board.printf("#%f$\n", data.reference_current);
                    pc.printf("[SENT] Calibration: %f\r\n", data.reference_voltage);
                    return state;
                break;
                */
                case CMD_GET_PWM_DUTY:
                    main_board.putc(INCOMING_DATA);
                    while(!main_board.writeable()) {}
                    main_board.printf("%f\n", data.pwm_duty);
                    pc.printf("[SENT] Duty: %fD\r\n", data.pwm_duty);
                    return state;
                break;
                        
                /*
                case CMD_GET_MOSFET_OHEAT:
                    main_board.putc(INCOMING_DATA);
                    while(!main_board.writeable()) {}
                    main_board.printf("%d\n", overheat);
                    pc.printf("[SENT] Overheat: %d\r\n", overheat);
                    return state;
                break;

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
        }
    } else {
        main_board.putc(NACK);
        main_board.putc(BAD_CMD);
    }
    return 0;
}

void update_temperatures_ISR(void) {
    update_temperature = true;
}

void update_measurements_ISR(void) {
    new_measurement = true;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
