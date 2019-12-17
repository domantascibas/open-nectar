#ifndef _NS_ERROR_HANDLER_H
#define _NS_ERROR_HANDLER_H

#include <stdint.h>
#include "error_codes.h"

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
