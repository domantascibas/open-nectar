#ifndef _NS_STORAGE_H
#define _NS_STORAGE_H

#include <stdint.h>
#include "data.h"
#include "eeprom.h"

namespace Storage {
  void init();
  void load_data(float *, float *, float *, float *);
  void save_data(float, float);
  void save_meters(float, float);
};

#endif
