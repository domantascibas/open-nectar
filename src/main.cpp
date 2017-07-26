#include "mbed.h"
#include "hardware.h"
#include "threads.h"

int main() {
    hardware::setup();
    hardware::startup();
    
    threads::start();
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
