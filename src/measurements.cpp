#include "mbed.h"
#include "measurements.h"
#include "storage.h"
#include "data.h"

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

I2C i2c(PB_14, PB_13);
extern Data data;

//MEASUREMENTS
namespace measurements {    
    const double    INPUT_VDIV = 4.1/400;
    
    uint8_t init() {
        uint8_t adc1_resp = sensors::check(USENSE_ADDR);
        uint8_t adc2_resp = sensors::check(ISENSE_ADDR);
        
        if((adc1_resp != NS_OK) && (adc2_resp != NS_OK)) {
            return ADC_ERROR;
        } else {
            if(adc1_resp != NS_OK) {
                return ADC_VOLTAGE_ERROR;
            }
            if(adc2_resp != NS_OK) {
                return ADC_CURRENT_ERROR;
            }
            return NS_OK;
        }
    }

    uint8_t getVoltage() {
        float voltage;
        voltage = (sensors::sample(USENSE_ADDR, RUNNING_SAMPLES) - data.reference_voltage) / INPUT_VDIV;
        if(voltage > OVERVOLTAGE) {
            data.moment_voltage = voltage;
            return DC_OVER_VOLTAGE;
        } else {
            if(voltage < 0) {
                data.moment_voltage = 0;
            } else {
                data.moment_voltage = voltage;
            }
            return NS_OK;
        }
    }

    uint8_t getCurrent() {
        float current;
        current = (sensors::sample(ISENSE_ADDR, RUNNING_SAMPLES) - data.reference_current) * 5.000;
        if(current > OVERCURRENT) {
            data.moment_current = current;
            return DC_OVER_CURRENT;
        } else {
            if(current < 0) {
                data.moment_current = 0;
            } else {
                data.moment_current = current;
            }
            return NS_OK;
        }
    }

    //obsolete
    float getReferenceVoltage() {
        return data.reference_voltage;
    }

    //obsolete
    float getReferenceCurrent() {
        return data.reference_current;
    }
}

//CALIBRATION
namespace calibration {
    int check() {
        int response = storage::init();
        if(response == NS_OK) {
            if(data.calibrated) {
                return NS_OK;
            } else {
                //could run calibration when no values are found on flash
                //measurements::calibrate()
                return CALIBRATION_ERROR;
            }
        } else {
            //storage init error
            return response;
        }
    }

    int testStorage(float voltage, float current) {
        storage::init();
        storage::testRead();
        return NS_OK;
    }
}

//SENSORS
namespace sensors {
    int test(float voltage, float current) {
        data.reference_voltage = sample(USENSE_ADDR, CALIBRATION_SAMPLES);
        data.reference_current = sample(ISENSE_ADDR, CALIBRATION_SAMPLES);
        //printf("%7.3fV %7.3fA\n\r", zero_voltage, zero_current);
        return NS_OK;
    }

    int setFrequency(int frequency) {
        i2c.frequency(frequency);		//I2C stops running at 1MHz
        return NS_OK;
    }

    int check(int addr) {
        char cmd[2];
        int error;
        
        cmd[0] = REG_ADDR_CONFIG;
        cmd[1] = 0x20;
        
        error = i2c.write(addr, cmd, 0);
        if(error == 0) {
            //printf("[OK] ADC at 0x%X found\r\n", addr);
            i2c.write(addr, cmd, 2, false);
            return NS_OK;
        } else {
            //printf("[ERROR] ADC at 0x%X not found!\r\n", addr);
            return NS_ERROR;
        }
    }

    float read(int addr) {
        char cmd[2];
        float raw;
        
        cmd[0] = REG_ADDR_RESULT;
        i2c.write(addr, cmd, 1);
        i2c.read(addr, cmd, 2);
        raw = ((cmd[0] & 0x0F) << 8) | cmd[1];
        return (raw * V_REF) / 2048; //raw * VREF * 2 / 4096
    }

    float sample(int addr, int samples) {
        float max = 0;
        float min = 10;
        float current = 0;
        float sum = 0;
        float avg = 0;
        int i = 0;
        
        //pc.printf("--------------------------------------\n\r");
        for(i=0; i<samples; i++) {
            current = read(addr);
            if(current > max) max = current;
            if(current < min) min = current;
            sum += current;
            //printf("CURR: %2.3f | MAX: %2.3f | MIN: %2.3f\n\r", current, max, min);
        }
        //pc.printf("--------------------------------------\n\r");
        sum = sum - max - min;
        avg = sum / (samples - 2);
        //printf("AVG: %2.3f | MAX: %2.3f | MIN: %2.3f\r\n", avg, max, min);
        if(avg < 0) {
            avg = 0;
        }
        return avg;
    }

    int calibrate() {
        int response = storage::init();
        data.reference_voltage = sample(USENSE_ADDR, CALIBRATION_SAMPLES);
        data.reference_current = sample(ISENSE_ADDR, CALIBRATION_SAMPLES);
        response = storage::saveData(data.reference_voltage, data.reference_current);
        if(response != NS_OK) {
            return response;
        }
        return NS_OK;
    }
}
// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
