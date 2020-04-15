#ifndef SET_TEMPERATURE_SCREEN_H
#define SET_TEMPERATURE_SCREEN_H

#include "../../helpers/helpers.h"
#include "../../menu_service.h"
#include "../ScreenModel.h"

struct SetTemperatureScreen : ScreenModel {
    SetTemperatureScreen(const TemperatureType &type, int8_t temperature)
        : ScreenModel(), type(type), temperature(temperature) {

        if (type == TemperatureMax) {
            uint8_t day = menu_actions::temperature(TemperatureDay);
            uint8_t night = menu_actions::temperature(TemperatureNight);
            uint8_t max = day > night ? day : night;
            // TODO: Use global max temperature
            if (temperature < 90)
                hasUpModel = true;
            if (temperature > max)
                hasDownModel = true;
        } else {
            if (temperature < menu_actions::temperature(TemperatureMax))
                hasUpModel = true;
            if (temperature > 20)
                hasDownModel = true;
        }

        hasNextModel = true;

        title = itos(temperature) + "°";
        screenType = Selector;
        selector = ScreenSelector(1, 0);
    };

    int8_t temperature;
    TemperatureType type;

    virtual ScreenModel *upModel();
    virtual ScreenModel *downModel();
    virtual ScreenModel *nextModel();
};

#endif
