#ifndef STORAGE_H
#define STORAGE_H
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

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
