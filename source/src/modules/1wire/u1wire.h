#ifndef _NS_U1WIRE_H
#define _NS_U1WIRE_H

#include <stdint.h>

/** One wire bus error codes.  */
typedef enum {
    U1WIRE_ERR_BUS_LOW = 1,        /* Can't drive bus high.  */
    U1WIRE_ERR_BUS_STUCK = 2,      /* Bus stuck low.  */
    U1WIRE_ERR_PRESENCE_SHORT = 3, /* Slave presence pulse too short.  */
    U1WIRE_ERR_PRESENCE_LONG = 4,  /* Slave presence pulse too long.  */
    U1WIRE_ERR_MULTIPLE_DEVICES = 5,
    U1WIRE_ERR_BUS_HIGH = 6,     /* Can't drive bus low.  */
    U1WIRE_ERR_CRC = 7,          /* CRC error.  */
    U1WIRE_ERR_WRONG_DEVICE = 8, /* Unexpected device on bus.  */
    U1WIRE_ERR_DISCONNECT = 9,   /* Device disappeared?  */
} u1wire_err_t;

/** One wire bus ROM code structure.  */
typedef union {
    struct {
        uint8_t family;
        uint8_t serial[6];
        uint8_t crc;
    } fields;
    uint8_t bytes[8];
} u1wire_rom_code_t;

/** Define one wire bus device structure.  */
typedef struct {
    u1wire_rom_code_t rom_code;
} u1wire_obj_t;

typedef u1wire_obj_t *u1wire_t;

/** Determine if one wire bus operation has completed.
@return 1 if one wire bus operation has completed otherwise 0  */
uint8_t u1wire_ready_p(void);

/** Reset one wire bus.
@return 1 if OK, 0 if no device responding, negative if error  */
int8_t u1wire_reset(void);

/** Write a bit on the one wire bus.
@param value bit to write  */
void u1wire_bit_write(uint8_t value);

/** Write a byte on the one wire bus.
@param value byte to write.  */
void u1wire_byte_write(uint8_t value);

/** Read a bit from the one wire bus.
@return bit read  */
uint8_t u1wire_bit_read(void);

/** Read a byte from the one wire bus.
@return byte read  */
uint8_t u1wire_byte_read(void);

/** Send command to selected device on one wire bus.
@param dev pointer to one wire device structure
@param command command to send
@return 1 if OK, 0 if no device responding, negative if error  */
int8_t u1wire_command(u1wire_t dev, uint8_t command);

/** Broadcast command to all devices on one wire bus.
@param command command to send
@return 1 if OK, 0 if no device responding, negative if error  */
int8_t u1wire_broadcast(uint8_t command);

/** Read number of bytes of data from one wire bus.
@param data pointer to buffer to write to
@param size number of bytes to write
@return number of bytes read, 0 if no device responding, negative if error  */
int8_t u1wire_read(void *data, uint8_t size);

/** Write number of bytes of data from one wire bus.
@param data pointer to buffer to write to
@param size number of bytes to write
@return number of bytes written, 0 if no device responding, negative if error  */
int8_t u1wire_write(void *data, uint8_t size);

/** Initialise one wire bus and discover if there is a single device on the bus.
@param device pointer to one wire device structure
@return 1 if OK, 0 if no device responding, negative if error  */
int8_t u1wire_init(u1wire_obj_t *device);

/* In u1wire_debug.c  */
void u1wire_debug(u1wire_t dev);

#endif
