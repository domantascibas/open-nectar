#include "mbed.h"
#include "DS1820.h"
#include "hardware.h"
#include "measurements.h"
#include "pwm_adjust.h"
#include "data.h"

// Create a Serial objects to communicate via USB
Serial pc(PA_2, PA_3);
Serial main_board(PC_10, PC_11);

DigitalOut shutdown(PB_0);
DigitalIn mosfet_overheat(PD_2);
InterruptIn calibrationButton(PC_12);

DS1820  probe_1(PC_6);
DS1820  probe_2(PC_7);
Ticker measure_temperatures;

extern Data data;
void update_temperatures_ISR(void);
void calibrate_ISR(void);

volatile uint8_t calibrating = false;

namespace hardware {
    void init(void) {
        float measure_interval = 0.5;
        uint32_t comms_pc_baud = 115200;
        uint32_t comms_main_baud = 19200;
        uint32_t i2c_freq = 400000;
        float pwm_freq = 3;
        
        pc.baud(comms_pc_baud);
        main_board.baud(comms_main_baud);
        pc.printf("\n\r[START]\r\n");
        pc.printf("********************\r\n");
        pc.printf("Serial started\n\rBaud rate: %d\r\n\n", comms_main_baud);
        
        pwm::init(pwm_freq);
        pc.printf("PWM frequency: %dkHz\r\n", pwm_freq);
        wait(0.2);
        
        sensors::setFrequency(i2c_freq);
        pc.printf("I2C frequency: %dHz\r\n", i2c_freq);
        wait(0.2);
        
        measure_temperatures.attach(&update_temperatures_ISR, 2.0);
        calibrationButton.fall(&calibrate_ISR);
        pc.printf("[SETUP OK]\r\n");
        wait(0.5);
    }

    uint8_t self_check(void) {
        uint8_t response;
        //initialise ADCs
        response = measurements::init();
        if(response != NS_OK) {
            printf("[ERROR] ADC error 0x%X\r\n", response);
            return response;
        }
        printf("[OK] ADC initialised\r\n");

        //check for calibration data from EEPROM
        response = calibration::check();
        if(response != NS_OK) {
            printf("[ERROR] Device Not Calibrated 0x%X\r\n", response);
            return response;
        }
        printf("[OK] Device Calibrated\r\n");
        
        //measure Vdc < 350
        response = measurements::getVoltage();
        if(response != NS_OK) {
            printf("[ERROR] DC Over-Voltage %7.2fV\r\n", data.moment_voltage);
            return response;
        }
        printf("[OK] V = %7.2fV\r\n", data.moment_voltage);
        //maybe should also check for Vdc == 0 - solar disconnected
        
        //measure Idc == ~0.0
        response = measurements::getCurrent();
        if(response != NS_OK) {
            printf("[ERROR] DC Over-Current %7.2fA\r\n", data.moment_current);
            return response;
        }
        if(data.moment_current < 0.2) {
            printf("[OK] I = %7.2fA\r\n", data.moment_current);
        } else {
            printf("[ERROR] DC Current Leaks %7.2fA\r\n", data.moment_current);
            return DC_CURRENT_LEAKS;
        }
        
        //check Tcap and Tmosfet
        response = temperature::update();
        if(response != NS_OK) {
            printf("[ERROR] Overheat airgap: %7.2f radiator: %7.2f\r\n", data.airgap_temperature, data.radiator_temperature);
            return response;
        }
        printf("[OK] Temperatures airgap: %7.2f radiator: %7.2f\r\n", data.airgap_temperature, data.radiator_temperature);
        
        //check PWM generator
        //TODO
        
        return NS_OK;
    }
}

namespace temperature {
    float overheat = 70.0;
        
    uint8_t init(void) {
        probe_1.begin();
        probe_1.startConversion();
        probe_2.begin();
        probe_2.startConversion();
        wait(0.2);
        
        //TODO add check to see if thermometers are connected;
        return NS_OK;
    }
    
    uint8_t update(void) {
        data.airgap_temperature = probe_1.read();
        data.radiator_temperature = probe_2.read();
        
        if((data.airgap_temperature >= overheat) && (data.radiator_temperature >= overheat)) {
            return OVERHEAT;
        } else {
            if(data.airgap_temperature >= overheat) {
                return AIRGAP_OVERHEAT;
            }
            if(data.radiator_temperature >= overheat) {
                return RADIATOR_OVERHEAT;
            }
        }
        
        probe_1.startConversion();
        probe_2.startConversion();
        return NS_OK;
    }
}

void update_temperatures_ISR(void) {
    temperature::update();
}

void calibrate_ISR(void) {
    calibrating = true;
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
