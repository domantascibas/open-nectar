#ifndef MODE_DEFAULT_H
#define MODE_DEFAULT_H

enum mode_power_source {
    TURN_ON_GRID,
    TURN_ON_PV,
    TURN_OFF_ALL
};

namespace mode_default {
    uint8_t run(double, double, bool);
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
