#include "PairingScreen.h"

#include "../00/ConfirmationScreen.h"
#include "BackScreen.h"
#include "LanguageScreen.h"

ScreenModel *PairingScreen::upModel() { return new LanguageScreen(); };

ScreenModel *PairingScreen::downModel() { return new BackScreen(); };

ScreenModel *PairingScreen::nextModel() {
  return new ConfirmationScreen(
      Pairing, LocalizedString(LocalizationPairingStart), true);
};
