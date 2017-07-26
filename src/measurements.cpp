#include "mbed.h"
#include "measurements.h"
#include "storage.h"
#include "utils.h"

I2C i2c(PB_14, PB_13);
extern Serial pc;

//Global variables
const double    INPUT_VDIV = 4.1/400;
data_collection power_board_data = { 0.00, 0.00, 0.00, 0.00, 0.1, 0.0, 0.0, false, storage::isCalibrated() };

//MEASUREMENTS
namespace measurements {    
    int init() {
        if((sensors::check(ISENSE_ADDR) == ADC_OK) && (sensors::check(USENSE_ADDR) == ADC_OK)) {
            return ADC_SETUP_OK;
        } else {
            return ADC_SETUP_ERROR;
        }
    }

    double getVoltage() {
        power_board_data.moment_voltage = (sensors::sample(USENSE_ADDR, RUNNING_SAMPLES) - power_board_data.reference_voltage) / INPUT_VDIV;
        if(power_board_data.moment_voltage < 0) power_board_data.moment_voltage = 0;
        return power_board_data.moment_voltage;
    }

    double getCurrent() {
        power_board_data.moment_current = (sensors::sample(ISENSE_ADDR, RUNNING_SAMPLES) - power_board_data.reference_current) * 5.000;
        if(power_board_data.moment_current < 0) power_board_data.moment_current = 0;
        return power_board_data.moment_current;
    }

    double getReferenceVoltage() {
        return power_board_data.reference_voltage;
    }

    double getReferenceCurrent() {
        return power_board_data.reference_current;
    }
}

//CALIBRATION
namespace calibration {
    int check() {
        int response = storage::init();
        if(response != CALIBRATION_OK) {
            return response;
        }
        
        if(storage::isCalibrated()) {
            power_board_data.reference_voltage = storage::getVoltage();
            power_board_data.reference_current = storage::getCurrent();
            return CALIBRATION_OK;
        } else {
            //could run calibration when no values are found on flash
            //measurements::calibrate()
            return CALIBRATION_ERROR;
        }
    }

    int testStorage(float voltage, float current) {
        storage::unlock();
        storage::testRead();
        return CALIBRATION_OK;
    }
}

//SENSORS
namespace sensors {
    int test(float voltage, float current) {
        power_board_data.reference_voltage = sample(USENSE_ADDR, CALIBRATION_SAMPLES);
        power_board_data.reference_current = sample(ISENSE_ADDR, CALIBRATION_SAMPLES);
        //pc.printf("%7.3fV %7.3fA\n\r", zero_voltage, zero_current);
        return CALIBRATION_OK;
    }

    int setFrequency(int frequency) {
        i2c.frequency(frequency);		//I2C stops running at 1MHz
        return I2C_OK;
    }

    int check(int addr) {
        char cmd[2];
        int error;
        
        cmd[0] = REG_ADDR_CONFIG;
        cmd[1] = 0x20;
        
        error = i2c.write(addr, cmd, 0);
        if(error == 0) {
            pc.printf("[ OK ] ADC at 0x%X found\n\r", addr);
            i2c.write(addr, cmd, 2, false);
            return ADC_OK;
        } else {
            pc.printf("[ ERROR ] ADC at 0x%X not found!\n\r", addr);
            return ADC_ERROR;
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
            //pc.printf("CURR: %2.3f | MAX: %2.3f | MIN: %2.3f\n\r", current, max, min);
        }
        //pc.printf("--------------------------------------\n\r");
        sum = sum - max - min;
        avg = sum / (samples - 2);
        printf("AVG: %2.3f | MAX: %2.3f | MIN: %2.3f\n\r", avg, max, min);
        //pc.printf("\n\r");
        if(avg < 0) {
            avg = 0;
        }
        return avg;
    }

    int calibrate() {
        int response = storage::init();
        power_board_data.reference_voltage = sample(USENSE_ADDR, CALIBRATION_SAMPLES);
        power_board_data.reference_current = sample(ISENSE_ADDR, CALIBRATION_SAMPLES);
        response = storage::saveData(power_board_data.reference_voltage, power_board_data.reference_current);
        if(response == FLASH_WRITE_OK) {
            return CALIBRATION_OK;
        } else {
            return response;
        }
    }
}
// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
