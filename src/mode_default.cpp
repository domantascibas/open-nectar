#include "mbed.h"
#include "mode_default.h"
#include "utils.h"

int mode_default::run(double temp_max, double temp_current, bool pv_available) {
	if(pv_available) {
		//check if Tcurrent < Tmax ? use PV : do nothing
		if(temp_current < temp_max) {
			//turn on PV
			return TURN_ON_PV;
		} else {
			//do nothing
			return TURN_OFF_ALL;
		}
	} else {
		//check if Tcurrent < Tmax ? use GRID : do nothing
		if(temp_current < temp_max) {
			//turn on grid power
			return TURN_ON_GRID;
		} else {
			//do nothing
			return TURN_OFF_ALL;
		}
	}
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
