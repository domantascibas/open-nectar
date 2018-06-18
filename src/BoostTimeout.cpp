#include "mbed.h"
#include "BoostTimeout.h"
#include "device_modes.h"

static uint8_t boost_timeout_counter = 0;
static bool boost_timeout_reached = false;
static bool boost_turned_on = false;

void boostTimeoutIncreaseCounter(void) {
	if(boost_timeout_counter < BOOST_TIMEOUT) {
		boost_timeout_counter++;
	} else {
		boost_timeout_reached = true;
	}
}

bool boostTimeoutReached(void) {
	return boost_timeout_reached;
}

void boostTimeoutReset(void) {
	boost_timeout_counter = 0;
	boost_timeout_reached = false;
}

void boostTimeoutStartCounter(void) {
	boost_turned_on = true;
	boostTimeoutReset();
}