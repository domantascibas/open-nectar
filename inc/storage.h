#ifndef STORAGE_H
#define STORAGE_H
//data written to and read from memory address 0x08002000

#define DEVICE_CALIBRATED       0xCA

namespace storage {
    uint8_t init();
    uint8_t saveData(float, float);
    
    char    testRead();
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
