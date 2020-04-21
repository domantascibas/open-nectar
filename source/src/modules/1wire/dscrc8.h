#ifndef DSCRC8_H
#define DSCRC8_H

#include <stdint.h>
// #include "config.h"

typedef uint8_t crc8_t;

/** Update CRC for a single byte.
@param crc CRC value
@param val value to update CRC with
@return new CRC  */
crc8_t dscrc8_byte(crc8_t crc, uint8_t val);

/** Update CRC for a number of bytes.
@param crc CRC value
@param bytes pointer to array of bytes
@param size number of bytes to process
@return new CRC  */
crc8_t dscrc8(crc8_t crc, uint8_t *bytes, uint8_t size);

#endif /* DSCRC8_H  */
