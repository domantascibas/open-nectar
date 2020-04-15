#ifndef _NS_ERROR_HANDLER_H
#define _NS_ERROR_HANDLER_H

#include <stdint.h>
#include "error_codes.h"

typedef struct {
    uint16_t error;
    uint8_t hasError;
} Error_t;

uint8_t error_init(void);
uint8_t error_set(NS_ERROR_t error);
uint16_t error_get(void);
uint8_t error_hasError(void);
uint8_t error_isSet(NS_ERROR_t error);
uint8_t error_clear(NS_ERROR_t error);
uint8_t error_clearAll(void);
void error_print(void);

#endif
