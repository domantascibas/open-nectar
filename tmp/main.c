#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "data.h"

/* USER FUNCTIONS */

void pwr_check(statusDataType_t status) {
    uint8_t i;
    for (i = 1; i < 16; ) {
        PowerData_write(STATUS, &i);
        // printf("DATA_WRITE %d , GET_STATUS %d , CLEAR_STATUS %d , SET_STATUS %d\n\r", i, GET_STATUS(CALIBRATION_STATUS), CLEAR_STATUS(CALIBRATION_STATUS), SET_STATUS(CALIBRATION_STATUS));
        printf("i: %d , VAL: %d , STATUS: %d , GET_STATUS(): %d\n\r", i, *PowerData_getStatusPtr(), status, GET_STATUS(status));
        if (GET_STATUS(status) != (i & (1UL << status) >> status)) {
            printf("[ERROR] GET_STATUS(): %d != (%d << %d) >> %d", GET_STATUS(status), i, status, status);
        }
        i *= 2;
    }
    printf("\n\r");
}

void pwr_setStatus(statusDataType_t status) {
    SET_STATUS(status);
    // printf("DATA_WRITE %d , GET_STATUS %d , CLEAR_STATUS %d , SET_STATUS %d\n\r", i, GET_STATUS(CALIBRATION_STATUS), CLEAR_STATUS(CALIBRATION_STATUS), SET_STATUS(CALIBRATION_STATUS));
    printf("VAL: %d , STATUS: %d , GET_STATUS(): %d\n\r", *PowerData_getStatusPtr(), status, GET_STATUS(status));
    if (GET_STATUS(status) != 1) {
        printf("[ERROR] SET_STATUS(): %d != 1", GET_STATUS(status));
    }
    CLEAR_STATUS(status);
    if (GET_STATUS(status) != 0) {
        printf("[ERROR] GET_STATUS(): %d != 1", GET_STATUS(status));
    }
    printf("VAL: %d , STATUS: %d , GET_STATUS(): %d\n\r", *PowerData_getStatusPtr(), status, GET_STATUS(status));
    printf("\n\r");
}



/* MAIN FUNCTIONS */

void delay(int number_of_seconds) { 
    int milli_seconds = 1000 * number_of_seconds; 
    clock_t start_time = clock(); 
    while (clock() < start_time + milli_seconds);
}

void init_main(void) {
    uint8_t res;
    res = PowerData_init();
}

void loop(void) {
    static uint8_t cnt = 0;
    printf("runs every 1s, %d\n\r", cnt);
    cnt++;
    /* USER CODE START */

    for (uint8_t i = 0; i < 4; i++) {
        // pwr_check((statusDataType_t)i);
        pwr_setStatus((statusDataType_t)i);
    }

    /* USER CODE END */
    delay(1000);
}

int main(void) {
    init_main();
    for (uint8_t i = 0; i < 1; i++) {
        loop();
    }
    return 0;
}
