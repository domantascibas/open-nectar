#include "mbed.h"
#include "rtos.h"
#include "modes.h"
#include "comms.h"
#include "utils.h"
#include "pin_names.h"

DigitalOut led1(LED1);
Thread device_update;
Thread blinky;
Serial pc(USBTX, USBRX);

extern DigitalOut relay_sun_on;
extern DigitalOut relay_grid_on;
extern Serial comms_power;
extern Serial comms_esp;

void print_thread(void);

void default_mode_test(void) {
    device_tests::set_temperature_max(80.0);
    
    device_tests::set_pv_available(false);
    device_tests::set_temperature_current(81.0);
    device_tests::set_temperature_scheduled(50.0);
    device::run(MODE_DEFAULT);
    
    device_tests::set_pv_available(false);
    device_tests::set_temperature_current(52.0);
    device_tests::set_temperature_scheduled(50.0);
    device::run(MODE_DEFAULT);
    
    device_tests::set_pv_available(false);
    device_tests::set_temperature_current(35.0);
    device_tests::set_temperature_scheduled(50.0);
    device::run(MODE_DEFAULT);
    
    device_tests::set_pv_available(true);
    device_tests::set_temperature_current(52.0);
    device_tests::set_temperature_scheduled(50.0);
    device::run(MODE_DEFAULT);
    
    device_tests::set_pv_available(true);
    device_tests::set_temperature_current(81.0);
    device_tests::set_temperature_scheduled(50.0);
    device::run(MODE_DEFAULT);
}

void print_char(char c = '*') {
    printf("%c", c);
    fflush(stdout);
}

void print_thread(void) {
    while (true) {
        Thread::wait(500);
        print_char();
        led1 = !led1;
    }
}

int main() {
    //comms_power.printf("ready to go\n\r");
    //device::set_relays(TURN_OFF_ALL);
    
    //printf("\n\n\r[ STARTING ]\n\r");
    //power_board::start();
    //wait(1);
    //blinky.start(&print_thread);
    //default_mode_test();
    //power_board::stop();
    
    //pc.printf("Received: %5.3f\n\r", power_board::get_voltage());
    pc.printf("Starting\n\r");
    //comms_power.putc(0x56);
    double voltage = 120.50;
    int voltage_int = voltage * 2049;
    comms_power.putc(voltage_int);
    //comms_power.printf("%f", voltage);
    pc.printf("done\n\r");
    while(1) {
        __WFI();
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
