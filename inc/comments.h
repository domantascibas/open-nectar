/*	
    while(1) {
		if(calibrating) {
            pc.printf("\n\r[ START ] Calibration\n\r");
			pc.printf("********************\n\r");
            calibrate();
            calibrating = false;
            wait(1.0);
            //pc.printf("V_ZERO = %7.3f I_ZERO = %7.3f\n\r", V_ZERO, I_ZERO);
            pc.printf("\n\r");
			//pc.printf("********************\n\r");
			pc.printf("[ DONE ] Calibration complete\n\r");
			pc.printf("\n\r");
            wait(2.0);
        }
		
		if(new_measurement) {
			led = !led;
			new_measurement = false;
			
			//sensing();
			
			curr_voltage = measure_voltage();
			curr_current = measure_current();
			curr_power = curr_current * curr_voltage;
						
			//pc.printf("%7.3fW %7.3fV %7.3fA\n\r", curr_voltage * curr_current, curr_voltage, curr_current);
			//adjust_pwm(curr_voltage, curr_current);	
			adjust_pwm(curr_voltage, curr_current);
			//pwm_swipe(0.15, 0.85, 0.05);
			
			pwm_gen.write(PWM_DUTY);
		}
		__WFI();
	}
	*/
	
/*
void adjust_pwm(double voltage, double current) {
	double power;
	double dP, dV, dI;
	double N = 0.01;
	
	power = voltage * current;
	dP = power - old_power;
	dV = voltage - old_voltage;
	dI = current - old_current;
	

	//if((dI != 0) && (dP != 0)){
	//	PWM_DUTY_STEP_CHANGE -= N * dP;
	//	if(PWM_DUTY_STEP_CHANGE < 0) PWM_DUTY_STEP_CHANGE = PWM_DUTY_STEP_CHANGE * (-1);
	//	if(PWM_DUTY_STEP_CHANGE > 1.0) PWM_DUTY_STEP_CHANGE = 0.1;
	//}

		
	if(dP != 0) {
		//pc.printf("%7.3f    %7.3f\n\r", dP/dI, PWM_DUTY_STEP_CHANGE);
		if(dP > 0.5) {
			if(dI > 0) {
				PWM_DUTY += PWM_DUTY_STEP_CHANGE;
			} else {
				PWM_DUTY -= PWM_DUTY_STEP_CHANGE;
			}
		} else {
			if(dI > 0) {
				PWM_DUTY -= PWM_DUTY_STEP_CHANGE;
			} else {
				PWM_DUTY += PWM_DUTY_STEP_CHANGE;
			}
		}
		
		old_power = power;
		old_voltage = voltage;
		old_current = current;
		
	} else PWM_DUTY += PWM_DUTY_STEP_CHANGE;
	
	if(power < 0.5) PWM_DUTY -= 0.25;
	PWM_DUTY = clamp(PWM_DUTY, PWM_MIN, PWM_MAX);
}
*/

/*
void adjust_pwm(double voltage, double current) {
	//example from MATLAB's P&O algorithm
	double power;
	double dP, dV;
	//int pwm_temp;
	//pc.printf("[ PWM ]\n\r");
	
	//calculate power
	power = voltage * current;
	dP = power - old_power;
	dV = voltage - old_voltage;
	
	//if(power > max_power) max_power = power;
	
	if(dP != 0) {
	
	//if((dP > 0.25) || (dP < -0.25)) {
		if(dP > 0) {
			//power increases
			//pc.printf(">>>\n\r");
			if((dV > 0.25) || (dV < -0.25)) {
				if(dV > 0){
					//voltage increases
					//pc.printf("Voltage > old_Voltage | PWM++\n\r");
					PWM_DUTY += PWM_DUTY_STEP_CHANGE * 2;
				} else {
					//pc.printf("Voltage < old_Voltage | PWM--\n\r");
					PWM_DUTY -= PWM_DUTY_STEP_CHANGE;
				}
			}
		} else {
			//pc.printf("<<<\n\r");
			if((dV > 0.25) || (dV < -0.25)) {
				if(dV > 0) {
					//pc.printf("Voltage > old_Voltage | PMW--\n\r");
					PWM_DUTY -= PWM_DUTY_STEP_CHANGE * 2;
				} else {
					//pc.printf("Voltage < old_Voltage | PWM++\n\r");
					PWM_DUTY += PWM_DUTY_STEP_CHANGE;
				}
			}
		}
	} else {
		PWM_DUTY += PWM_DUTY_STEP_CHANGE * 2;
	}
	
	if(power < 0.5) PWM_DUTY -= 0.3;
	
	PWM_DUTY = clamp(PWM_DUTY, PWM_MIN, PWM_MAX);
	
	old_voltage = voltage;
	old_current = current;
	old_power = power;
}
*/


// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
