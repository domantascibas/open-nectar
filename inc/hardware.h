#ifndef HARDWARE_H
#define HARDWARE_H

namespace hardware {
    uint8_t setup(void);
    uint8_t startup(void);
    void loop(void);
}

namespace temperature {
    uint8_t setup(void);
    uint8_t update(void);
}

namespace comms {
    uint8_t setup(void);
    uint8_t esp_loop(void);
    uint8_t service_loop(void);
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
