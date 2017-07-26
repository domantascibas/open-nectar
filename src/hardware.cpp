#include "mbed.h"
#include "hardware.h"
#include "DS1820.h"
#include "data.h"
#include "comms.h"

//Serial com pins
#define PC_TX                       PA_2
#define PC_RX                       PA_3
#define POWER_TX                    PB_10
#define POWER_RX                    PB_11
#define ESP_TX                      PA_9
#define ESP_RX                      PA_10

//comms
Serial comms_pc(PC_TX, PC_RX);
Serial comms_power(POWER_TX, POWER_RX);
Serial comms_esp(ESP_TX, ESP_RX);

//user interface
//OLED SDA PB_7, SCL PB_6
//DigitalIn left PA_5
//DigitalIn center PA_6
//DigitalIn right PA_7

//temperature sensors
DS1820  probe_boiler(PB_8);
DS1820  probe_internal(PB_9);

//relays
DigitalOut relay_sun(PB_3);
DigitalOut relay_grid(PB_4);

//other peripherals
DigitalIn zero_cross(PB_5);
DigitalOut led1(LED1);

namespace hardware {
    uint8_t response;
    
    //setup all hardware when started
    uint8_t setup(void) {
        comms::setup();
        comms_pc.printf("[COMMS] Started\r\n");
        
        response = temperature::setup();
        if(response != NS_OK) {
            comms_pc.printf("[TEMPERATURE] Setup failed!\r\n");
        } else {
            comms_pc.printf("[TEMPERATURE] INT and AUX started\r\n");
        }
        
        return NS_OK;
    }
    
    uint8_t startup(void) {
        //startup sequence
        //run safety checks, instruct power board to turn on/off
        
        //init boiler thermometer
        //check if >0;
        data.temp_boiler = probe_boiler.read();
        probe_boiler.startConversion();
        if(data.temp_boiler < 1) {
            printf("[BOILER] ERROR\r\n");
        }
        
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
        return NS_OK;
    }
    
    void loop(void) {
    }
}

namespace temperature {
    uint8_t setup(void) {
        probe_boiler.begin();
        probe_boiler.startConversion();
        probe_internal.begin();
        probe_internal.startConversion();
        wait(0.5);
        
        //TODO add check to see if thermometers are connected        
        return NS_OK;
    }
    
    uint8_t update(void) {
        data.temp_boiler = probe_boiler.read();
        data.device_temperature = probe_internal.read();
        
        probe_boiler.startConversion();
        probe_internal.startConversion();
    }
}

namespace comms {
    uint8_t setup(void) {
        pc_monitor::init(115200);
        power_board::init(19200);
        esp::init(19200);
    }
    
    uint8_t esp_loop(void) {
        esp::loop();
    }
    
    uint8_t service_loop(void) {
        pc_monitor::loop();
    }
}

namespace relays {
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
