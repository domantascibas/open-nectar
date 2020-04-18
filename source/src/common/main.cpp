#include "init.h"
#include "mbed.h"

int main() {
    printf("\r\n");
    // should print current time
    printf("\r\n***** START *****\r\n");
    print_device_info();

    hw_init();
    run();
}
