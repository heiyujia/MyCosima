/*
 * ipc_c28.c
 *
 *  Created on: 03.04.2013
 *      Author: Philipp Löhdefink
 */

#define SCOPE_OFFSET_bits 0x00
#define MOTORCONTROL_OFFSET_bits 1000


#include "DSP28x_Project.h"       // Device Headerfile and Examples Include File
#include <string.h>
#include "../include/ipc_c28.h"
#include "../main_c28.h"
#include "../include/javascope.h"

Int16 Frequenzvorgabe = 10;

Uint32 ADCconvFactor_Hbytes;
Uint32 ADCconvFactor_Lbytes;

extern Uint16 js_factor1, js_factor2, js_factor3, js_factor4;	// javascope
extern float sin1amp;

extern c28_to_m3_shared_struct c28_to_m3;

void ipc_mtoc_func(Uint16 msgId, Int16 value, DS_Data* data)
{
	if ( msgId != 0)
	{
		// ------------------- NETZ ---------------------
		if (msgId == 1+SCOPE_OFFSET_bits) // state++
		{
			// do something
		}
		else if (msgId == 2+SCOPE_OFFSET_bits) // stop
		{
			// do something
		}
		else if (msgId == 16+SCOPE_OFFSET_bits) // frequenz update
		{
			if (value > 50)
				Frequenzvorgabe = 50;
			else if(value < 0)
				Frequenzvorgabe = 0;
			else
				Frequenzvorgabe = value;
		}
		else if (msgId == 17+SCOPE_OFFSET_bits)
		{
			sin1amp = (float)value;
		}
		else if (msgId == 112+SCOPE_OFFSET_bits) // resette alle Fehler
		{
			// do something
		}
		else if (msgId == 200+SCOPE_OFFSET_bits) // SEND_PREC_CH1_bits
		{
			js_factor1 = value;
		}
		else if (msgId == 201+SCOPE_OFFSET_bits) // SEND_PREC_CH2_bits
		{
			js_factor2 = value;
		}
		else if (msgId == 202+SCOPE_OFFSET_bits) // SEND_PREC_CH3_bits
		{
			js_factor3 = value;
		}
		else if (msgId == 203+SCOPE_OFFSET_bits) // SEND_PREC_CH4_bits
		{
			js_factor4 = value;
		}
		else if (msgId == 204+SCOPE_OFFSET_bits) // SELECT_DATA_CH1_bits
		{
			if ( value >= 0 && value < JSO_ENDMARKER )
			{
				js_ptr[0] = js_ptr_arr[value];
			}
		}
		else if (msgId == 205+SCOPE_OFFSET_bits) // SELECT_DATA_CH2_bits
		{
			if ( value >= 0 && value < JSO_ENDMARKER )
			{
				js_ptr[1] = js_ptr_arr[value];
			}
		}
		else if (msgId == 206+SCOPE_OFFSET_bits) // SELECT_DATA_CH3_bits
		{
			if ( value >= 0 && value < JSO_ENDMARKER )
			{
				js_ptr[2] = js_ptr_arr[value];
			}
		}
		else if (msgId == 207+SCOPE_OFFSET_bits) // SELECT_DATA_CH4_bits
		{
			if ( value >= 0 && value < JSO_ENDMARKER )
			{
				js_ptr[3] = js_ptr_arr[value];
			}
		}

#if (PLATFORM == PLAT_OHMrichter)
		//INVERTER_EN
		else if (msgId == 2 + MOTORCONTROL_OFFSET_bits) {
			data->cw.enableSystem = flagDisabled;
		}
		else if (msgId == 1 + MOTORCONTROL_OFFSET_bits) {
			data->cw.enableSystem = flagEnabled;
		}
		//CONTROL_EN
		else if (msgId == 4 + MOTORCONTROL_OFFSET_bits)
			data->cw.enableControl = flagDisabled;
			//PIR_enable = 0;
		else if (msgId == 3 + MOTORCONTROL_OFFSET_bits)
			data->cw.enableControl = flagEnabled;
			//PIR_enable = 1;

		//ERROR RESET
		else if (msgId == 41 + MOTORCONTROL_OFFSET_bits)
			data->appV.reset_error = 1;

		//ACCEPT
		else if (msgId == 5 + MOTORCONTROL_OFFSET_bits)
			data->appV.accept = 1;
			//accept = 1;
		//RESET
		else if (msgId == 6 + MOTORCONTROL_OFFSET_bits)
			data->appV.reset = 1;
			//reset = 1;

		//MOTOR_ID
		else if (msgId == 0x101 + MOTORCONTROL_OFFSET_bits)
			data->appV.MotorID = 1;
		else if (msgId == 0x102 + MOTORCONTROL_OFFSET_bits)
			data->appV.MotorID = 0;

		//IDENTLQ
		else if (msgId == 103 + MOTORCONTROL_OFFSET_bits)
			data->appV.identLq = 1;
		else if (msgId == 104 + MOTORCONTROL_OFFSET_bits)
			data->appV.identLq = 0;

		//I_D_SAMPLETIMEISR
		else if (msgId == 0x105 + MOTORCONTROL_OFFSET_bits)
			data->appV.sampleTimeISR = value * 0.000001;
		//I_D_POLEPAIRS
		else if (msgId == 0x106 + MOTORCONTROL_OFFSET_bits)
			data->appV.polePairs = value;
		//I_D_DUTYCYC
		else if (msgId == 0x107 + MOTORCONTROL_OFFSET_bits)
			data->appV.dutyCyc = value * 0.01;
		//I_D_NREFM
		else if (msgId == 0x108 + MOTORCONTROL_OFFSET_bits)
			data->appV.n_ref_measurement = value;
		//I_D_NREFFOC
		else if (msgId == 0x109 + MOTORCONTROL_OFFSET_bits)
			data->appV.n_ref = value;
		//I_D_TREF
		else if (msgId == 0x110 + MOTORCONTROL_OFFSET_bits)
			data->appV.Trq_ref = value * 0.001;
		//I_D_IDREF
		else if (msgId == 0x111 + MOTORCONTROL_OFFSET_bits)
			data->appV.i_d_ref = value * 0.001;
		//I_D_IQREF
		else if (msgId == 0x112 + MOTORCONTROL_OFFSET_bits)
			data->appV.i_q_ref = value * 0.001;

		//Ronline_ON
		else if (msgId == 0x190 + MOTORCONTROL_OFFSET_bits)
			data->appV.identR = 1;
		//Ronline_OFF
		else if (msgId == 0x191 + MOTORCONTROL_OFFSET_bits)
			data->appV.identR = 0;
		//identRAmp
		else if (msgId == 0x192 + MOTORCONTROL_OFFSET_bits)
			data->appV.identRAmp = value * 0.01;

		//AMM_ON
		else if (msgId == 0x201 + MOTORCONTROL_OFFSET_bits)//Automated Measuring Mode - Enter State
			data->appV.AMM_on = 1;
		//AMM_OFF
		else if (msgId == 0x202 + MOTORCONTROL_OFFSET_bits)
			data->appV.AMM_on = 0;
		//AMM_RUN_ON
		else if (msgId == 0x203 + MOTORCONTROL_OFFSET_bits)//Automated Measuring Mode - Run Automated Measuring
			data->appV.AMM_run = 1;
		//AMM_RUN_OFF
		else if (msgId == 0x204 + MOTORCONTROL_OFFSET_bits)//Automated Measuring Mode - Shut Down Automated Measuring
			data->appV.AMM_run = 0;
		//LOG_INTERVALS
		else if (msgId == 0x205 + MOTORCONTROL_OFFSET_bits)
			data->appV.LogIntervals = value * 0.001;
		//NUMBER_OF_MEASURES
		else if (msgId == 0x206 + MOTORCONTROL_OFFSET_bits)
			data->appV.NumberOfSamples = value;
		//SETTLING_TIME
		else if (msgId == 0x207 + MOTORCONTROL_OFFSET_bits)
			data->appV.settlingTime = value;
		//IDstart
		else if (msgId == 0x210 + MOTORCONTROL_OFFSET_bits)
			data->appV.IDstart = value * 0.001;
		//IDstop
		else if (msgId == 0x211 + MOTORCONTROL_OFFSET_bits)
			data->appV.IDstop = value * 0.001;
		//IDstepsize
		else if (msgId == 0x212 + MOTORCONTROL_OFFSET_bits)
			data->appV.IDstepsize = value * 0.001;
		//IQstart
		else if (msgId == 0x213 + MOTORCONTROL_OFFSET_bits)
			data->appV.IQstart = value * 0.001;
		//IQstop
		else if (msgId == 0x214 + MOTORCONTROL_OFFSET_bits)
			data->appV.IQstop = value * 0.001;
		//IQstepsize
		else if (msgId == 0x215 + MOTORCONTROL_OFFSET_bits)
			data->appV.IQstepsize = value * 0.001;

//SETUP MEASUREMENTS
		//ROTOR SENSOR
		else if (msgId == 0x300+MOTORCONTROL_OFFSET_bits)
			data->pC.rSS.rotorSensorType = value;
			//rotorSensorType = value;
		//analog hall
		//digital hall
		else if (msgId == 0x320+MOTORCONTROL_OFFSET_bits)
			data->pC.rSS.digHallSensorType = value;
		//hiperface/SSI
		//incremental encoder
		else if (msgId == 0x340+MOTORCONTROL_OFFSET_bits)
			data->pC.iEP.countsPerRevolution = value;
		else if(msgId == 0x341+MOTORCONTROL_OFFSET_bits)
			data->pC.iEP.maxSpeedRPM = value;
		else if (msgId == 0x342+MOTORCONTROL_OFFSET_bits) {
			if (data->appV.MotorID == 0)
				data->pC.iEP.encoderOffset = value * 0.001;
			data->pC.iEP.newEncoderValuesFlag = 1;
		}

		//ADC
		else if (msgId == 0x350+MOTORCONTROL_OFFSET_bits)
			ADCconvFactor_Hbytes = value;
		else if (msgId == 0x351+MOTORCONTROL_OFFSET_bits) {
			ADCconvFactor_Lbytes = value;
			data->prp.ADCConversionFactorIPH = ((ADCconvFactor_Hbytes << 16) | (ADCconvFactor_Lbytes & 0xFFFF))*1e-9;
		}
		else if (msgId == 0x352+MOTORCONTROL_OFFSET_bits)
			ADCconvFactor_Hbytes = value;
		else if (msgId == 0x353+MOTORCONTROL_OFFSET_bits) {
			ADCconvFactor_Lbytes = value;
			data->prp.ADCConversionFactorVPH = ((ADCconvFactor_Hbytes << 16) | (ADCconvFactor_Lbytes % 0xFFFF))*1e-9;
		}
		else if (msgId == 0x354+MOTORCONTROL_OFFSET_bits)
			ADCconvFactor_Hbytes = value;
		else if (msgId == 0x355+MOTORCONTROL_OFFSET_bits) {
			ADCconvFactor_Lbytes = value;
			data->prp.ADCConversionFactorIDC = ((ADCconvFactor_Hbytes << 16) | (ADCconvFactor_Lbytes % 0xFFFF))*1e-9;
		}
		else if (msgId == 0x356+MOTORCONTROL_OFFSET_bits)
			ADCconvFactor_Hbytes = value;
		else if (msgId == 0x357+MOTORCONTROL_OFFSET_bits) {
			ADCconvFactor_Lbytes = value;
			data->prp.ADCConversionFactorVDC = ((ADCconvFactor_Hbytes << 16) | (ADCconvFactor_Lbytes % 0xFFFF))*1e-9;
		}
		else if (msgId == 0x358+MOTORCONTROL_OFFSET_bits)
			ADCconvFactor_Hbytes = value;
		else if (msgId == 0x359+MOTORCONTROL_OFFSET_bits) {
			ADCconvFactor_Lbytes = value;
			data->prp.ADCConversionFactorTRQ = ((ADCconvFactor_Hbytes << 16) | (ADCconvFactor_Lbytes % 0xFFFF))*1e-9;
		}
		else if (msgId == 0x360+MOTORCONTROL_OFFSET_bits)
			ADCconvFactor_Hbytes = value;
		else if (msgId == 0x361+MOTORCONTROL_OFFSET_bits) {
			ADCconvFactor_Lbytes = value;
			data->prp.ADCConversionFactorTMP = ((ADCconvFactor_Hbytes << 16) | (ADCconvFactor_Lbytes % 0xFFFF))*1e-9;
		}
		else if (msgId == 0x362+MOTORCONTROL_OFFSET_bits)
			ADCconvFactor_Hbytes = value;
		else if (msgId == 0x363+MOTORCONTROL_OFFSET_bits) {
			ADCconvFactor_Lbytes = value;
			data->prp.ADCConversionFactorANALOGHALL = ((ADCconvFactor_Hbytes << 16) | (ADCconvFactor_Lbytes % 0xFFFF))*1e-9;
		}

		else if (msgId == 0x369+MOTORCONTROL_OFFSET_bits) {
			data->appV.ADCconvFactorReadRequest = value;
			switch (data->appV.ADCconvFactorReadRequest) {

			case (0x350+MOTORCONTROL_OFFSET_bits):
				data->appV.ADCconvFactorReadback = data->prp.ADCConversionFactorIPH;
				break;
			case (0x352+MOTORCONTROL_OFFSET_bits):
				data->appV.ADCconvFactorReadback = data->prp.ADCConversionFactorVPH;
				break;
			case (0x354+MOTORCONTROL_OFFSET_bits):
				data->appV.ADCconvFactorReadback = data->prp.ADCConversionFactorIDC;
				break;
			case (0x356+MOTORCONTROL_OFFSET_bits):
				data->appV.ADCconvFactorReadback = data->prp.ADCConversionFactorVDC;
				break;
			case (0x358+MOTORCONTROL_OFFSET_bits):
				data->appV.ADCconvFactorReadback = data->prp.ADCConversionFactorTRQ;
				break;
			case (0x360+MOTORCONTROL_OFFSET_bits):
				data->appV.ADCconvFactorReadback = data->prp.ADCConversionFactorTMP;
				break;
			case (0x362+MOTORCONTROL_OFFSET_bits):
				data->appV.ADCconvFactorReadback = data->prp.ADCConversionFactorANALOGHALL;
				break;
			default:
				data->appV.ADCconvFactorReadback = 0xBAD;
				break;
			}
		}


//MODULATION
		else if (msgId == 0x600+MOTORCONTROL_OFFSET_bits)
			data->appV.modulationMethod = value;

//CONTROL
		//general motor control
		else if (msgId == 0x400+MOTORCONTROL_OFFSET_bits)
			data->appV.controlType = value;
		else if (msgId == 0x401+MOTORCONTROL_OFFSET_bits)
			data->appV.controlReference = value;

		//FOC control
		else if (msgId == 0x402 + MOTORCONTROL_OFFSET_bits)
			data->appV.Kp_id = value * 0.001;
		else if (msgId == 0x403 + MOTORCONTROL_OFFSET_bits)
			data->appV.Tn_id = value * 0.0001;
		else if (msgId == 0x404 + MOTORCONTROL_OFFSET_bits)
			data->appV.Kp_iq = value * 0.001;
		else if (msgId == 0x405 + MOTORCONTROL_OFFSET_bits)
			data->appV.Tn_iq = value * 0.0001;
		else if (msgId == 0x406 + MOTORCONTROL_OFFSET_bits)
			data->appV.Kp_n = value * 0.001;
		else if (msgId == 0x407 + MOTORCONTROL_OFFSET_bits)
			data->appV.Tn_n = value * 0.0001;
		else if (msgId == 0x408 + MOTORCONTROL_OFFSET_bits)
			data->appV.FOCFeedForward = value;

		//add vibration to FOC current control
		else if (msgId == 0x500 + MOTORCONTROL_OFFSET_bits)
			data->appV.VibON = 1;
		else if (msgId == 0x501 + MOTORCONTROL_OFFSET_bits)
			data->appV.VibON = 0;
		else if (msgId == 0x502 + MOTORCONTROL_OFFSET_bits)
			data->appV.VibAmp = value * 0.01;
		else if (msgId == 0x503 + MOTORCONTROL_OFFSET_bits)
			data->appV.VibFreq = value;

		//online Rs measuring and temp calculation
		else if (msgId == 0x510 + MOTORCONTROL_OFFSET_bits)
			data->appV.R_s_ref = value * 0.001;
		else if (msgId == 0x511 + MOTORCONTROL_OFFSET_bits)
			data->appV.Temp_ref = value * 0.01;

//MODULATION
		else if (msgId == 0x600 + MOTORCONTROL_OFFSET_bits)
			data->appV.modulationMethod = value;


	}

	/* Status-Bits setzen - MH 20.09.2016 */
	/* Bit 0 - ui16_drv_enable */
	if (data->cw.enableSystem == flagEnabled) {
		c28_to_m3.status_ctom |= 1 << 0;
	} else {
		c28_to_m3.status_ctom &= ~(1 << 0);
	}
	/* Bit 1 - PIR_ENABLE */
	if (data->cw.enableControl == flagEnabled) {
		c28_to_m3.status_ctom |= 1 << 1;
	} else {
		c28_to_m3.status_ctom &= ~(1 << 1);
	}
	/* Bit 2 - IDENT_LQ */
	if (data->appV.identLq == 1) {
		c28_to_m3.status_ctom |= 1 << 2;
	} else {
		c28_to_m3.status_ctom &= ~(1 << 2);
	}
	/* Bit 3 - CURRENT_CONTROL */
	if (data->appV.controlType == 1) {
		c28_to_m3.status_ctom |= 1 << 3;
	} else {
		c28_to_m3.status_ctom &= ~(1 << 3);
	}
	/* Bit 4 - SPEED_CONTROL */
	if (data->appV.controlType == 2) {
		c28_to_m3.status_ctom |= 1 << 4;
	} else {
		c28_to_m3.status_ctom &= ~(1 << 4);
	}
	/* Bit 5 - ADD VIBRATION */
	if (data->appV.VibON == 1) {
		c28_to_m3.status_ctom |= 1 << 5;
	} else {
		c28_to_m3.status_ctom &= ~(1 << 5);
	}
	/* Bit 6 - IDorNOT */
	if (data->appV.MotorID == 1) {
		c28_to_m3.status_ctom |= 1 << 6;
	} else {
		c28_to_m3.status_ctom &= ~(1 << 6);
	}
	/* Bit 7 - identROnline */
	if (data->appV.identR == 1) {
		c28_to_m3.status_ctom |= 1 << 7;
	} else {
		c28_to_m3.status_ctom &= ~(1 << 7);
	}

	/* Bit 15 - FOCFeedForward */
	if (data->appV.FOCFeedForward == 1) {
		c28_to_m3.status_ctom |= 1 << 15;
	} else {
		c28_to_m3.status_ctom &= ~(1 << 15);
	}

#endif // (PLATFORM == OHMrichter)


}

#if (PLATFORM == PLAT_STEUER)
}
#endif // (PLATFORM == PLAT_STEUER)


#if (PLATFORM == PLAT_CUSTOM)

#endif // (PLATFORM == PLAT_CUSTOM)
