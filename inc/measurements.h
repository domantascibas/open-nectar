#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

namespace measurements {	
    uint8_t init();
    
    uint8_t getVoltage();
    uint8_t getCurrent();
    float getReferenceVoltage();
    float getReferenceCurrent();
}
    
namespace calibration {
    int     check();
    int     testStorage(float, float);
}

namespace sensors {
    int     test(float, float);
    int     setFrequency(int);
    
    int     check(int);
    float   read(int);
    float   sample(int, int);
    int     calibrate();
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
