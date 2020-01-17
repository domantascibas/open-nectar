#ifndef _NS_ERROR_HANDLER_H
#define _NS_ERROR_HANDLER_H

#include <stdint.h>
#include "error_codes.h"

typedef struct {
    uint16_t error;
    uint8_t hasError;
} Error_t;

uint8_t error_init(uint16_t error);
uint8_t error_set(NS_ERROR error);
uint16_t error_get(void);
uint8_t error_clear(NS_ERROR error);
uint8_t error_clearAll(void);
void error_print(void);

struct ErrorHandler {
public:
    ErrorHandler();
    void set_error(ERROR_CODE);
    void get_errors(uint32_t *);
    uint32_t get_errors();
    void save_error_code(uint32_t);
    void get_last_error(ERROR_CODE *);
    void clear_error();
    void clear_error(ERROR_CODE);
    bool has_error(ERROR_CODE);
    bool has_errors;

private:
    uint32_t raised_errors;
    ERROR_CODE last_error;
    void print_error(ERROR_CODE *);
};

#endif
