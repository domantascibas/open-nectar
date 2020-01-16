// #include "mbed.h"
#include "consts.h"
#include "pins.h"
#include "DS1820.h"
#include "TemperatureSensor.h"
// #include "error_controller.h"

DS1820 temp_probe(BOILER_TEMP_PIN);
// Ticker ticker;
// Timeout timeout;

uint8_t temperature;
uint8_t last_temperature;
uint8_t error_counter;
uint8_t sensor_found;
uint8_t identical_count;
uint8_t refresh_rate;
uint8_t new_value_avail;

// must implement state machine with 4 states:
// - idle
// - start conversion
// - wait for conversion end
// - temperature ready

void temperatureSensor_init(void) {
	temperature = 0.0;
	last_temperature = 0.0;
	error_counter = 0;
	if(temp_probe.begin()) {
		sensor_found = 1;
		// temperatureSensor_start();
		// attachTicker(refreshRate);
	}
//    printf("Temperature sensor found\r\n");
//  } else printf("Temperature sensor not found\r\n");
}

uint8_t temperatureSensor_isFound(void) {
	return sensor_found;
}

uint8_t temperatureSensor_isNewValAvail(void) {
	return new_value_avail;
}

uint8_t temperatureSensor_get(void) {
	new_value_avail = 0;
	// return temperature;
	return 85;
}

uint8_t temperatureSensor_start(void) {
	temp_probe.startConversion();
}

void temperatureSensor_read(void) {
	// if((identical_count >= LOST_SENSOR_COUNT) || mainBoardError.has_error(NO_BOILER_TEMP)) {
	if (identical_count >= LOST_SENSOR_COUNT) {
		if (temp_probe.begin()) {
			sensor_found = 1;
			identical_count = 0;
		} else {
			sensor_found = 0;
			identical_count = LOST_SENSOR_COUNT;
		}
	}

	if (sensor_found) {
		float t;
		last_temperature = temperature;
		temp_probe.read(t);
		temperature = (uint8_t)t;

		if (temperature == last_temperature) {
			identical_count++;
		} else {
			identical_count = 0;
		}

		if (temperature != 85) {
			// if(mainBoardError.has_error(NO_BOILER_TEMP)) {
			// 	mainBoardError.clear_error(NO_BOILER_TEMP);
			// }
			error_counter = 0;
			new_value_avail = 1;
		} else {
			error_counter++;
		}

		if(error_counter >= 10) {
			error_counter = 10;
			temperature = 0;
			// mainBoardError.set_error(NO_BOILER_TEMP);
			new_value_avail = 1;
		}
	} else {
		temperature = 0;
		// mainBoardError.set_error(NO_BOILER_TEMP);
		new_value_avail = 1;
	}
}
