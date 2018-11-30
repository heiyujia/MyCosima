//=============================================================================
//=============================== isr.c =======================================
// Created by: Institut ELSYS - Technische Hochschule Nuernberg, 2017

//====================== INCLUDES AND DEFINES =================================
//=============================================================================
#include "DSP28x_Project.h"
#include "../include/isr.h"

#include "../include/platform.h"
#include "../include/application.h"
#include "../include/my_posspeed.h"
#include "../include/errordetection.h"

//Generated Code from Simulink model

//JAVASCOPE
#include "../include/javascope.h"
//====================== END INCLUDES AND DEFINES =============================
//=============================================================================

//============================= VARIABLES =====================================
//=============================================================================
extern DS_Data Global_Data;

//Communication structures for inter-processor communication
extern m3_to_c28_shared_struct m3_to_c28, m3_to_c28_shadow;
extern c28_to_m3_shared_struct c28_to_m3;
//Communication flag for Inter Processor Communication
extern Uint16 g_flag_new_m3_to_c28_ipc;

extern POSSPEED qep_posspeed;

// counters
int var_mess = 0;
int saegezahn_loop = 0;
Int32 takt_1ms = 0;
static Int16 zeit_100us = 0;

// ISR timing measurement variables
Int32 time_measurement = 0;
Int32 ISRStartTime = 0, ISRStopTime = 0;

// testvariables JavaScope
float test_js_sinewave1 = 0.0, test_javaScope_freqHz = 1.0, argument = 0.0,
	  sin1amp = 100.0, sin2amp = 80.0, sawfak = 0.2;
float test_js_sinewave2 = 0.0, test_js_sawtooth1 = 0.0, test_js_sawtooth2 = 0.0;
Uint32 cnt_200us = 0, chscope = 2;
extern Int16 Frequenzvorgabe;

#pragma CODE_SECTION(adc1_isr, "ramfuncs");
//=========================== END VARIABLES ===================================
//=============================================================================

interrupt void adc1_isr() {

	//GpioDataRegs.GPASET.bit.GPIO6 = 1; // set Pin for timing measuring
	ISRStartTime = CpuTimer0.RegsAddr->TIM.all;

	if (Global_Data.cw.initSystemDone != 1) {
	//===============================Start: Allow to interrupt the adc1_isr-interrupt by the ECAP-interrupt (for the Hall-Sensor)=======================================================//
	// Hierdurch wird nur den ECAP1, ECAP2, ECAP3 - Interrupts erlaub die nachfolgende ISR zu unterbrechen, alle anderen Interruts können die ISR nicht unterbrechen. Aber Hall-ISR ist ja sehr kurz!
	DINT;
	//SW: Clear all interrupts and initialize PIE vector table
	IER |= M_INT4; //SW: Zeile 4 der Interrupt-Vector-Tabelle freigeben (Es werden hierbei alle Spalten der Tabelle freigegeben!)
	IER &= M_INT4; //SW: Zeile 4 der Interrupt-Vector-Tabelle freigeben (Es werden hierbei alle Spalten der Tabelle freigegeben!)
	PieCtrlRegs.PIEACK.all = 0xFFFF; //SW: Freigabe übernehmen (Acknowledge)
	asm("	NOP");
	EINT;
	//SW: Enable Global interrupt INTM
	//===============================Start: Allow to interrupt the adc1_isr-interrupt by the ECAP-interrupt (for the Hall-Sensor)=======================================================//

#if (PLATFORM  == PLAT_OHMrichter)
		if (var_mess < 1000 && Global_Data.cw.ADCoffsetCalculationDone != 1) {

			Global_Data.rawADC.I_U_int				= ADC_I_U;
			Global_Data.rawADC.I_V_int				= ADC_I_V;
			Global_Data.rawADC.I_W_int				= ADC_I_W;
			Global_Data.rawADC.U_U_int				= ADC_U_U;
			Global_Data.rawADC.U_V_int				= ADC_U_V;
			Global_Data.rawADC.U_W_int				= ADC_U_W;
			Global_Data.rawADC.AnalogHall_H1_int 	= ADC_ANA_HALL_1;
			Global_Data.rawADC.AnalogHall_H2_int 	= ADC_ANA_HALL_2;
			Global_Data.rawADC.AnalogHall_H3_int 	= ADC_ANA_HALL_3;
			// add up 1000 ADC values
			Global_Data.rawADC.i_U_offset 			+= (float32) Global_Data.rawADC.I_U_int;
			Global_Data.rawADC.i_V_offset 			+= (float32) Global_Data.rawADC.I_V_int;
			Global_Data.rawADC.i_W_offset 			+= (float32) Global_Data.rawADC.I_W_int;
			Global_Data.rawADC.u_U_offset 			+= (float32) Global_Data.rawADC.U_U_int;
			Global_Data.rawADC.u_V_offset 			+= (float32) Global_Data.rawADC.U_V_int;
			Global_Data.rawADC.u_W_offset 			+= (float32) Global_Data.rawADC.U_W_int;
			Global_Data.rawADC.AnalogHall_H1_offset	+= (float32) Global_Data.rawADC.AnalogHall_H1_int;
			Global_Data.rawADC.AnalogHall_H2_offset	+= (float32) Global_Data.rawADC.AnalogHall_H2_int;
			Global_Data.rawADC.AnalogHall_H3_offset	+= (float32) Global_Data.rawADC.AnalogHall_H3_int;


			var_mess++;

		} else { //if (var_mess == 1000) -> finished adding ADC values
			// calculate mean ADC offsets
			Global_Data.rawADC.i_U_offset			= Global_Data.rawADC.i_U_offset / (float32) var_mess;
			Global_Data.rawADC.i_V_offset			= Global_Data.rawADC.i_V_offset / (float32) var_mess;
			Global_Data.rawADC.i_W_offset			= Global_Data.rawADC.i_W_offset / (float32) var_mess;
			Global_Data.rawADC.u_U_offset			= Global_Data.rawADC.u_U_offset / (float32) var_mess;
			Global_Data.rawADC.u_V_offset			= Global_Data.rawADC.u_V_offset / (float32) var_mess;
			Global_Data.rawADC.u_W_offset			= Global_Data.rawADC.u_W_offset / (float32) var_mess;
			Global_Data.rawADC.AnalogHall_H1_offset	= Global_Data.rawADC.AnalogHall_H1_offset / (float32) var_mess;
			Global_Data.rawADC.AnalogHall_H2_offset	= Global_Data.rawADC.AnalogHall_H2_offset / (float32) var_mess;
			Global_Data.rawADC.AnalogHall_H3_offset	= Global_Data.rawADC.AnalogHall_H3_offset / (float32) var_mess;

			Global_Data.cw.ADCoffsetCalculationDone = flagEnabled;
			Global_Data.cw.initSystemDone = flagEnabled;
		}
#endif // (PLATFORM == OHMrichter)

#if (PLATFORM == PLAT_STEUER)
		if (var_mess < 1000 && Global_Data.cw.ADCoffsetCalculationDone != 1) {

			Global_Data.rawADC.I_L1_int				= ADC_I_L1;
			Global_Data.rawADC.I_L2_int				= ADC_I_L2;
			Global_Data.rawADC.I_L3_int				= ADC_I_L3;
			Global_Data.rawADC.U_L1_int				= ADC_U_L1;
			Global_Data.rawADC.U_L2_int				= ADC_U_L2;
			Global_Data.rawADC.U_L3_int				= ADC_U_L3;
			Global_Data.rawADC.I_U_int				= ADC_I_U;
			Global_Data.rawADC.I_V_int				= ADC_I_V;
			Global_Data.rawADC.I_W_int				= ADC_I_W;
			Global_Data.rawADC.U_U_int				= ADC_U_U;
			Global_Data.rawADC.U_V_int				= ADC_U_V;
			Global_Data.rawADC.U_W_int				= ADC_U_W;
			Global_Data.rawADC.AnalogHall_H1_int 	= ADC_ANA_HALL_1;
			Global_Data.rawADC.AnalogHall_H2_int 	= ADC_ANA_HALL_2;
			Global_Data.rawADC.AnalogHall_H3_int 	= ADC_ANA_HALL_3;
			// add up 1000 ADC values
			Global_Data.rawADC.i_L1_offset 			+= (float32) Global_Data.rawADC.I_L1_int;
			Global_Data.rawADC.i_L2_offset 			+= (float32) Global_Data.rawADC.I_L2_int;
			Global_Data.rawADC.i_L3_offset 			+= (float32) Global_Data.rawADC.I_L3_int;
			Global_Data.rawADC.u_L1_offset 			+= (float32) Global_Data.rawADC.U_L1_int;
			Global_Data.rawADC.u_L2_offset 			+= (float32) Global_Data.rawADC.U_L2_int;
			Global_Data.rawADC.u_L3_offset 			+= (float32) Global_Data.rawADC.U_L3_int;
			Global_Data.rawADC.i_U_offset 			+= (float32) Global_Data.rawADC.I_U_int;
			Global_Data.rawADC.i_V_offset 			+= (float32) Global_Data.rawADC.I_V_int;
			Global_Data.rawADC.i_W_offset 			+= (float32) Global_Data.rawADC.I_W_int;
			Global_Data.rawADC.u_U_offset 			+= (float32) Global_Data.rawADC.U_U_int;
			Global_Data.rawADC.u_V_offset 			+= (float32) Global_Data.rawADC.U_V_int;
			Global_Data.rawADC.u_W_offset 			+= (float32) Global_Data.rawADC.U_W_int;
			Global_Data.rawADC.AnalogHall_H1_offset	+= (float32) Global_Data.rawADC.AnalogHall_H1_int;
			Global_Data.rawADC.AnalogHall_H2_offset	+= (float32) Global_Data.rawADC.AnalogHall_H2_int;
			Global_Data.rawADC.AnalogHall_H3_offset	+= (float32) Global_Data.rawADC.AnalogHall_H3_int;


			var_mess++;

		} else { //if (var_mess == 1000) -> finished adding ADC values
			// calculate mean ADC offsets
			Global_Data.rawADC.i_L1_offset			= Global_Data.rawADC.i_L1_offset / (float32) var_mess;
			Global_Data.rawADC.i_L2_offset			= Global_Data.rawADC.i_L2_offset / (float32) var_mess;
			Global_Data.rawADC.i_L3_offset			= Global_Data.rawADC.i_L3_offset / (float32) var_mess;
			Global_Data.rawADC.u_L1_offset			= Global_Data.rawADC.u_L1_offset / (float32) var_mess;
			Global_Data.rawADC.u_L2_offset			= Global_Data.rawADC.u_L2_offset / (float32) var_mess;
			Global_Data.rawADC.u_L3_offset			= Global_Data.rawADC.u_L3_offset / (float32) var_mess;
			Global_Data.rawADC.i_U_offset			= Global_Data.rawADC.i_U_offset / (float32) var_mess;
			Global_Data.rawADC.i_V_offset			= Global_Data.rawADC.i_V_offset / (float32) var_mess;
			Global_Data.rawADC.i_W_offset			= Global_Data.rawADC.i_W_offset / (float32) var_mess;
			Global_Data.rawADC.u_U_offset			= Global_Data.rawADC.u_U_offset / (float32) var_mess;
			Global_Data.rawADC.u_V_offset			= Global_Data.rawADC.u_V_offset / (float32) var_mess;
			Global_Data.rawADC.u_W_offset			= Global_Data.rawADC.u_W_offset / (float32) var_mess;
			Global_Data.rawADC.AnalogHall_H1_offset	= Global_Data.rawADC.AnalogHall_H1_offset / (float32) var_mess;
			Global_Data.rawADC.AnalogHall_H2_offset	= Global_Data.rawADC.AnalogHall_H2_offset / (float32) var_mess;
			Global_Data.rawADC.AnalogHall_H3_offset	= Global_Data.rawADC.AnalogHall_H3_offset / (float32) var_mess;

			Global_Data.cw.ADCoffsetCalculationDone = flagEnabled;
			Global_Data.cw.initSystemDone = flagEnabled;
		}
#endif // (PLATFORM == PLAT_STEUER)


#if (PLATFORM == PLAT_CUSTOM)

#endif // (PLATFORM == PLAT_CUSTOM)


	} else { // if (Global_Data.cw.initSystemDone == 1 )
		if (Global_Data.cw.initApplicationDone != 1) {
			asm(" nop");
			// ### INIT APPLICATION ############################################
			// #################################################################

			/* Pack model data into RTM */


			/* Initialize model */


			// ################################################################
			// ################################################################
			Global_Data.cw.initApplicationDone = flagEnabled;
		} //END of (Global_Data.cw.initApplicationDone != 1)


		// CYCLIC PART OF ISR
#if (PLATFORM == PLAT_OHMrichter)
		// get ADC values
		Global_Data.rawADC.I_U_int 				= ADC_I_U;
		Global_Data.rawADC.I_V_int 				= ADC_I_V;
		Global_Data.rawADC.I_W_int 				= ADC_I_W;
		Global_Data.rawADC.U_ZK_int				= ADC_U_ZK;
		Global_Data.rawADC.U_U_int 				= ADC_U_U;
		Global_Data.rawADC.U_V_int 				= ADC_U_V;
		Global_Data.rawADC.U_W_int 				= ADC_U_W;
		Global_Data.rawADC.AnalogHall_H1_int 	= ADC_ANA_HALL_1;
		Global_Data.rawADC.AnalogHall_H2_int 	= ADC_ANA_HALL_2;
		Global_Data.rawADC.AnalogHall_H3_int 	= ADC_ANA_HALL_3;
		// calculate physical values from ADC values
		Global_Data.av.I_U 	= ((float32) ((Global_Data.rawADC.I_U_int - Global_Data.rawADC.i_U_offset) * Global_Data.prp.ADCConversionFactorIPH));	//[A] mit 2^12/2 = 2048
		Global_Data.av.I_V	= ((float32) ((Global_Data.rawADC.I_V_int - Global_Data.rawADC.i_V_offset) * Global_Data.prp.ADCConversionFactorIPH));	//[A] mit 2^12/2 = 2048
		Global_Data.av.I_W	= ((float32) ((Global_Data.rawADC.I_W_int - Global_Data.rawADC.i_W_offset) * Global_Data.prp.ADCConversionFactorIPH));	//[A] mit 2^12/2 = 2048
		Global_Data.av.U_ZK	= ((float32) (Global_Data.rawADC.U_ZK_int) * Global_Data.prp.ADCConversionFactorVPH); //[V] mit +-1% Spannungsteiler R2 = 95,3 kOhm R5 = 4,99 kOhm auf Leistungsplatine und 0,1% Spannungsteiler R502 = 3,5 kOhm R5503 = 6,8 kOhm -> Umax = (3.3*((3.5kOhm+6.8kOhm)/6.8kOhm)*((95kOhm+4.99kOhm)/4.99kOhm)) = 146.7 V -> Umax/2^12 = 0.03581786
		Global_Data.av.U_U	= ((float32) (Global_Data.rawADC.U_U_int - Global_Data.rawADC.u_U_offset) * Global_Data.prp.ADCConversionFactorVPH);	//[V] mit +-1% Spannungsteiler R17 = 95,3 kOhm R18 = 4,99 kOhm auf Leistungsplatine und 0,1% Spannungsteiler R524 = 3,5 kOhm R525 = 6,8 kOhm -> Umax = (3.3*((3.5kOhm+6.8kOhm)/6.8kOhm)*((95kOhm+4.99kOhm)/4.99kOhm)) = 146.7 V -> Umax/2^12 = 0.03581786
		Global_Data.av.U_V	= ((float32) (Global_Data.rawADC.U_V_int - Global_Data.rawADC.u_V_offset) * Global_Data.prp.ADCConversionFactorVPH);	//[V] mit +-1% Spannungsteiler R19 = 95,3 kOhm R20 = 4,99 kOhm auf Leistungsplatine und 0,1% Spannungsteiler R514 = 3,5 kOhm R515 = 6,8 kOhm -> Umax = (3.3*((3.5kOhm+6.8kOhm)/6.8kOhm)*((95kOhm+4.99kOhm)/4.99kOhm)) = 146.7 V -> Umax/2^12 = 0.03581786
		Global_Data.av.U_W	= ((float32) (Global_Data.rawADC.U_W_int - Global_Data.rawADC.u_W_offset) * Global_Data.prp.ADCConversionFactorVPH);	//[V] mit +-1% Spannungsteiler R21 = 95,3 kOhm R22 = 4,99 kOhm auf Leistungsplatine und 0,1% Spannungsteiler R504 = 3,5 kOhm R505 = 6,8 kOhm -> Umax = (3.3*((3.5kOhm+6.8kOhm)/6.8kOhm)*((95kOhm+4.99kOhm)/4.99kOhm)) = 146.7 V -> Umax/2^12 = 0.03581786
		Global_Data.av.AnalogHallState.H1 = ((float32) (Global_Data.rawADC.AnalogHall_H1_int) * Global_Data.prp.ADCConversionFactorANALOGHALL);//[V] mit 0,1% Spannungsteiler R526 = 3,5 kOhm R527 = 6,8 kOhm -> Umax = (3.3*((3.5kOhm+6.8kOhm)/6.8kOhm)) = 4.9985 V -> Umax/2^12 = 0.00122034
		Global_Data.av.AnalogHallState.H2 = ((float32) (Global_Data.rawADC.AnalogHall_H2_int) * Global_Data.prp.ADCConversionFactorANALOGHALL);//[V] mit 0,1% Spannungsteiler R516 = 3,5 kOhm R517 = 6,8 kOhm -> Umax = (3.3*((3.5kOhm+6.8kOhm)/6.8kOhm)) = 4.9985 V -> Umax/2^12 = 0.00122034
		Global_Data.av.AnalogHallState.H3 = ((float32) (Global_Data.rawADC.AnalogHall_H3_int) * Global_Data.prp.ADCConversionFactorANALOGHALL);//[V] mit 0,1% Spannungsteiler R506 = 3,5 kOhm R507 = 6,8 kOhm -> Umax = (3.3*((3.5kOhm+6.8kOhm)/6.8kOhm)) = 4.9985 V -> Umax/2^12 = 0.00122034
		// get incremental encoder values
		qep_posspeed.calc(&qep_posspeed);
		Global_Data.av.mechanicalRotorSpeed = qep_posspeed.SpeedRpm_pr;
		Global_Data.av.theta_mech = qep_posspeed.theta_mech;
		//Cut 2PI
		Global_Data.av.theta_mech = (Global_Data.av.theta_mech * (2 * PI)) * 0.0000305185; // -> 0.0000305185 = 1/32767;
#endif // (PLATFORM == PLAT_OHMrichter)


#if (PLATFORM == PLAT_STEUER)
		// get ADC values
		Global_Data.rawADC.I_L1_int 				= ADC_I_L1;
		Global_Data.rawADC.I_L2_int 				= ADC_I_L2;
		Global_Data.rawADC.I_L3_int 				= ADC_I_L3;
		Global_Data.rawADC.U_L1_int 				= ADC_U_L1;
		Global_Data.rawADC.U_L2_int 				= ADC_U_L2;
		Global_Data.rawADC.U_L3_int 				= ADC_U_L3;
		Global_Data.rawADC.I_U_int 					= ADC_I_U;
		Global_Data.rawADC.I_V_int 					= ADC_I_V;
		Global_Data.rawADC.I_W_int 					= ADC_I_W;
		Global_Data.rawADC.U_ZK_int 				= ADC_U_ZK;
		Global_Data.rawADC.U_U_int 					= ADC_U_U;
		Global_Data.rawADC.U_V_int 					= ADC_U_V;
		Global_Data.rawADC.U_W_int 					= ADC_U_W;
		Global_Data.rawADC.AnalogHall_H1_int 		= ADC_ANA_HALL_1;
		Global_Data.rawADC.AnalogHall_H2_int 		= ADC_ANA_HALL_2;
		Global_Data.rawADC.AnalogHall_H3_int 		= ADC_ANA_HALL_3;
		// calculate physical values from ADC values
		Global_Data.av.I_L1 = ((float32) ((Global_Data.rawADC.I_L1_int - Global_Data.rawADC.i_L1_offset) * Global_Data.prp.ADCConversionFactorIPH_GRID));//[A] mit 2^12/2 = 2048
		Global_Data.av.I_L2 = ((float32) ((Global_Data.rawADC.I_L2_int - Global_Data.rawADC.i_L2_offset) * Global_Data.prp.ADCConversionFactorIPH_GRID));//[A] mit 2^12/2 = 2048
		Global_Data.av.I_L3 = ((float32) ((Global_Data.rawADC.I_L3_int - Global_Data.rawADC.i_L3_offset) * Global_Data.prp.ADCConversionFactorIPH_GRID));//[A] mit 2^12/2 = 2048
		Global_Data.av.U_L1 = ((float32) ((Global_Data.rawADC.U_L1_int - Global_Data.rawADC.u_L1_offset) * Global_Data.prp.ADCConversionFactorVPH_GRID));//[A] mit 2^12/2 = 2048
		Global_Data.av.U_L2 = ((float32) ((Global_Data.rawADC.U_L2_int - Global_Data.rawADC.u_L2_offset) * Global_Data.prp.ADCConversionFactorVPH_GRID));//[A] mit 2^12/2 = 2048
		Global_Data.av.U_L3 = ((float32) ((Global_Data.rawADC.U_L3_int - Global_Data.rawADC.u_L3_offset) * Global_Data.prp.ADCConversionFactorVPH_GRID));//[A] mit 2^12/2 = 2048
		Global_Data.av.I_U = ((float32) ((Global_Data.rawADC.I_U_int - Global_Data.rawADC.i_U_offset) * Global_Data.prp.ADCConversionFactorIPH_MOTOR));//[A] mit 2^12/2 = 2048
		Global_Data.av.I_V = ((float32) ((Global_Data.rawADC.I_V_int - Global_Data.rawADC.i_V_offset) * Global_Data.prp.ADCConversionFactorIPH_MOTOR));//[A] mit 2^12/2 = 2048
		Global_Data.av.I_W = ((float32) ((Global_Data.rawADC.I_W_int - Global_Data.rawADC.i_W_offset) * Global_Data.prp.ADCConversionFactorIPH_MOTOR));//[A] mit 2^12/2 = 2048
		Global_Data.av.U_ZK = ((float32) (Global_Data.rawADC.U_ZK_int) * Global_Data.prp.ADCConversionFactorVDC);//[V] mit +-1% Spannungsteiler R2 = 95,3 kOhm R5 = 4,99 kOhm auf Leistungsplatine und 0,1% Spannungsteiler R502 = 3,5 kOhm R5503 = 6,8 kOhm -> Umax = (3.3*((3.5kOhm+6.8kOhm)/6.8kOhm)*((95kOhm+4.99kOhm)/4.99kOhm)) = 146.7 V -> Umax/2^12 = 0.03581786
		Global_Data.av.U_U = ((float32) (Global_Data.rawADC.U_U_int - Global_Data.rawADC.u_U_offset) * Global_Data.prp.ADCConversionFactorVPH_MOTOR);//[V] mit +-1% Spannungsteiler R17 = 95,3 kOhm R18 = 4,99 kOhm auf Leistungsplatine und 0,1% Spannungsteiler R524 = 3,5 kOhm R525 = 6,8 kOhm -> Umax = (3.3*((3.5kOhm+6.8kOhm)/6.8kOhm)*((95kOhm+4.99kOhm)/4.99kOhm)) = 146.7 V -> Umax/2^12 = 0.03581786
		Global_Data.av.U_V = ((float32) (Global_Data.rawADC.U_V_int - Global_Data.rawADC.u_V_offset) * Global_Data.prp.ADCConversionFactorVPH_MOTOR);//[V] mit +-1% Spannungsteiler R19 = 95,3 kOhm R20 = 4,99 kOhm auf Leistungsplatine und 0,1% Spannungsteiler R514 = 3,5 kOhm R515 = 6,8 kOhm -> Umax = (3.3*((3.5kOhm+6.8kOhm)/6.8kOhm)*((95kOhm+4.99kOhm)/4.99kOhm)) = 146.7 V -> Umax/2^12 = 0.03581786
		Global_Data.av.U_W = ((float32) (Global_Data.rawADC.U_W_int - Global_Data.rawADC.u_W_offset) * Global_Data.prp.ADCConversionFactorVPH_MOTOR);//[V] mit +-1% Spannungsteiler R21 = 95,3 kOhm R22 = 4,99 kOhm auf Leistungsplatine und 0,1% Spannungsteiler R504 = 3,5 kOhm R505 = 6,8 kOhm -> Umax = (3.3*((3.5kOhm+6.8kOhm)/6.8kOhm)*((95kOhm+4.99kOhm)/4.99kOhm)) = 146.7 V -> Umax/2^12 = 0.03581786
		Global_Data.av.AnalogHallState.H1 = ((float32) (Global_Data.rawADC.AnalogHall_H1_int) * Global_Data.prp.ADCConversionFactorANALOGHALL);//[V] mit 0,1% Spannungsteiler R526 = 3,5 kOhm R527 = 6,8 kOhm -> Umax = (3.3*((3.5kOhm+6.8kOhm)/6.8kOhm)) = 4.9985 V -> Umax/2^12 = 0.00122034
		Global_Data.av.AnalogHallState.H2 = ((float32) (Global_Data.rawADC.AnalogHall_H2_int) * Global_Data.prp.ADCConversionFactorANALOGHALL);//[V] mit 0,1% Spannungsteiler R516 = 3,5 kOhm R517 = 6,8 kOhm -> Umax = (3.3*((3.5kOhm+6.8kOhm)/6.8kOhm)) = 4.9985 V -> Umax/2^12 = 0.00122034
		Global_Data.av.AnalogHallState.H3 = ((float32) (Global_Data.rawADC.AnalogHall_H3_int) * Global_Data.prp.ADCConversionFactorANALOGHALL);//[V] mit 0,1% Spannungsteiler R506 = 3,5 kOhm R507 = 6,8 kOhm -> Umax = (3.3*((3.5kOhm+6.8kOhm)/6.8kOhm)) = 4.9985 V -> Umax/2^12 = 0.00122034
#endif // (PLATFORM == PLAT_STEUER)

#if (PLATFORM == PLAT_CUSTOM)

#endif // (PLATFORM == PLAT_CUSTOM)

		//ERROR DETECTION
		errorDetection(&Global_Data);

		//CHECK SYSTEM ENABLE FLAG
		if (Global_Data.cw.enableSystem) {
			PWM_HB_ON(1); // enable PWM half bridges
			PWM_HB_ON(2);
			PWM_HB_ON(3);
		} else {
			PWM_HB_OFF(1); // disable PWM half bridges via software trip event
			PWM_HB_OFF(2);
			PWM_HB_OFF(3);
		}

		if (Global_Data.cw.initApplicationDone == 1 && Global_Data.cw.enableSystem) {

			// ### CYCLIC APPLICATION CALL ######################################################################################
			// ##################################################################################################################
			// check enable control flag
			if (Global_Data.cw.enableControl) {


					// Assign pins to PWM module
					EALLOW;
					GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1; 	// pwm functionality
					GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1; 	// pwm functionality
					GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1; 	// pwm functionality
					GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1; 	// pwm functionality
					GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1; 	// pwm functionality
					GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1; 	// pwm functionality
					EDIS;

					// Assign values to PWM compare registers
					EALLOW;
					//EPwm1Regs.CMPA.half.CMPA = PERIOD *;  // = PERIOD * normalized PWM output signal (0....1)
					//EPwm2Regs.CMPA.half.CMPA = PERIOD *;
					//EPwm3Regs.CMPA.half.CMPA = PERIOD *;
					EDIS;

			}

			// ### END OF CYCLIC APPLICATION CALL ###############################################################################
			// ##################################################################################################################
			// ##################################################################################################################

		} //End of "if(Global_Data.cw.initApplicationDone == 1 && Global_Data.cw.enableSystem)"
	} // End of "if(Global_Data.cw.initSystemDone != 1)"



	// ISR heartbeat check
	saegezahn_loop++;
	if (saegezahn_loop == 81) {
		saegezahn_loop = 0;
	}

	// timer control, data transfer from C28 to M3 every 100 µs (requires 10 kHz ISR frequency)
	zeit_100us++;
	if (zeit_100us == 10000) { // 1 sec. passed
		zeit_100us = 0;
		zeit_sekunden_inc();
	}
	if ((zeit_100us % 10) == 1) {	// 1 ms passed
		takt_1ms++;
		if (takt_1ms >= 2073600000) //Reset after 24 days
			takt_1ms = 0;
	}

	// JAVASCOPE
	// assign values to JavaScope
	argument += PI2 * ((float) Frequenzvorgabe) * 0.0001; // Frequenzvorgabe ueber JavaScope
	if (argument > PI2)
		argument -= PI2;

	test_js_sinewave1 = sin1amp * sin(argument);
	test_js_sinewave2 = sin2amp * sin(argument + (PI2 / 4.0));
	test_js_sawtooth1 = zeit_100us % 100;
	test_js_sawtooth2 = sawfak * ((float) (zeit_100us % 500));

	// Store every observable signal into the Pointer-Array.
	// With the JavaScope, 4 signals can be displayed simultaneously (data stream at 200us time intervals).
	// Changing between the observable signals is possible at runtime in the JavaScope.
	js_ptr_arr[JSO_SineWave1] = &test_js_sinewave1;
	js_ptr_arr[JSO_SineWave2] = &test_js_sinewave2;
	js_ptr_arr[JSO_Sawtooth1] = &test_js_sawtooth1;
	js_ptr_arr[JSO_Sawtooth2] = &test_js_sawtooth2;
	js_ptr_arr[JSO_ua] = &Global_Data.av.U_U;
	js_ptr_arr[JSO_ub] = &Global_Data.av.U_V;
	js_ptr_arr[JSO_uc] = &Global_Data.av.U_W;
	js_ptr_arr[JSO_ia] = &Global_Data.av.I_U;
	js_ptr_arr[JSO_ib] = &Global_Data.av.I_V;
	js_ptr_arr[JSO_ic] = &Global_Data.av.I_W;
	js_ptr_arr[JSO_id] = 0;
	js_ptr_arr[JSO_iq] = 0;
	js_ptr_arr[JSO_ud] = 0;
	js_ptr_arr[JSO_uq] = 0;
	js_ptr_arr[JSO_speed] = 0;

	// Store slow / not-time-critical signals into the SlowData-Array.
	// Will be transferred one after another (one every 1 ms).
	// The array may grow arbitrarily long, the refresh rate of the individual values decreases.
	js_slowDataArray[JSSD_INT_SecondsSinceSystemStart].i = get_zeit_sekunden();
	js_slowDataArray[JSSD_FLOAT_uSecPerIsr].f = ((float) time_measurement * 6.66666666666667e-03);
	js_slowDataArray[JSSD_FLOAT_Sine].f = 10.0 * sin(PI2 * 0.05 * ((float) zeit_100us * 0.0001));// 0.05 Hz => T=20sec
	js_slowDataArray[JSSD_FLOAT_FreqReadback].f = (float) Frequenzvorgabe;
	js_slowDataArray[JSSD_INT_Milliseconds].i = takt_1ms;
	js_slowDataArray[JSSD_FLOAT_ADCconvFactorReadback].f = 0;
	js_slowDataArray[JSSD_FLOAT_Rs].f = 			0;
	js_slowDataArray[JSSD_FLOAT_Ld].f = 0;
	js_slowDataArray[JSSD_FLOAT_Lq].f = 0;
	js_slowDataArray[JSSD_FLOAT_psiPM].f = 0;
	js_slowDataArray[JSSD_FLOAT_J].f = 0;
	js_slowDataArray[JSSD_INT_polePairs].i = 0;
	js_slowDataArray[JSSD_INT_activeState].i = 0;
	js_slowDataArray[JSSD_FLOAT_u_d].f = 0;
	js_slowDataArray[JSSD_FLOAT_u_q].f = 0;
	js_slowDataArray[JSSD_FLOAT_i_d].f = 0;
	js_slowDataArray[JSSD_FLOAT_i_q].f = 0;
	js_slowDataArray[JSSD_FLOAT_speed].f = 0;
	js_slowDataArray[JSSD_FLOAT_torque].f = 0;
	js_slowDataArray[JSSD_FLOAT_encoderOffset].f = 0;
	js_slowDataArray[JSSD_FLOAT_u_d_ref].f = 0;
	js_slowDataArray[JSSD_FLOAT_u_q_ref].f = 0;
	js_slowDataArray[JSSD_FLOAT_CustomValue1].f = get_zeit_sekunden();
	js_slowDataArray[JSSD_FLOAT_CustomValue2].f = get_zeit_sekunden() * 10;
	js_slowDataArray[JSSD_FLOAT_CustomValue3].f = get_zeit_sekunden() * 100;
	js_slowDataArray[JSSD_FLOAT_CustomValue4].f = get_zeit_sekunden() * 1000;
	js_slowDataArray[JSSD_FLOAT_CustomValue5].f = get_zeit_sekunden();
	js_slowDataArray[JSSD_FLOAT_CustomValue6].f = get_zeit_sekunden() * 0.1;
	js_slowDataArray[JSSD_FLOAT_CustomValue7].f = get_zeit_sekunden() * 0.01;
	js_slowDataArray[JSSD_FLOAT_CustomValue8].f = get_zeit_sekunden() * 0.001;

	if (chscope == 4)
		js_fetchData4CH();
	else if (chscope == 2)
		js_fetchData2CH();
	// End JavaScope

	ISRStopTime = CpuTimer0.RegsAddr->TIM.all;
	time_measurement = (Int32) (ISRStartTime - ISRStopTime);

	Adc1Regs.ADCINTFLGCLR.bit.ADCINT1 = 1; //Clear ADCINT1 flag reinitialize for next SOC
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // Acknowledge interrupt to PIE

	Adc2Regs.ADCINTFLGCLR.bit.ADCINT2 = 1; // Clear ADCINT2 flag reinitialize for next SOC
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // Acknowledge interrupt to PIE

	//GpioDataRegs.GPACLEAR.bit.GPIO6 = 1; // clear Pin for timing measuring

	return;
}


/*
 * Interrupt for JavaScope
 */
interrupt void mtoc_isr() {

	m3_to_c28_shadow.id = m3_to_c28.id;
	m3_to_c28_shadow.value = m3_to_c28.value;

	g_flag_new_m3_to_c28_ipc = 1;

// Acknowledge interrupt to PIE
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;
// Acknowledge interrupt to M3
	CtoMIpcRegs.MTOCIPCACK.all |= IPC_FLAG3;
	return;
}
