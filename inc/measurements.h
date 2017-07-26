#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#define USENSE_ADDR             0x55 << 1
#define ISENSE_ADDR             0x5A << 1

#define RUNNING_SAMPLES         128
#define CALIBRATION_SAMPLES     1024

#define REG_ADDR_RESULT         0x00
#define REG_ADDR_ALERT          0x01
#define REG_ADDR_CONFIG         0x02
#define REG_ADDR_LIMITL         0x03
#define REG_ADDR_LIMITH         0x04
#define REG_ADDR_HYST           0x05
#define REG_ADDR_CONVL          0x06
#define REG_ADDR_CONVH          0x07

struct data_collection {
    float moment_voltage;
    float moment_current;
    
    float reference_voltage;
    float reference_current;
    
    float pwm_duty;
    float  airgap_temperature;
    float radiator_temperature;
    bool mosfet_overheat_on;
    
    bool calibrated;
};

//Interface
namespace measurements {	
    int     init();
    
    double  getVoltage();
    double  getCurrent();
    double  getReferenceVoltage();
    double  getReferenceCurrent();
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
