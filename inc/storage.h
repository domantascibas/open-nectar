#ifndef STORAGE_H
#define STORAGE_H

struct StoredItem {
  StoredItem(const uint16_t address) 
    : lo_address(address), hi_address(address + 0x100) {};
  void load(float *);
  void read(float *);
  void save(float);

  private:
    const uint16_t lo_address;
    const uint16_t hi_address;
  
    union {
      float f;
      uint16_t b[2];
    } u;
};

namespace Storage {
  uint8_t init();
  void load_data(float *, float *, float *, float *);
  void save_data(float, float, float, float);
};

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
