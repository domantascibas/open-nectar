#include "mbed.h"
#include "rtos.h"
#include "hardware.h"
#include "modes.h"
#include "comms.h"

Thread thread1;
Thread thread2;
Thread thread3;

void service_thread() {
    while(1) service::loop();
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

void update_device() {
    while(1) {
        temperature::update();
        Thread::wait(500);
        nectar::loop();
        Thread::wait(500);
    }
}

namespace threads {
    uint8_t start(void) {
        thread1.start(power_thread);
        thread2.start(esp_thread);
        thread3.start(update_device);
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
