#ifndef CONFIRMATION_SCREEN_H
#define CONFIRMATION_SCREEN_H

#include "../ScreenModel.h"
#include <iostream>

enum ConfirmationType { Reset, Pairing, Calibration };

struct ConfirmationScreen : ScreenModel {
    ConfirmationScreen(const ConfirmationType &type, const std::string &_title,
            const bool &isYes)
        : ScreenModel(), type(type), isYes(isYes) {

        hasUpModel = true;
        hasDownModel = true;
        hasNextModel = true;

        title = _title;
        subtitle = isYes ? LocalizedString(LocalizationYes)
                : LocalizedString(LocalizationNo);
        screenType = Subtitled;
    };

    bool isYes;
    ConfirmationType type;

    virtual ScreenModel *upModel();
    virtual ScreenModel *downModel();
    virtual ScreenModel *nextModel();
};

#endif
