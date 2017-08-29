#include "mbed.h"
//#include "stdlib.h"
#include "measurements.h"
#include "storage.h"
#include "device_modes.h"
#include "data.h"

static const PinName SDA = PB_14;
static const PinName SCL = PB_13;
static const PinName CALIBRATION_BTN = PC_12;
static const PinName USER_LED = PA_15;

#define REG_ADDR_RESULT         0x00
#define REG_ADDR_ALERT          0x01
#define REG_ADDR_CONFIG         0x02
#define REG_ADDR_LIMITL         0x03
#define REG_ADDR_LIMITH         0x04
#define REG_ADDR_HYST           0x05
#define REG_ADDR_CONVL          0x06
#define REG_ADDR_CONVH          0x07

static const uint8_t USENSE_ADDR = 0x55 << 1;
static const uint8_t ISENSE_ADDR = 0x5A << 1;

static const uint8_t RUNNING_SAMPLES = 128;
static const uint16_t CALIBRATION_SAMPLES = 1024;

static const float INPUT_VDIV = 4.1/400;
static const float CURRENT_OFFSET = 0.00;
static const float V_REF = 3.00;

Ticker measure_voltage;
Ticker measure_current;
I2C i2c(SDA, SCL);
InterruptIn calibration_button(CALIBRATION_BTN);
DigitalOut led(USER_LED);

//SENSORS
namespace sensors {
  volatile bool measure_v = false;
  volatile bool measure_i = false;
  volatile bool calibrate_sensors = false;
  
  void measure_voltage_ISR() {
    led = !led;
    measure_v = true;
  }
  
  void measure_current_ISR() {
    measure_i = true;
  }
  
  void calibrate_sensors_ISR() {
    calibrate_sensors = true;
  }

  uint8_t check(uint8_t addr) {
    char cmd[2];
    uint8_t error;

    cmd[0] = REG_ADDR_CONFIG;
    cmd[1] = 0x20;

    error = i2c.write(addr, cmd, 0);
    if(error == 0) {
      printf("[ok] ADC at 0x%X found\r\n", addr);
      i2c.write(addr, cmd, 2, false);
      return NS_OK;
    } else {
      printf("[ERROR] ADC at 0x%X not found!\r\n", addr);
      return NS_ERROR;
    }
  }

  float read(uint8_t addr) {
    char cmd[2];
    float raw;

    cmd[0] = REG_ADDR_RESULT;
    i2c.write(addr, cmd, 1);
    i2c.read(addr, cmd, 2);
    raw = ((cmd[0] & 0x0F) << 8) | cmd[1];
    return (raw * V_REF) / 2048; //raw * VREF * 2 / 4096
  }

  float sample(uint8_t addr, uint16_t samples) {
    float sum = 0;
    float avg = 0;
    uint16_t i = 0;

    for(i=0; i<samples; i++) {
      sum += read(addr);
    }
    avg = sum/samples;
    if(avg < 0) {
      avg = 0;
    }
    //printf("ADC 0x%X %.2f\r\n", addr, avg);
    return avg;
  }

  uint8_t calibrate() {
    uint8_t response = storage::init();
    device_modes::start();
    data.reference_voltage = sample(USENSE_ADDR, CALIBRATION_SAMPLES);
    data.reference_current = sample(ISENSE_ADDR, CALIBRATION_SAMPLES);
    device_modes::stop();
    response = storage::saveData(data.reference_voltage, data.reference_current);
    if(response != NS_OK) {
      return response;
    }
    return NS_OK;
  }
  
  void get_voltage() {
    float voltage;
    voltage = (sample(USENSE_ADDR, RUNNING_SAMPLES) - data.reference_voltage) / INPUT_VDIV;
    if(voltage >= VOLTAGE_LIMIT) {
      data.moment_voltage = voltage;
      data.error = DC_OVER_VOLTAGE;
      printf("[ERROR] DC Over-Voltage %7.2fV\r\n", data.moment_voltage);
      return;
    } else {
//      if(voltage < 0) {
//        data.moment_voltage = 0;
//      } else {
        data.moment_voltage = voltage;
//      }
    }
  }
  
  void get_current() {
    float current;
    current = (sample(ISENSE_ADDR, RUNNING_SAMPLES) - data.reference_current) * 5.000 - CURRENT_OFFSET;
    if(current >= CURRENT_LIMIT) {
      data.moment_current = current;
      data.error = DC_OVER_CURRENT;
      printf("[ERROR] DC Over-Current %7.2fA\r\n", data.moment_current);
      return;
    } else {
//      if(current < 0) {
//        data.moment_current = 0;
//      } else {
        data.moment_current = current;
//      }
    }
  }
  
  void setup() {
    static const uint32_t i2c_frequency = 400000;
    static const float update_interval = 0.5;
    calibration_button.fall(&calibrate_sensors_ISR);
    
    printf("[MEASUREMENTS]\r\n");
    i2c.frequency(i2c_frequency);		//I2C stops running at 1MHz
    printf("[ok] i2c %d Hz\r\n", i2c_frequency);
    //init ADCs
    uint8_t adc1_resp = check(USENSE_ADDR);
    uint8_t adc2_resp = check(ISENSE_ADDR);
    
    if(adc1_resp != NS_OK) {
      printf("[ERROR] Voltage ADC not found\r\n");
      data.error = ADC_VOLTAGE_ERROR;
      return;
    }
    if(adc2_resp != NS_OK) {
      printf("[ERROR] Current ADC not found\r\n");
      data.error = ADC_CURRENT_ERROR;
      return;
    }
    printf("[ok] ADC\r\n");
    
    //check for calibration data from EEPROM
    calibration::check();
    if(data.error != 0x00) return;
    printf("[ok] calibrated\r\nref: %.5fV %.5fA\r\n", data.reference_voltage, data.reference_current);
    
    //measure Vdc < 350
    get_voltage();
    if(data.error != 0x00) return;
    printf("[ok] V = %7.2fV\r\n", data.moment_voltage);
    
    //measure Idc == ~0.0
    get_current();
    if(data.error != 0x00) return;
    if(data.moment_current > 0.2) {
      printf("[ERROR] DC Current Leaks %7.2fA\r\n", data.moment_current);
      data.error = DC_CURRENT_LEAKS;
      return;
    }
    printf("[ok] I = %7.2fA\r\n", data.moment_current);
    
    //check Tcap and Tmosfet
    //TODO
    
    //check PWM generator
    //TODO
    
    measure_voltage.attach(&measure_voltage_ISR, update_interval);
    measure_current.attach(&measure_current_ISR, update_interval);
    printf("[ok] interval %.2fs\r\n\n", update_interval);
  }
  
  void loop() {
    if(measure_v) {
      measure_v = false;
      get_voltage();
      if(data.error != 0x00) {
        if(data.current_state != STOP) {
          printf("[ERROR] stopping device 0x%X\r\n", data.error);
          device_modes::stop();
        }
        return;
      }
      //printf("V %.2fV\r\n", data.moment_voltage);
    }
    if(measure_i) {
      measure_i = false;
      get_current();
      if(data.error != 0x00) {
        printf("[ERROR] stopping device 0x%X\r\n", data.error);
        device_modes::stop();
        return;
      }
      //printf("I %.2fA\r\n", data.moment_current);
    }
    if(calibrate_sensors) {
      calibrate_sensors = false;
      if(data.current_state == IDLE) {
        printf("Calibrating\r\n");
        measure_voltage.detach();
        measure_current.detach();
        calibrate();
        measure_voltage.attach(&measure_voltage_ISR, 0.5);
        measure_current.attach(&measure_current_ISR, 0.5);
        printf("[ok] calibrated\r\nref: %.5fV %.5fA\r\n", data.reference_voltage, data.reference_current);
      } else {
        printf("[ERROR] can't calibrate. not in IDLE\r\n");
      }
    }
  }
}

//CALIBRATION
namespace calibration {
  void check() {
    uint8_t response = storage::init();
    if(response == NS_OK) {
      if(!data.calibrated) {
        //could run calibration when no values are found on flash
        //measurements::calibrate()
        data.error = CALIBRATION_ERROR;
        printf("[ERROR] Device Not Calibrated 0x%X\r\n\n", data.error);
        return;
      }
    } else {
      //storage init error
      data.error = response;
      printf("[ERROR] CAN'T ACCESS CALIBRATION DATA 0x%X\r\n\n", data.error);
      return;
    }
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
