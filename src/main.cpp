#include "mbed.h"
#include "rtos.h"
#include "modes.h"
#include "utils.h"

DigitalOut led1(LED1);
Thread device_update;
Thread blinky;

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
    printf("\n\n\r[ STARTING ]\n\r");
    //blinky.start(&print_thread);
    default_mode_test();

    while(1) {
        __WFI();
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
