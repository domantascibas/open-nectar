#ifndef _NS_SCREEN_DISPLAY_H
#define _NS_SCREEN_DISPLAY_H

#include "ScreenModel.h"
#include "u8g2.h"

namespace ScreenDisplay {
void setup();
void init();
void clear();
void render(ScreenModel *);
}

#endif
