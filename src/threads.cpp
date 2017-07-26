#include "mbed.h"
#include "rtos.h"
#include "hardware.h"

Thread thread1;
Thread thread2;
Thread thread3;
Thread thread4;

void service_thread() {
    while(1) comms::service_loop();
}

void esp_thread() {
    while(1) comms::esp_loop();
}

void check_heatsink_temperature() {
    while(1) {
        //measure T heatsink
        //if heatsink temp > safe temp ? shutdown power board : run power board
        Thread::wait(5000);
    }
}

void check_airgap_temperature() {
    while(1) {
        //measure T capacitor
        //if airgap_temp ? shutdown power board : run power board
        Thread::wait(5000);
    }
}

void update_temperatures() {
    while(1) {
        temperature::update();
        Thread::wait(1000);
        
        
        //if(data.device_temperature > 75.0) {
        //    power_board::shutdown(1);
        //}
    }
}

namespace threads {
    uint8_t start(void) {
        thread1.start(service_thread);
        thread2.start(esp_thread);
        thread3.start(update_temperatures);
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
