#include <stdio.h>
#include "error_handler.h"

Error_t nsError = {0, 0};

#define ERROR_CLEAR(x)          (nsError.error &= (uint16_t)~(1U << x))
#define ERROR_SET(x)            (nsError.error |= (uint16_t)(1U << x))
#define ERROR_GET(x)            ((nsError.error & (uint16_t)(1U << x)) >> x)

uint8_t error_init(uint16_t error) {
    nsError.hasError = 0;
    nsError.error = error;
    if (error != 0) {
        nsError.hasError = 1;
    }
    return 1;
}

uint8_t error_set(NS_ERROR_t error) {
    ERROR_SET(error);
    nsError.hasError = 1;
    return 1;
}

uint16_t error_get(void) {
    return nsError.error;
}

uint8_t error_hasError(void) {
    return nsError.hasError;
}

uint8_t error_isSet(NS_ERROR_t error) {
    if (nsError.hasError && ((nsError.error & (1U << error)) >> error)) {
        return 1;
    }
    return 0;
}

uint8_t error_clear(NS_ERROR_t error) {
    ERROR_CLEAR(error);
    if (nsError.error == 0) {
        nsError.hasError = 0;
    }
    return 1;
}

uint8_t error_clearAll(void) {
    nsError.error = 0;
    nsError.hasError = 0;
    return 1;
}

void error_print(void) {
    if (nsError.hasError) {
        printf("[NS-ERROR] errors present: 0x%2X\r\n", nsError.error);
    } else {
        printf("[NS-ERROR] no errors present\r\n");
    }
}
