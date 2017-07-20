#ifndef ESP_COMMS_H
#define ESP_COMMS_H

#define ESP_TX      PA_9
#define ESP_RX      PA_10

#define NUM_FIELDS  12 //number of comma seperated values in the data...TODO does this remain constant?

#define SET_MODE                0x4D    //'M'
#define GET_STATS               0x54    //'T'

namespace esp {
    uint8_t set_mode();         //read message from ESP
    uint8_t get_stats();        //send message to ESP
    
    //cancel_boost();
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************
