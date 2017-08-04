#include "mbed.h"
#include "data.h"
#include "comms.h"

extern RawSerial comms_pc;
extern RawSerial comms_esp;

namespace esp {
    void init(uint32_t baudrate) {
        uint8_t resp;
        comms_esp.baud(baudrate);
    }
    
    uint8_t get_stats(void) {
        comms_pc.printf("sent 0x%X\r\n", INCOMING_DATA);
        //comms_esp.putc(INCOMING_DATA);
        //can send 64 chars max
        comms_esp.printf("#%.2f,%d,%.2f,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f,%.2f$\n", data.pv_power, data.grid_relay_on, data.temp_boiler, data.sun_relay_on, data.pv_voltage, data.pv_current, data.device_temperature, data.mosfet_overheat_on, data.radiator_temp, data.pwm_duty, data.airgap_temp);
        //comms_pc.printf("<%.2f,%d,%.2f,%d,%.2f,%.2f,%.2f,%d,%.2f,%.2f>\n\r", data.pv_power, data.grid_relay_on, data.temp_boiler, data.sun_relay_on, data.pv_voltage, data.pv_current, data.device_temperature, data.mosfet_overheat_on, data.radiator_temp, data.pwm_duty);
        //comms_pc.printf("P:%.2f GRID:%d Twater:%.2f SUN:%d V:%.2f I:%.2f Tdevice:%.2f MOSFET:%d Trad:%.2f Duty:%.2f Tairgap:%.2f\r\n", data.pv_power, data.grid_relay_on, data.temp_boiler, data.sun_relay_on, data.pv_voltage, data.pv_current, data.device_temperature, data.mosfet_overheat_on, data.radiator_temp, data.pwm_duty, data.airgap_temp);
        comms_pc.printf("V:%5.2f I:%5.2f D:%5.2f Tmosfet:%5.2f Overheat:%d Tairgap:%5.2f\r\n", data.pv_voltage, data.pv_current, data.pwm_duty, data.radiator_temp, data.mosfet_overheat_on, data.airgap_temp);
    }
    
    void loop(void) {
        uint8_t command, resp;
        while(!comms_esp.readable()) {}
        if(comms_esp.readable()) {
            command = comms_esp.getc();
            if(command != NULL) {
                comms_pc.printf("\r\nCMD: 0x%X\r\n", command);
                switch(command) {
                    case GET_STATS:
                        comms_pc.printf("ESP GET STATS\r\n");
                        get_stats();
                    break;
                    
                    default:
                        comms_pc.printf("Unrecognized CMD 0x%X\r\n", command);
                    break;
                }
            } else {
                comms_pc.printf("CMD: NULL\r\n");
            }
        }
    }
}
