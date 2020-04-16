#include "menu_service.h"
#include "consts.h"
#include "screen_display.h"
#include "DataService.h"
// #include "error_controller.h"
#include "Storage.h"
#include "CommsController.h"
#include "device_modes.h"
// #include "Sanitizer.h"
// #include "BoostTimeout.h"

// #include "Screens/00/SetLanguageScreen.h"
// #include "Screens/01/ActiveStatusScreen.h"
// #include "Screens/error/DebugScreen.h"
// #include "Screens/error/ErrorScreen.h"
#include "Screens/welcome/IdleScreen.h"

// PD_2 button backlight control. active HIGH
DigitalOut buttonBacklight(MENU_BUTTON_BACKLIGHT_PIN);
InterruptIn keyUp(MENU_BUTTON_UP_PIN);          // KEY_L
InterruptIn keyDown(MENU_BUTTON_DOWN_PIN);      // KEY_R
InterruptIn keyCenter(MENU_BUTTON_CENTER_PIN);  // KEY_C

Timeout buttonDebouncer;
Timeout screenUpdater;
Timeout idleScreen;
Ticker timeKeeper;

Ticker longPressTicker;
Ticker enterPressTicker;
bool isLongPress = false;
bool isEnterPress = false;
bool isEnterPressReleased = true;
bool isFirst = true;
uint8_t longPressCounter = 0;
uint8_t enterPressCounter = 0;

ScreenModel *m_screen;

void setScreenModel(ScreenModel *screen_model) {
    ScreenDisplay::clear();
    delete m_screen;
    m_screen = screen_model;
}

namespace menu_service {
void returnToIdle(void);
volatile bool needUpdate = false;
bool resetScreen = false;

void enableButtons(void) {
    keyUp.rise(&upButtonPressed);
    keyDown.rise(&downButtonPressed);
    keyCenter.rise(&nextButtonPressed);
    printf("BUTTONS ENABLED\r\n");
}

void disableButtons(void) {
    keyUp.rise(NULL);
    keyDown.rise(NULL);
    keyCenter.rise(NULL);
    printf("BUTTONS DISABLED\r\n");
}

//   void enterTestScreen(void) {
//     deviceOpMode.setTestMode();
//     // if(menu_actions::hasErrors()) {
//     //   ScreenModel *error_screen = new ErrorScreen();
//     //   setScreenModel(error_screen);
//     // } else {
//       ScreenModel *debug_screen = new DebugScreen();
//       setScreenModel(debug_screen);
//     // }
//     needUpdate = true;
//   }

//  void enterMenuItem(void) {
//      if (!m_screen->hasNextModel)
//       return;
//     buttonBacklight = MENU_BUTTON_BACKLIGHT_ON;
//     setScreenModel(m_screen->nextModel());
//     needUpdate = true;
//     if (deviceOpMode.isOnboarding() || menu_actions::isInTestMode()) {
//       return;
//     } else {
//       idleScreen.attach(&returnToIdle, MENU_TIMEOUT_TO_IDLE);
//     }
//  }

//   void longPressISR(void) {
//     if (keyUp && keyDown) {
//       longPressCounter++;
//     } else {
//       longPressCounter = 0;
//       longPressTicker.detach();
//     }
//     if (longPressCounter >= (MENU_BUTTON_LONG_PRESS_PERIOD / MENU_BUTTON_DEBOUNCE_PERIOD)) {
//       longPressCounter = 0;
//       enterTestScreen();
//     }
//   }

// void enterPressISR(void) {
//  if(keyCenter) {
//      enterPressCounter++;
//  } else {
//      enterPressCounter = 0;
//      enterPressTicker.detach();
//      isEnterPressReleased = true;
//      enableButtons();
//  }
//  if(enterPressCounter >= (MENU_BUTTON_ENTER_PRESS_PERIOD / MENU_BUTTON_DEBOUNCE_PERIOD)) {
//      if(isEnterPressReleased) {
//          isEnterPressReleased = false;
//          enterPressCounter = 0;
//          enterMenuItem();
//      }
//  }
// }

void screenUpdaterISR(void) { needUpdate = true; }

void returnToIdle(void) {
    ScreenModel *idle_screen = new IdleScreen();
    setScreenModel(idle_screen);
    needUpdate = true;
    // buttonBacklight = MENU_BUTTON_BACKLIGHT_OFF;
}

void updateScreen(void) {
    needUpdate = false;

    if (!deviceOpMode_isLoading()) {
        if (isFirst) {
            isFirst = false;
            // buttonBacklight = MENU_BUTTON_BACKLIGHT_ON;
            // enableButtons();
            // if(!menu_actions::isInOnboarding()) {
            //   ScreenModel *active_screen = new ActiveStatusScreen();
            //   setScreenModel(active_screen);
            //   menu_actions::updateTime();
            //   idleScreen.attach(&returnToIdle, MENU_TIMEOUT_TO_IDLE);
            // } else {
            //   set_time(MENU_DEFAULT_INIT_TIME);
            //   time_t curr_time = time(NULL);
            //   printf("TIME %s", ctime(&curr_time));
            // }
        }

        //   if(menu_actions::isErrorCritical(powerBoardError.get_errors()) || menu_actions::isErrorCritical(mainBoardError.get_errors())) {
        //     if(menu_actions::isErrorCritical(powerBoardError.get_errors())) printf("[CRITICAL] POWER ERROR: 0x%X\n", powerBoardError.get_errors());
        //     if(menu_actions::isErrorCritical(mainBoardError.get_errors())) printf("[CRITICAL] MAIN ERROR: 0x%X\n", mainBoardError.get_errors());
        //     if(!menu_actions::isInTestMode()) {
        //       ScreenModel *error_screen = new ErrorScreen();
        //       setScreenModel(error_screen);
        //     }
        //   }

        if (resetScreen) {
            // resetScreen = false;
            // ScreenModel *active_screen = new ActiveStatusScreen();
            // setScreenModel(active_screen);
            // idleScreen.attach(&returnToIdle, MENU_TIMEOUT_TO_IDLE);
        }

        if (m_screen->refreshRate != 0) {
            screenUpdater.attach(&screenUpdaterISR, m_screen->refreshRate);
        } else {
            screenUpdater.detach();
        }
    }

    ScreenDisplay::render(m_screen);
}

void setup(void) {
    // buttonBacklight = MENU_BUTTON_BACKLIGHT_OFF;
    resetScreen = false;
    localization::setup();
    ScreenDisplay::setup();
    // m_screen = new SetLanguageScreen(localization::currentLanguage());
    m_screen = new IdleScreen();

    screenUpdater.attach(&screenUpdaterISR, 1);
    timeKeeper.attach(&menu_actions::updateTime, MENU_TIME_KEEPER_PERIOD);

    time_t curr_time = time(NULL);
    if (curr_time > 0) {
        printf("Current time %s\r\n", ctime(&curr_time));
    }
}

void upButtonPressed(void) {
//     disableButtons();
//     buttonDebouncer.attach(enableButtons, MENU_BUTTON_DEBOUNCE_PERIOD);
//     if(deviceOpMode.isOnboarding()) {
//       if (!isLongPress)
//         isLongPress = true;
//       longPressTicker.attach(longPressISR, MENU_BUTTON_DEBOUNCE_PERIOD);
//     }
//     if(!m_screen->hasUpModel)
//       return;
//     buttonBacklight = MENU_BUTTON_BACKLIGHT_ON;
//     setScreenModel(m_screen->upModel());
// //    printf("up pressed\r\n");
//     needUpdate = true;
//     if (deviceOpMode.isOnboarding() || menu_actions::isInTestMode()) {
//       return;
//     } else {
//       idleScreen.attach(&returnToIdle, MENU_TIMEOUT_TO_IDLE);
//     }
};

void downButtonPressed(void) {
//     disableButtons();
//     buttonDebouncer.attach(enableButtons, MENU_BUTTON_DEBOUNCE_PERIOD);
//     if (deviceOpMode.isOnboarding()) {
//       if (!isLongPress)
//         isLongPress = true;
//       longPressTicker.attach(longPressISR, MENU_BUTTON_DEBOUNCE_PERIOD);
//     }
//     if (!m_screen->hasDownModel)
//       return;
//     buttonBacklight = MENU_BUTTON_BACKLIGHT_ON;
//     setScreenModel(m_screen->downModel());
// //    printf("down pressed\r\n");
//     needUpdate = true;
//     if (deviceOpMode.isOnboarding() || menu_actions::isInTestMode()) {
//       return;
//     } else {
//       idleScreen.attach(&returnToIdle, MENU_TIMEOUT_TO_IDLE);
//     }
};

void nextButtonPressed(void) {
//     disableButtons();
//      if(!isEnterPress)
//          isEnterPress = true;
//      enterPressTicker.attach(enterPressISR, MENU_BUTTON_DEBOUNCE_PERIOD);
};
} // namespace menu_service

namespace menu_actions {
uint8_t pairingPeriodCounter;
uint8_t screenInitCounter = 0;
char textString[16];

bool hasConfig(void) {
    if (deviceOpMode_isInTestMode()) return false;
//    printf("has config %d\r\n", deviceOpMode.isConfigured());
    return deviceOpMode_isConfigured();
}
bool hasWifi(void) {
//    printf("has wifi %d\r\n", espData.has_internet_connection);
    return espData.has_internet_connection;
}

const HeaterMode &deviceMode(void) {
    return (HeaterMode)DataService::getCurrentHeaterMode();
};

void setDeviceMode(const HeaterMode &mode) {
    DataService::setCurrentHeaterMode((nectar_contract::HeaterMode)mode);
};

void reset(void) {
    // deviceOpMode.setReset(true);
    // commsController.sendEspMessage();
    // idleScreen.detach();
    // localization::setLanguage(EN);
    //  localization::saveLanguage();

    // deviceOpMode.resetConfiguration();
    // startPairing(true);
};

void startPairing(const bool &start) {
    // if(start) {
    //   deviceOpMode.setPairing(true);
    //   pairingPeriodCounter = 0;
    // } else {
    //   deviceOpMode.setPairing(false);
    //   pairingPeriodCounter = 0;
    // }
};

void calibratePowerBoard(void) {
    // DataService::setCalibrate(true);
};

bool isPairingOn(void) { return deviceOpMode_isPairing(); };
bool isInTestMode(void) { return deviceOpMode_isInTestMode(); };
bool isInOnboarding(void) { return deviceOpMode_isOnboarding(); };
bool isLoading(void) { return deviceOpMode_isLoading(); };
void endOnboarding(void) { deviceOpMode_endOnboarding(); };
void modeSelected(void) { DataService::modeSelected(); };

std::string getPin(void) {
    // if(isPairingOn()) {
    //   if(espData.pin != 0) {
    //     snprintf(textString, 5, "%d", espData.pin);
    //     return textString;
    //   } else {
    //     return LocalizedString(LocalizationPairingActive);
    //   }
    // } else {
    //   return LocalizedString(LocalizationPairingInactive);
    // }
};

//   uint32_t error(void) {
//     if(powerBoardError.has_errors) {
//       return powerBoardError.get_errors();
//     }
//     if(mainBoardError.has_errors) {
//       return mainBoardError.get_errors();
//     }

//     return 0xFFFF;
//   }

//   bool hasErrors(void) {
//     if(powerBoardError.has_errors || mainBoardError.has_errors) return true;
//     else return false;
//   }

//   bool isErrorCritical(uint32_t err) {
//     if((err == NONE) || (err == 0x00)) return false;
//     else {
//       if(isInTestMode()) {return true;
//       } else {
//         if((err == (1 << FLASH_ACCESS_ERROR)) ||
//           (err == (1 << DC_LOW_VOLTAGE)) ||
//           (err == (1 << DC_OVER_VOLTAGE)) ||
//           (err == (1 << DC_OVER_CURRENT)) ||
//           (err == (1 << DEVICE_OVERHEAT)) ||
//           (err == (1 << MAX_TEMPERATURE)) ||
//           (err == (1 << MIN_TEMPERATURE)) ||
//           (err == (1 << NO_LOAD))
//         ) return false;
//         else return true;
//       }
//     }
//   }

int8_t temperature(const TemperatureType &type) {
    // switch (type) {
    // case TemperatureDay:
    //   return temperatureData.getDayTemperature();
    // case TemperatureNight:
    //   return temperatureData.getNightTemperature();
    // case TemperatureMax:
    //   return temperatureData.getMaxTemperature();
    // }
};

void setTemperature(const TemperatureType &type, const int8_t &t) {
    // switch (type) {
    // case TemperatureDay:
    //   temperatureData.setDayTemperature(t);
    //   break;
    // case TemperatureNight:
    //   temperatureData.setNightTemperature(t);
    //   break;
    // case TemperatureMax:
    //   temperatureData.setMaxTemperature(t);
    //   break;
    // }
    // Storage::saveTemp(type, t);
};

time_hm _time(12, 00);
time_hm _time_day_start(6, 00);
time_hm _time_night_start(23, 00);

void updateTime(void) {
//     time_t rtc = time(NULL);

//     int8_t hrs = rtc / 60 / 60 % 24;
//     int8_t min = rtc / 60 % 60;

//     time_hm new_time(hrs, min);
//     _time = new_time;

//     if(pairingPeriodCounter >= MENU_PAIRING_PERIOD) {
//       pairingPeriodCounter = 0;
//       deviceOpMode_setPairing(0);
//     } else {
//       pairingPeriodCounter++;
//     }

//     if ((hrs == 0) && (min == 0)) {
//          DataService::calculateSolarKwhDiff(true);
//      }

//      if(screenInitCounter >= MENU_SCREEN_INIT_PERIOD) {
//          screenInitCounter = 0;
//          ScreenDisplay::init();
//      } else {
//          screenInitCounter++;
//      }

// //       sanitizerIncreaseCounter();
// //       boostTimeoutIncreaseCounter();
//     menu_service::needUpdate = true;
}

time_hm getTime(const TimeType &type) {
    // switch (type) {
    // case Current:
    //   return _time;
    // case DayStart:
    //   return _time_day_start;
    // case NightStart:
    //   return _time_night_start;
    // }
}

void setTime(const time_hm &nectar_time, const TimeType &type) {
//     time_t rtc = time(NULL);
//     switch (type) {
//     case Current:
//       set_time(rtc - (rtc % (24 * 60 * 60)) + nectar_time.hours * 60 * 60 + nectar_time.minutes * 60);
//       _time = nectar_time;
//       break;
//     case DayStart:
//       _time_day_start = nectar_time;
//       break;
//     case NightStart:
//       _time_night_start = nectar_time;
//       break;
//     }
//     Storage::saveTime(nectar_time, type);
}
} // namespace menu_actions
