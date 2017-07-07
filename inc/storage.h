#ifndef STORAGE_H
#define STORAGE_H
//data written to and read from memory address 0x08002000

#define DEVICE_CALIBRATED       0xCA

namespace storage {
    char    testRead();
    int     init();
    
    bool    getLock();
    float   getVoltage();
    float   getCurrent();
    bool    isCalibrated();
    
    int     saveData(float, float);
    
    int     unlock();
    float   read(int);
    int     write(float, float);
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
