/*
 * errordetection.c
 *
 *  Created on: 05.10.2018
 *      Author: Michael Hoerner
 */
#include "DSP28x_Project.h"
#include "../main_c28.h"

#include "../include/platform.h"

#if (PLATFORM == PLAT_OHMrichter)
void errorDetection(DS_Data* data) {
	// ERROR DETECTION
	// overcurrent AC
	if ( ((data->av.I_U > SHORT_TERM_CURRENT_LIMIT || data->av.I_U < -SHORT_TERM_CURRENT_LIMIT) ||
		  (data->av.I_V > SHORT_TERM_CURRENT_LIMIT || data->av.I_V < -SHORT_TERM_CURRENT_LIMIT) ||
		  (data->av.I_W > SHORT_TERM_CURRENT_LIMIT || data->av.I_W < -SHORT_TERM_CURRENT_LIMIT)) && data->ew.overCurrentDetectedAC == valueFalse) {
		data->ew.currentErrorCounterAC++;
		if (data->ew.currentErrorCounterAC >= SHORT_TERM_CURRENT_DURATION) {
			PWM_HB_OFF(1);
			PWM_HB_OFF(2);
			PWM_HB_OFF(3);
			data->cw.enableSystem = flagDisabled;
			data->cw.enableControl = flagDisabled;
			data->ew.overCurrentDetectedAC = valueTrue;
		}
	} else {
		data->ew.currentErrorCounterAC = 0;
	}
	// overvoltage AC
	if ( ((data->av.U_U > SHORT_TERM_VOLTAGE_LIMIT || data->av.U_U < -SHORT_TERM_VOLTAGE_LIMIT) ||
		  (data->av.U_V > SHORT_TERM_VOLTAGE_LIMIT || data->av.U_V < -SHORT_TERM_VOLTAGE_LIMIT) ||
		  (data->av.U_W > SHORT_TERM_VOLTAGE_LIMIT || data->av.U_W < -SHORT_TERM_VOLTAGE_LIMIT)) && data->ew.overVoltageDetectedAC == valueFalse) {
		data->ew.voltageErrorCounterAC++;
		if (data->ew.voltageErrorCounterAC >= SHORT_TERM_VOLTAGE_DURATION) {
			PWM_HB_OFF(1);
			PWM_HB_OFF(2);
			PWM_HB_OFF(3);
			data->cw.enableSystem = flagDisabled;
			data->cw.enableControl = flagDisabled;
			data->ew.overVoltageDetectedAC = valueTrue;
		}
	} else {
		data->ew.voltageErrorCounterAC = 0;
	}
	//overvoltage DC
	if ( data->av.U_ZK > SHORT_TERM_DC_LINK_VOLTAGE_LIMIT && data->ew.overVoltageDetectedDClink == valueFalse ) {
		data->ew.voltageErrorCounterDC++;
		if (data->ew.voltageErrorCounterDC >= SHORT_TERM_DC_LINK_VOLTAGE_DURATION) {
			PWM_HB_OFF(1);
			PWM_HB_OFF(2);
			PWM_HB_OFF(3);
			data->cw.enableSystem = flagDisabled;
			data->cw.enableControl = flagDisabled;
			data->ew.overVoltageDetectedDClink = valueTrue;
		}
	} else {
		data->ew.voltageErrorCounterDC = 0;
	}
}
#endif



#if (PLATFORM == PLAT_STEUER)
void errorDetection(DS_Data* data) {
	// ERROR DETECTION
	// overcurrent AC
	if ( ((data->av.I_U > SHORT_TERM_CURRENT_LIMIT || data->av.I_U < -SHORT_TERM_CURRENT_LIMIT) ||
		  (data->av.I_V > SHORT_TERM_CURRENT_LIMIT || data->av.I_V < -SHORT_TERM_CURRENT_LIMIT) ||
		  (data->av.I_W > SHORT_TERM_CURRENT_LIMIT || data->av.I_W < -SHORT_TERM_CURRENT_LIMIT)) && data->ew.overCurrentDetectedAC == valueFalse) {
		data->ew.currentErrorCounterAC++;
		if (data->ew.currentErrorCounterAC >= SHORT_TERM_CURRENT_DURATION) {
			PWM_HB_OFF(1);
			PWM_HB_OFF(2);
			PWM_HB_OFF(3);
			data->cw.enableSystem = flagDisabled;
			data->cw.enableControl = flagDisabled;
			data->ew.overCurrentDetectedAC = valueTrue;
		}
	} else {
		data->ew.currentErrorCounterAC = 0;
	}
	// overvoltage AC
	if ( ((data->av.U_U > SHORT_TERM_VOLTAGE_LIMIT || data->av.U_U < -SHORT_TERM_VOLTAGE_LIMIT) ||
		  (data->av.U_V > SHORT_TERM_VOLTAGE_LIMIT || data->av.U_V < -SHORT_TERM_VOLTAGE_LIMIT) ||
		  (data->av.U_W > SHORT_TERM_VOLTAGE_LIMIT || data->av.U_W < -SHORT_TERM_VOLTAGE_LIMIT)) && data->ew.overVoltageDetectedAC == valueFalse) {
		data->ew.voltageErrorCounterAC++;
		if (data->ew.voltageErrorCounterAC >= SHORT_TERM_VOLTAGE_DURATION) {
			PWM_HB_OFF(1);
			PWM_HB_OFF(2);
			PWM_HB_OFF(3);
			data->cw.enableSystem = flagDisabled;
			data->cw.enableControl = flagDisabled;
			data->ew.overVoltageDetectedAC = valueTrue;
		}
	} else {
		data->ew.voltageErrorCounterAC = 0;
	}
	//overvoltage DC
	if ( data->av.U_ZK > SHORT_TERM_DC_LINK_VOLTAGE_LIMIT && data->ew.overVoltageDetectedDClink == valueFalse ) {
		data->ew.voltageErrorCounterDC++;
		if (data->ew.voltageErrorCounterDC >= SHORT_TERM_DC_LINK_VOLTAGE_DURATION) {
			PWM_HB_OFF(1);
			PWM_HB_OFF(2);
			PWM_HB_OFF(3);
			data->cw.enableSystem = flagDisabled;
			data->cw.enableControl = flagDisabled;
			data->ew.overVoltageDetectedDClink = valueTrue;
		}
	} else {
		data->ew.voltageErrorCounterDC = 0;
	}
}
#endif



#if (PLATFORM == PLAT_CUSTOM)

#endif

