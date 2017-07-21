#include "mbed.h"
#include "rtos.h"
#include "modes.h"
#include "comms.h"
#include "data.h"

DigitalOut led1(LED1);
DigitalIn zero_cross(PB_5);
//OLED SDA PB_7, SCL PB_6
//Temperature aux PB_8 (boiler)
//Temperature device PB_9 (inside)
//DigitalIn left PA_5
//DigitalIn center PA_6
//DigitalIn right PA_7

Thread thread1;
Thread thread2;
Thread thread3;
Thread thread4;
Mutex serial;

extern Serial comms_power;
extern Serial comms_esp;
extern Data data;
uint8_t cal;

void startup();

void pc_thread();
void check_heatsink_temperature();
void check_capacitor_temperature();

void pc_thread() {
    while(1) {
        pc_monitor::loop();
    }
}

void check_heatsink_temperature() {
    while(1) {
        //measure T heatsink
        //if heatsink temp > safe temp ? shutdown power board : run power board
        Thread::wait(1000);
    }
}

void check_capacitor_temperature() {
    while(1) {
        //measure T capacitor
        //if capacitor_temp ? shutdown power board : run power board
        Thread::wait(1000);
    }
}



void startup() {
    pc_monitor::init(115200);
    power_board::init(19200);
    esp::init(19200);
    
    //check ESP comms baud rate
    
    //startup sequence
    //run safety checks, instruct power board to turn on/off
    
    //init boiler thermometer
    //check if >0;
    
    //init heatsink thermometer
    //check if <40C
    
    //init capacitor thermometer
    //check if <40C
    
    //init transistor overheat
    //check if off
    
    //comms_power.putc(POWER_BOARD_START);
    //wait for response from power board
    
    //check sun relay
    //turn on sun relay, measure current
    
    //check grid relay
    //turn on grid relay, measure boiler temperature
}

int main() {
    uint8_t command_pc, command_esp, command;
    uint8_t mode;
    
    startup();
    thread1.start(pc_thread);
    printf("ready to go\n\r");

/*    
    while(1) {
        while(!comms_esp.readable() && !pc.readable()) {
            __WFI();
        }
        
        if(comms_esp.readable()) {
            command_esp = comms_power.getc();
            if(command_esp != NULL) {
                pc.printf("Data from POWER %c\n\r", command_esp);
            } else {
                pc.printf("NULL\n\r");
            }
        }
*/
        
        /*
        serial.lock();
        
        serial.unlock();
        switch(command) {
            case SET_MODE:
                serial.lock();
                esp::set_mode();
                serial.unlock();
                mode = nectar::run();
            
                if((mode == TURN_ON_GRID) | (mode == TURN_ON_PV)) {
                    data_tests::set_temperature_moment(data_tests::get_temperature_moment()+2);
                } else {
                    data_tests::set_temperature_moment(data_tests::get_temperature_moment()-2);
                }
            break;
            
            case GET_STATS:
                esp::get_stats();
            break;
            
            default:
            break;
        }
        */

/*
        if(pc.readable()) {
            command_pc = pc.getc();
            if(command_pc != NULL) {
                switch(command_pc) {
                    case KEYBOARD_STOP:
                        serial.lock();
                        power_board::stop();
                        relay_sun_on = 0;
                        pc.printf("[ SENT ] STOP\n\r");
                        serial.unlock();
                    break;
                    
                    case KEYBOARD_START:
                        serial.lock();
                        relay_sun_on = 1;
                        power_board::start();
                        pc.printf("[ SENT ] START\n\r");
                        serial.unlock();
                    break;
                    
                    case KEYBOARD_RECEIVE_DATA:
                        serial.lock();
                        nectar_data.sun_voltage = power_board::get_voltage();
                        nectar_data.sun_current = power_board::get_current();
                        pc.printf("[ SENT ] Request data\n\r");
                        pc.printf("[ RECEIVED ] %f, %f\n\r", nectar_data.sun_voltage, nectar_data.sun_current);
                        serial.unlock();
                    break;
                    
                    case PWM_DRIVER_ON:
                        serial.lock();
                        power_board::shutdown(DRIVER_ON);
                        pc.printf("[ SENT ] Driver ON\n\r");
                        serial.unlock();
                    break;
                    
                    case PWM_DRIVER_OFF:
                        serial.lock();
                        power_board::shutdown(DRIVER_OFF);
                        pc.printf("[ SENT ] Driver OFF\n\r");
                        serial.unlock();
                    break;
                    
                    case RECEIVE_VOLTAGE:
                        serial.lock();
                        nectar_data.sun_voltage = power_board::get_voltage();
                        pc.printf("[ RECEIVED ] Voltage %f\n\r", nectar_data.sun_voltage);
                        serial.unlock();
                    break;
                    
                    case RECEIVE_CURRENT:
                        serial.lock();
                        nectar_data.sun_current = power_board::get_current();
                        pc.printf("[ RECEIVED ] Current %f\n\r", nectar_data.sun_current);
                        serial.unlock();
                    break;
                    
                    case RECEIVE_CALIBRATION_DATA:
                        float ref_voltage, ref_current;
                        serial.lock();
                        ref_voltage = power_board::get_ref_voltage();
                        ref_current = power_board::get_ref_current();
                        pc.printf("[ SENT ] Request calibration data\n\r");
                        pc.printf("[ RECEIVED ] %f, %f\n\r", ref_voltage, ref_current);
                        serial.unlock();
                    break;
                        
                    case RECEIVE_MOSFET_OVERHEAT:
                        serial.lock();
                        nectar_data.transistor_overheat_on = power_board::get_overheat();
                        pc.printf("[ RECEIVED ] Overheat: %d\n\r", nectar_data.transistor_overheat_on);
                        serial.unlock();
                    break;
                    
                    default:
                        pc.printf("Unrecognized command %c\n\r", command_pc);
                        command_pc = NULL;
                    break;
                }
            } else {
                pc.printf("NULL\n\r");
            }
        }
    }
    //esp::set_mode();

    //int i;
    //pc.printf("Starting\n\r");
    //comms_power.putc(0x56);
    //float voltage = 120.12345;
    //pc.printf("V%f\n\r", voltage);
    
    //comms_power.printf("%f\n", voltage); //must include a the newline char at the end of transmission, so scanf knows where's the end
    //comms_power.printf("%f", voltage);
    pc.printf("done\n\r");
    //}
*/

}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
