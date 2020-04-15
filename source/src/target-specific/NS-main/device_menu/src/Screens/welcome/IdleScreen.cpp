#include "IdleScreen.h"

#include "../01/ActiveStatusScreen.h"

ScreenModel *IdleScreen::upModel() { return new ActiveStatusScreen(); };
ScreenModel *IdleScreen::downModel() { return new ActiveStatusScreen(); };
ScreenModel *IdleScreen::nextModel() { return new ActiveStatusScreen(); };
