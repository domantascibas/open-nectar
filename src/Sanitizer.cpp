#include "mbed.h"
#include "Sanitizer.h"
#include "device_modes.h"
#include "DataService.h"

void sanitizerResetCounter(void);
uint16_t sanitizerGetPeriodCounter(void);
bool sanitizerIsOn(void);

static uint16_t sanitizer_period_counter = 0;
static bool sanitizer_heating_on = false;

void sanitizerResetCounter(void) {
	sanitizer_period_counter = 0;
}

void sanitizerIncreaseCounter(void) {
	if(sanitizer_period_counter < LEGIONELLA_PREVENTION_PERIOD) {
		sanitizer_period_counter++;
	} else {
		sanitizerTurnOn(true);
		sanitizerResetCounter();
	}
}

uint16_t sanitizerGetPeriodCounter(void) {
	return sanitizer_period_counter;
}

bool sanitizerIsOn(void) {
	return sanitizer_heating_on;
}

void sanitizerTurnOn(bool turn_on) {
	if(turn_on && !DataService::isDayTime()) {
		sanitizer_heating_on = true;
		DataService::setCurrentHeaterMode(nectar_contract::Boost);
	} else {
		sanitizer_heating_on = false;
		sanitizerResetCounter();
	}
}
