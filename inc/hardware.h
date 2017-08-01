#ifndef HARDWARE_H
#define HARDWARE_H

namespace hardware {
    void init(void);
    uint8_t self_check(void);
}

namespace temperature {
    uint8_t init(void);
    uint8_t update(void);
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
