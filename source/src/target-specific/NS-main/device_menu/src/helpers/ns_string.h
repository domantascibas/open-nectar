#ifndef _NS_DEVICE_MENU_STRING_H
#define _NS_DEVICE_MENU_STRING_H

#include <sstream>
#include <string>

static std::string itos(int i) {
  std::ostringstream os;
  
  if(i < 10) {  
    os << '0' << i;
  } else {
    os << i;
  }
  return os.str();
};

#endif
