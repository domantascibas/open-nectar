#include "mbed.h"
#include "rtos.h"
#include "hardware.h"
#include "modes.h"
#include "power_board.h"
#include "service.h"
#include "esp.h"

Thread thread1;
Thread thread2;
Thread thread3;

void service_thread() {
    while(1) {
        service::loop();
    }
}

void power_thread() {
    while(1) {
        power_board::loop();
        Thread::wait(5000);
    }
}

void esp_thread() {
    while(1) {
        esp::loop();
    }
}

void main_board_thread() {
    while(1) {
        temperature::update();
        Thread::wait(500);
        nectar::loop();
        Thread::wait(500);
    }
}

int main() {
    hardware::setup();
    hardware::startup();
    
    thread1.start(power_thread);
    //thread2.start(esp_thread);
    //thread3.start(main_board_thread);
    
    while(1) {
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
