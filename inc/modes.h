#ifndef MODES_H
#define MODES_H

namespace device {
    void    set_mode(uint8_t);
    uint8_t get_mode();
    
    void    run(uint8_t);
}

//test functions. Delete for release
namespace device_tests {
    void    set_temperature_max(double);
    void    set_temperature_current(double);
    void    set_pv_available(bool);
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
