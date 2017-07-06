#include "mbed.h"
#include "rtos.h"
#include "modes.h"
#include "utils.h"

void print_char(char c = '*') {
    printf("%c", c);
    fflush(stdout);
}

DigitalOut led1(LED1);
Thread device_update;

void fifteen_second_function(void);

void fifteen_second_function(void const*) {
	device::run(MODE_DEFAULT);
	Thread::wait(15000);
}

/*
void print_thread(void const *argument) {
    while (true) {
        Thread::wait(1000);
        print_char();
    }
}
*/

int main() {
	device_update.start(fifteen_second_function);

	/*
	printf("\n\n\r*** RTOS basic example ***\n\r");
    Thread thread(print_thread, NULL, osPriorityNormal, DEFAULT_STACK_SIZE);
    while (true) {
        led1 = !led1;
        Thread::wait(500);
    }
	*/
	//the main loop
	while(1) {
		__WFI();
	}
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
