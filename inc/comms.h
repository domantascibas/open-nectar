#ifndef COMMS_H
#define COMMS_H

namespace comms {
    uint8_t send_cmd_power(uint8_t command);
    uint8_t send_cmd_esp(uint8_t command);
    uint8_t receive_message_power(void);
    uint8_t parse_fields(char*, char**, uint32_t, char);
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
