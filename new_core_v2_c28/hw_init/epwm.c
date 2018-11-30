/*
 * epwm.c
 *
 *  Created on: 23.07.2012
 *      Author: Philipp Löhdefink
 *      Changed: Sebastian Wendel 11.07.2016
 */

#include "DSP28x_Project.h"
#include "../main_c28.h"
#include "../include/platform.h"


void PWM_HB_ON(int HalfBridgeNmb) {

	EALLOW;
	switch(HalfBridgeNmb) {

	case 1:
		EPwm1Regs.TZCLR.bit.OST = 1;
		break;
	case 2:
		EPwm2Regs.TZCLR.bit.OST = 1;
		break;
	case 3:
		EPwm3Regs.TZCLR.bit.OST = 1;
		break;
	case 4:
		EPwm4Regs.TZCLR.bit.OST = 1;
		break;
	case 5:
		EPwm5Regs.TZCLR.bit.OST = 1;
		break;
	case 6:
		EPwm6Regs.TZCLR.bit.OST = 1;
		break;
	default:
		break;
	}
	EDIS;
}

void PWM_HB_OFF(int HalfBridgeNmb) {

	EALLOW;
	switch(HalfBridgeNmb) {

	case 1:
		EPwm1Regs.TZFRC.bit.OST = 1;
		break;
	case 2:
		EPwm2Regs.TZFRC.bit.OST = 1;
		break;
	case 3:
		EPwm3Regs.TZFRC.bit.OST = 1;
		break;
	case 4:
		EPwm4Regs.TZFRC.bit.OST = 1;
		break;
	case 5:
		EPwm5Regs.TZFRC.bit.OST = 1;
		break;
	case 6:
		EPwm6Regs.TZFRC.bit.OST = 1;
		break;
	default:
		break;
	}
	EDIS;
}

#if (PLATFORM == PLAT_OHMrichter)
void InitEPWM_CH1(int period, int vala, int valb) {
	// Assumes ePWM1 clock is already enabled in InitSysCtrl();
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC 	= 0;					//Synchronisation des PWM clocks disablen
	// Time-base registers
	EPwm1Regs.TBPRD 					= period;           	// Set EPWM timer period
	EPwm1Regs.TBPHS.half.TBPHS 			= 0x0000;         		// Time-Base Phase Register
	EPwm1Regs.TBCTL.bit.CTRMODE 		= TB_COUNT_UPDOWN; 		// Count-up mode
	EPwm1Regs.TBCTL.bit.PHSEN 			= TB_DISABLE;      		// Disable phase loading
	EPwm1Regs.TBCTL.bit.PRDLD 			= TB_SHADOW;	    	// Set shadow
	EPwm1Regs.TBCTL.bit.SYNCOSEL 		= TB_CTR_ZERO;  		// gibt den Takt an
	// Setup shadow register load on ZERO
	EPwm1Regs.CMPCTL.bit.SHDWAMODE 		= CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE 		= CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE 		= CC_CTR_ZERO; 			// load on CTR=Zero
	EPwm1Regs.CMPCTL.bit.LOADBMODE 		= CC_CTR_ZERO; 			// load on CTR=Zero
	// Action Qualifier Control
	EPwm1Regs.AQCTLA.bit.CAU 			= AQ_CLEAR;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	EPwm1Regs.AQCTLA.bit.CAD 			= AQ_SET;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	// Dead Band Generator
	EPwm1Regs.DBCTL.bit.OUT_MODE 		= DB_FULL_ENABLE; 		//DBA_ENABLE;	//DB_DISABLE ; DBA_ENABLE ; DBB_ENABLE; DB_FULL_ENABLE//SW: 25.03.15 "DBA_Enable" nur PWM1A macht Deadband, PWM1B wollen wir nicht, damit keine Schaltverluste bei konstanter Spannungsrichtung
	EPwm1Regs.DBCTL.bit.IN_MODE 		= DBA_ALL;				//DBA_ALL ; DBB_RED_DBA_FED; DBA_RED_DBB_FED;DBB_ALL //SW: 25.03.15 "DBA_ALL" beide Deadtimes werden bei PWM1A abgezogen

	EPwm1Regs.DBCTL.bit.POLSEL 			= PWM_GATE_POLARITY;	//defined in platform.h
	EPwm1Regs.DBRED 					= RISING_EDGE_DELAY;	//defined in platform.h
	EPwm1Regs.DBFED 					= FALLING_EDGE_DELAY;	//defined in platform.h

	EPwm1Regs.TBCTR 					= 0x0000;           	// Time-Base Counter Register
	EPwm1Regs.TBCTL.bit.PHSDIR 			= TB_UP; 				//SW: Nicht auskommentiert wegen DemonstratorV2
	EPwm1Regs.TBCTL.bit.HSPCLKDIV 		= TB_DIV1;     			// TBCLK = SYSCLKOUT
	EPwm1Regs.TBCTL.bit.CLKDIV 			= TB_DIV1;
	// Set Compare values
	EPwm1Regs.CMPA.half.CMPA 			= vala;
	EPwm1Regs.CMPB 						= valb;
	// Chopper
	EPwm1Regs.PCCTL.bit.CHPEN 			= 0; 					// bypass the chopper
	// Trip Zone
	EPwm1Regs.TZCTL.bit.TZA 			= HIGH_SIDE_TRIP_EVENT;	// trip events defined in platform.h
	EPwm1Regs.TZCTL.bit.TZB 			= LOW_SIDE_TRIP_EVENT;

	//Event Trigger and Interrupt
	EPwm1Regs.ETSEL.bit.INTSEL 			= ET_CTR_PRD;			//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm1Regs.ETSEL.bit.INTEN 			= 1;
	EPwm1Regs.ETPS.bit.INTPRD 			= ET_1ST;				//ET_DISABLE ; ET_1ST ; ET_2ND ; ET_3RD
	EPwm1Regs.ETSEL.bit.SOCAEN 			= 1;
	EPwm1Regs.ETSEL.bit.SOCASEL 		= ET_CTR_ZERO;			//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm1Regs.ETSEL.bit.SOCBEN 			= 0;					//INT_ON ; INT_OFF
	EPwm1Regs.ETSEL.bit.SOCBSEL 		= ET_CTR_ZERO;			//ET_CTR_PRD;SW: 25.03.15 angepasst	//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm1Regs.ETPS.bit.SOCACNT 			= ET_1ST;				//ET_DISABLE ; ET_1ST ; ET_2ND ; ET_3RD
	EPwm1Regs.ETPS.bit.SOCAPRD 			= ET_1ST;
	EPwm1Regs.ETPS.bit.SOCBPRD 			= ET_1ST;
	EPwm1Regs.ETPS.bit.SOCBCNT 			= ET_1ST;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC 	= 1;					//Synchronisation des PWM clocks enablen
	EDIS;
}

void InitEPWM_CH2(int period, int vala, int valb)
{

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;						//Synchronisation des PWM clocks disablen

	// Time Base
	EPwm2Regs.TBPRD 					= period;				// Wert der Periode in TBCLKs
	EPwm2Regs.TBPHS.half.TBPHS 			= 0x0000;				// Wert auf den TBCTR gesetzt wir wenn TBCLKSYNK
	EPwm2Regs.TBCTL.bit.CTRMODE 		= TB_COUNT_UPDOWN;		// TB_COUNT_UP ; TB_COUNT_DOWN ; TB_COUNT_UPDOWN ; TB_FREEZE
	EPwm2Regs.TBCTL.bit.PHSEN 			= TB_DISABLE;			// TB_DISABLE ; TB_ENABLE
	EPwm2Regs.TBCTL.bit.PRDLD 			= TB_SHADOW;			// TB_SHADOW ; TB_IMMEDIATE
	EPwm2Regs.TBCTL.bit.SYNCOSEL 		= TB_SYNC_IN;			//TB_SYNC_DISABLE ; TB_CTR_CMPB ; TB_CTR_ZERO ; TB_SYNC_IN
	// Setup shadow register load on ZERO
	EPwm2Regs.CMPCTL.bit.SHDWAMODE 		= CC_SHADOW;			//CC_SHADOW ; CC_IMMEDIATE
	EPwm2Regs.CMPCTL.bit.SHDWBMODE 		= CC_SHADOW;			//CC_SHADOW ; CC_IMMEDIATE
	EPwm2Regs.CMPCTL.bit.LOADAMODE 		= CC_CTR_ZERO; 			//CC_CTR_ZERO ; CC_CTR_PRD ; CC_CTR_ZERO_PRD ; CC_LD_DISABLE
	EPwm2Regs.CMPCTL.bit.LOADBMODE 		= CC_CTR_ZERO;			//CC_CTR_ZERO ; CC_CTR_PRD ; CC_CTR_ZERO_PRD ; CC_LD_DISABLE
	// Action Qualifier Control
	EPwm2Regs.AQCTLA.bit.CAU 			= AQ_CLEAR;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	EPwm2Regs.AQCTLA.bit.CAD 			= AQ_SET;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	// Dead Band Generator
	EPwm2Regs.DBCTL.bit.OUT_MODE 		= DB_FULL_ENABLE; 		//DBA_ENABLE;	//DB_DISABLE ; DBA_ENABLE ; DBB_ENABLE; DB_FULL_ENABLE//SW: 25.03.15 "DBA_Enable" nur PWM2A macht Deadband, PWM1B wollen wir nicht, damit keine Schaltverluste bei konstanter Spannungsrichtung
	EPwm2Regs.DBCTL.bit.IN_MODE 		= DBA_ALL;				//DBA_ALL ; DBB_RED_DBA_FED; DBA_RED_DBB_FED;DBB_ALL //SW: 25.03.15 "DBA_ALL" beide Deadtimes werden bei PWM2A abgezogen

	EPwm2Regs.DBCTL.bit.POLSEL 			= PWM_GATE_POLARITY;	//DB_ACTV_HI ; DB_ACTV_LOC; DB_ACTV_HIC; DB_ACTV_LO
	EPwm2Regs.DBRED 					= RISING_EDGE_DELAY;	//defined in platform.h
	EPwm2Regs.DBFED 					= FALLING_EDGE_DELAY;	//defined in platform.h

	EPwm2Regs.TBCTR						= 0x0000;				// Anfangswert des TBCounters
	EPwm2Regs.TBCTL.bit.PHSDIR 			= TB_DOWN;				//TB_UP ; TB_DOWN
	EPwm2Regs.TBCTL.bit.HSPCLKDIV 		= TB_DIV1;				// TB_DIV1 ; TB_DIV2 ; TB_DIV4
	EPwm2Regs.TBCTL.bit.CLKDIV 			= TB_DIV1;				// TB_DIV1 ; TB_DIV2 ; TB_DIV4
	// Set Compare Values
	EPwm2Regs.CMPA.half.CMPA 			= vala;					//Einstellung des Initialwertes für Comperator a
	EPwm2Regs.CMPB 						= valb;
	// Chopper
	EPwm2Regs.PCCTL.bit.CHPEN 			= 0; 					// bypass the chopper
	// Trip Zone
	EPwm2Regs.TZCTL.bit.TZA 			= HIGH_SIDE_TRIP_EVENT;	//TZ_HIZ ; TZ_FORCE_HI ; TZ_FORCE_LO ; TZ_NO_CHANGE
	EPwm2Regs.TZCTL.bit.TZB 			= LOW_SIDE_TRIP_EVENT;	//TZ_HIZ ; TZ_FORCE_HI ; TZ_FORCE_LO ; TZ_NO_CHANGE
	//Event Trigger and Interrupt
	EPwm2Regs.ETSEL.bit.INTSEL 			= ET_CTR_ZERO;			//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm2Regs.ETSEL.bit.INTEN  			= 0;					//INT_ON ; INT_OFF
	EPwm2Regs.ETPS.bit.INTPRD  			= ET_1ST;				//ET_DISABLE ; ET_1ST ; ET_2ND ; ET_3RD
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC 	= 1;					//Synchronisation des PWM clocks enablen
	EDIS;
}

void InitEPWM_CH3(int period, int vala, int valb)
{

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;	//Synchronisation des PWM clocks disablen

	// Time Base
	EPwm3Regs.TBPRD 					= period;				// Wert der Periode in TBCLKs
	EPwm3Regs.TBPHS.half.TBPHS 			= 0x0000;				// Wert auf den TBCTR gesetzt wir wenn TBCLKSYNK
	EPwm3Regs.TBCTL.bit.CTRMODE 		= TB_COUNT_UPDOWN;		// TB_COUNT_UP ; TB_COUNT_DOWN ; TB_COUNT_UPDOWN ; TB_FREEZE
	EPwm3Regs.TBCTL.bit.PHSEN 			= TB_DISABLE;			// TB_DISABLE ; TB_ENABLE
	EPwm3Regs.TBCTL.bit.PRDLD 			= TB_SHADOW;			// TB_SHADOW ; TB_IMMEDIATE
	EPwm3Regs.TBCTL.bit.SYNCOSEL 		= TB_SYNC_IN;			//TB_SYNC_DISABLE ; TB_CTR_CMPB ; TB_CTR_ZERO ; TB_SYNC_IN
	// Setup shadow register load on ZERO
	EPwm3Regs.CMPCTL.bit.SHDWAMODE 		= CC_SHADOW;			//CC_SHADOW ; CC_IMMEDIATE
	EPwm3Regs.CMPCTL.bit.SHDWBMODE 		= CC_SHADOW;			//CC_SHADOW ; CC_IMMEDIATE
	EPwm3Regs.CMPCTL.bit.LOADAMODE 		= CC_CTR_ZERO; 			//CC_CTR_ZERO ; CC_CTR_PRD ; CC_CTR_ZERO_PRD ; CC_LD_DISABLE
	EPwm3Regs.CMPCTL.bit.LOADBMODE 		= CC_CTR_ZERO;			//CC_CTR_ZERO ; CC_CTR_PRD ; CC_CTR_ZERO_PRD ; CC_LD_DISABLE
	// Action Qualifier Control
	EPwm3Regs.AQCTLA.bit.CAU 			= AQ_CLEAR;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	EPwm3Regs.AQCTLA.bit.CAD 			= AQ_SET;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	// Dead Band Generator
	EPwm3Regs.DBCTL.bit.OUT_MODE 		= DB_FULL_ENABLE; 		//DBA_ENABLE;	//DB_DISABLE ; DBA_ENABLE ; DBB_ENABLE; DB_FULL_ENABLE//SW: 25.03.15 "DBA_Enable" nur PWM2A macht Deadband, PWM1B wollen wir nicht, damit keine Schaltverluste bei konstanter Spannungsrichtung
	EPwm3Regs.DBCTL.bit.IN_MODE 		= DBA_ALL;				//DBA_ALL ; DBB_RED_DBA_FED; DBA_RED_DBB_FED;DBB_ALL //SW: 25.03.15 "DBA_ALL" beide Deadtimes werden bei PWM2A abgezogen

	EPwm3Regs.DBCTL.bit.POLSEL 			= PWM_GATE_POLARITY;	//DB_ACTV_HI ; DB_ACTV_LOC; DB_ACTV_HIC; DB_ACTV_LO
	EPwm3Regs.DBRED 					= RISING_EDGE_DELAY;	//defined in platform.h
	EPwm3Regs.DBFED 					= FALLING_EDGE_DELAY;	//defined in platform.h

	EPwm3Regs.TBCTR						= 0x0000;				// Anfangswert des TBCounters
	EPwm3Regs.TBCTL.bit.PHSDIR 			= TB_DOWN;				//TB_UP ; TB_DOWN
	EPwm3Regs.TBCTL.bit.HSPCLKDIV 		= TB_DIV1;				// TB_DIV1 ; TB_DIV2 ; TB_DIV4
	EPwm3Regs.TBCTL.bit.CLKDIV 			= TB_DIV1;				// TB_DIV1 ; TB_DIV2 ; TB_DIV4
	// Set Compare Values
	EPwm3Regs.CMPA.half.CMPA 			= vala;					//Einstellung des Initialwertes für Comperator a
	EPwm3Regs.CMPB 						= valb;
	// Chopper
	EPwm3Regs.PCCTL.bit.CHPEN 			= 0; 					// bypass the chopper
	// Trip Zone
	EPwm3Regs.TZCTL.bit.TZA 			= HIGH_SIDE_TRIP_EVENT;	//TZ_HIZ ; TZ_FORCE_HI ; TZ_FORCE_LO ; TZ_NO_CHANGE
	EPwm3Regs.TZCTL.bit.TZB 			= LOW_SIDE_TRIP_EVENT;	//TZ_HIZ ; TZ_FORCE_HI ; TZ_FORCE_LO ; TZ_NO_CHANGE
	//Event Trigger and Interrupt
	EPwm3Regs.ETSEL.bit.INTSEL 			= ET_CTR_ZERO;			//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm3Regs.ETSEL.bit.INTEN  			= 0;					//INT_ON ; INT_OFF
	EPwm3Regs.ETPS.bit.INTPRD  			= ET_1ST;				//ET_DISABLE ; ET_1ST ; ET_2ND ; ET_3RD
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC 	= 1;					//Synchronisation des PWM clocks enablen
	EDIS;
}
#endif //(PLATFORM == PLAT_OHMrichter)



#if (PLATFORM == PLAT_STEUER)
void InitEPWM_CH1(int period, int vala, int valb) {
	// Assumes ePWM1 clock is already enabled in InitSysCtrl();
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC 	= 0;					//Synchronisation des PWM clocks disablen
	// Time-base registers
	EPwm1Regs.TBPRD 					= period;           	// Set EPWM timer period
	EPwm1Regs.TBPHS.half.TBPHS 			= 0x0000;         		// Time-Base Phase Register
	EPwm1Regs.TBCTL.bit.CTRMODE 		= TB_COUNT_UPDOWN; 		// Count-up mode
	EPwm1Regs.TBCTL.bit.PHSEN 			= TB_DISABLE;      		// Disable phase loading
	EPwm1Regs.TBCTL.bit.PRDLD 			= TB_SHADOW;	    	// Set shadow
	EPwm1Regs.TBCTL.bit.SYNCOSEL 		= TB_CTR_ZERO;  		// gibt den Takt an
	// Setup shadow register load on ZERO
	EPwm1Regs.CMPCTL.bit.SHDWAMODE 		= CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE 		= CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE 		= CC_CTR_ZERO; 			// load on CTR=Zero
	EPwm1Regs.CMPCTL.bit.LOADBMODE 		= CC_CTR_ZERO; 			// load on CTR=Zero
	// Action Qualifier Control
	EPwm1Regs.AQCTLA.bit.CAU 			= AQ_CLEAR;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	EPwm1Regs.AQCTLA.bit.CAD 			= AQ_SET;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	// Dead Band Generator
	EPwm1Regs.DBCTL.bit.OUT_MODE 		= DB_FULL_ENABLE; 		//DBA_ENABLE;	//DB_DISABLE ; DBA_ENABLE ; DBB_ENABLE; DB_FULL_ENABLE//SW: 25.03.15 "DBA_Enable" nur PWM1A macht Deadband, PWM1B wollen wir nicht, damit keine Schaltverluste bei konstanter Spannungsrichtung
	EPwm1Regs.DBCTL.bit.IN_MODE 		= DBA_ALL;				//DBA_ALL ; DBB_RED_DBA_FED; DBA_RED_DBB_FED;DBB_ALL //SW: 25.03.15 "DBA_ALL" beide Deadtimes werden bei PWM1A abgezogen

	EPwm1Regs.DBCTL.bit.POLSEL 			= PWM_GATE_POLARITY;	//defined in platform.h
	EPwm1Regs.DBRED 					= RISING_EDGE_DELAY;	//defined in platform.h
	EPwm1Regs.DBFED 					= FALLING_EDGE_DELAY;	//defined in platform.h

	EPwm1Regs.TBCTR 					= 0x0000;           	// Time-Base Counter Register
	EPwm1Regs.TBCTL.bit.PHSDIR 			= TB_UP; 				//SW: Nicht auskommentiert wegen DemonstratorV2
	EPwm1Regs.TBCTL.bit.HSPCLKDIV 		= TB_DIV1;     			// TBCLK = SYSCLKOUT
	EPwm1Regs.TBCTL.bit.CLKDIV 			= TB_DIV1;
	// Set Compare values
	EPwm1Regs.CMPA.half.CMPA 			= vala;
	EPwm1Regs.CMPB 						= valb;
	// Chopper
	EPwm1Regs.PCCTL.bit.CHPEN 			= 0; 					// bypass the chopper
	// Trip Zone
	EPwm1Regs.TZCTL.bit.TZA 			= HIGH_SIDE_TRIP_EVENT;	// trip events defined in platform.h
	EPwm1Regs.TZCTL.bit.TZB 			= LOW_SIDE_TRIP_EVENT;

	//Event Trigger and Interrupt
	EPwm1Regs.ETSEL.bit.INTSEL 			= ET_CTR_PRD;			//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm1Regs.ETSEL.bit.INTEN 			= 1;
	EPwm1Regs.ETPS.bit.INTPRD 			= ET_1ST;				//ET_DISABLE ; ET_1ST ; ET_2ND ; ET_3RD
	EPwm1Regs.ETSEL.bit.SOCAEN 			= 1;
	EPwm1Regs.ETSEL.bit.SOCASEL 		= ET_CTR_ZERO;			//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm1Regs.ETSEL.bit.SOCBEN 			= 0;					//INT_ON ; INT_OFF
	EPwm1Regs.ETSEL.bit.SOCBSEL 		= ET_CTR_ZERO;			//ET_CTR_PRD;SW: 25.03.15 angepasst	//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm1Regs.ETPS.bit.SOCACNT 			= ET_1ST;				//ET_DISABLE ; ET_1ST ; ET_2ND ; ET_3RD
	EPwm1Regs.ETPS.bit.SOCAPRD 			= ET_1ST;
	EPwm1Regs.ETPS.bit.SOCBPRD 			= ET_1ST;
	EPwm1Regs.ETPS.bit.SOCBCNT 			= ET_1ST;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC 	= 1;					//Synchronisation des PWM clocks enablen
	EDIS;
}

void InitEPWM_CH2(int period, int vala, int valb)
{

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;						//Synchronisation des PWM clocks disablen

	// Time Base
	EPwm2Regs.TBPRD 					= period;				// Wert der Periode in TBCLKs
	EPwm2Regs.TBPHS.half.TBPHS 			= 0x0000;				// Wert auf den TBCTR gesetzt wir wenn TBCLKSYNK
	EPwm2Regs.TBCTL.bit.CTRMODE 		= TB_COUNT_UPDOWN;		// TB_COUNT_UP ; TB_COUNT_DOWN ; TB_COUNT_UPDOWN ; TB_FREEZE
	EPwm2Regs.TBCTL.bit.PHSEN 			= TB_DISABLE;			// TB_DISABLE ; TB_ENABLE
	EPwm2Regs.TBCTL.bit.PRDLD 			= TB_SHADOW;			// TB_SHADOW ; TB_IMMEDIATE
	EPwm2Regs.TBCTL.bit.SYNCOSEL 		= TB_SYNC_IN;			//TB_SYNC_DISABLE ; TB_CTR_CMPB ; TB_CTR_ZERO ; TB_SYNC_IN
	// Setup shadow register load on ZERO
	EPwm2Regs.CMPCTL.bit.SHDWAMODE 		= CC_SHADOW;			//CC_SHADOW ; CC_IMMEDIATE
	EPwm2Regs.CMPCTL.bit.SHDWBMODE 		= CC_SHADOW;			//CC_SHADOW ; CC_IMMEDIATE
	EPwm2Regs.CMPCTL.bit.LOADAMODE 		= CC_CTR_ZERO; 			//CC_CTR_ZERO ; CC_CTR_PRD ; CC_CTR_ZERO_PRD ; CC_LD_DISABLE
	EPwm2Regs.CMPCTL.bit.LOADBMODE 		= CC_CTR_ZERO;			//CC_CTR_ZERO ; CC_CTR_PRD ; CC_CTR_ZERO_PRD ; CC_LD_DISABLE
	// Action Qualifier Control
	EPwm2Regs.AQCTLA.bit.CAU 			= AQ_CLEAR;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	EPwm2Regs.AQCTLA.bit.CAD 			= AQ_SET;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	// Dead Band Generator
	EPwm2Regs.DBCTL.bit.OUT_MODE 		= DB_FULL_ENABLE; 		//DBA_ENABLE;	//DB_DISABLE ; DBA_ENABLE ; DBB_ENABLE; DB_FULL_ENABLE//SW: 25.03.15 "DBA_Enable" nur PWM2A macht Deadband, PWM1B wollen wir nicht, damit keine Schaltverluste bei konstanter Spannungsrichtung
	EPwm2Regs.DBCTL.bit.IN_MODE 		= DBA_ALL;				//DBA_ALL ; DBB_RED_DBA_FED; DBA_RED_DBB_FED;DBB_ALL //SW: 25.03.15 "DBA_ALL" beide Deadtimes werden bei PWM2A abgezogen

	EPwm2Regs.DBCTL.bit.POLSEL 			= PWM_GATE_POLARITY;	//DB_ACTV_HI ; DB_ACTV_LOC; DB_ACTV_HIC; DB_ACTV_LO
	EPwm2Regs.DBRED 					= RISING_EDGE_DELAY;	//defined in platform.h
	EPwm2Regs.DBFED 					= FALLING_EDGE_DELAY;	//defined in platform.h

	EPwm2Regs.TBCTR						= 0x0000;				// Anfangswert des TBCounters
	EPwm2Regs.TBCTL.bit.PHSDIR 			= TB_DOWN;				//TB_UP ; TB_DOWN
	EPwm2Regs.TBCTL.bit.HSPCLKDIV 		= TB_DIV1;				// TB_DIV1 ; TB_DIV2 ; TB_DIV4
	EPwm2Regs.TBCTL.bit.CLKDIV 			= TB_DIV1;				// TB_DIV1 ; TB_DIV2 ; TB_DIV4
	// Set Compare Values
	EPwm2Regs.CMPA.half.CMPA 			= vala;					//Einstellung des Initialwertes für Comperator a
	EPwm2Regs.CMPB 						= valb;
	// Chopper
	EPwm2Regs.PCCTL.bit.CHPEN 			= 0; 					// bypass the chopper
	// Trip Zone
	EPwm2Regs.TZCTL.bit.TZA 			= HIGH_SIDE_TRIP_EVENT;	//TZ_HIZ ; TZ_FORCE_HI ; TZ_FORCE_LO ; TZ_NO_CHANGE
	EPwm2Regs.TZCTL.bit.TZB 			= LOW_SIDE_TRIP_EVENT;	//TZ_HIZ ; TZ_FORCE_HI ; TZ_FORCE_LO ; TZ_NO_CHANGE
	//Event Trigger and Interrupt
	EPwm2Regs.ETSEL.bit.INTSEL 			= ET_CTR_ZERO;			//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm2Regs.ETSEL.bit.INTEN  			= 0;					//INT_ON ; INT_OFF
	EPwm2Regs.ETPS.bit.INTPRD  			= ET_1ST;				//ET_DISABLE ; ET_1ST ; ET_2ND ; ET_3RD
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC 	= 1;					//Synchronisation des PWM clocks enablen
	EDIS;
}

void InitEPWM_CH3(int period, int vala, int valb)
{

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;	//Synchronisation des PWM clocks disablen

	// Time Base
	EPwm3Regs.TBPRD 					= period;				// Wert der Periode in TBCLKs
	EPwm3Regs.TBPHS.half.TBPHS 			= 0x0000;				// Wert auf den TBCTR gesetzt wir wenn TBCLKSYNK
	EPwm3Regs.TBCTL.bit.CTRMODE 		= TB_COUNT_UPDOWN;		// TB_COUNT_UP ; TB_COUNT_DOWN ; TB_COUNT_UPDOWN ; TB_FREEZE
	EPwm3Regs.TBCTL.bit.PHSEN 			= TB_DISABLE;			// TB_DISABLE ; TB_ENABLE
	EPwm3Regs.TBCTL.bit.PRDLD 			= TB_SHADOW;			// TB_SHADOW ; TB_IMMEDIATE
	EPwm3Regs.TBCTL.bit.SYNCOSEL 		= TB_SYNC_IN;			//TB_SYNC_DISABLE ; TB_CTR_CMPB ; TB_CTR_ZERO ; TB_SYNC_IN
	// Setup shadow register load on ZERO
	EPwm3Regs.CMPCTL.bit.SHDWAMODE 		= CC_SHADOW;			//CC_SHADOW ; CC_IMMEDIATE
	EPwm3Regs.CMPCTL.bit.SHDWBMODE 		= CC_SHADOW;			//CC_SHADOW ; CC_IMMEDIATE
	EPwm3Regs.CMPCTL.bit.LOADAMODE 		= CC_CTR_ZERO; 			//CC_CTR_ZERO ; CC_CTR_PRD ; CC_CTR_ZERO_PRD ; CC_LD_DISABLE
	EPwm3Regs.CMPCTL.bit.LOADBMODE 		= CC_CTR_ZERO;			//CC_CTR_ZERO ; CC_CTR_PRD ; CC_CTR_ZERO_PRD ; CC_LD_DISABLE
	// Action Qualifier Control
	EPwm3Regs.AQCTLA.bit.CAU 			= AQ_CLEAR;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	EPwm3Regs.AQCTLA.bit.CAD 			= AQ_SET;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	// Dead Band Generator
	EPwm3Regs.DBCTL.bit.OUT_MODE 		= DB_FULL_ENABLE; 		//DBA_ENABLE;	//DB_DISABLE ; DBA_ENABLE ; DBB_ENABLE; DB_FULL_ENABLE//SW: 25.03.15 "DBA_Enable" nur PWM2A macht Deadband, PWM1B wollen wir nicht, damit keine Schaltverluste bei konstanter Spannungsrichtung
	EPwm3Regs.DBCTL.bit.IN_MODE 		= DBA_ALL;				//DBA_ALL ; DBB_RED_DBA_FED; DBA_RED_DBB_FED;DBB_ALL //SW: 25.03.15 "DBA_ALL" beide Deadtimes werden bei PWM2A abgezogen

	EPwm3Regs.DBCTL.bit.POLSEL 			= PWM_GATE_POLARITY;	//DB_ACTV_HI ; DB_ACTV_LOC; DB_ACTV_HIC; DB_ACTV_LO
	EPwm3Regs.DBRED 					= RISING_EDGE_DELAY;	//defined in platform.h
	EPwm3Regs.DBFED 					= FALLING_EDGE_DELAY;	//defined in platform.h

	EPwm3Regs.TBCTR						= 0x0000;				// Anfangswert des TBCounters
	EPwm3Regs.TBCTL.bit.PHSDIR 			= TB_DOWN;				//TB_UP ; TB_DOWN
	EPwm3Regs.TBCTL.bit.HSPCLKDIV 		= TB_DIV1;				// TB_DIV1 ; TB_DIV2 ; TB_DIV4
	EPwm3Regs.TBCTL.bit.CLKDIV 			= TB_DIV1;				// TB_DIV1 ; TB_DIV2 ; TB_DIV4
	// Set Compare Values
	EPwm3Regs.CMPA.half.CMPA 			= vala;					//Einstellung des Initialwertes für Comperator a
	EPwm3Regs.CMPB 						= valb;
	// Chopper
	EPwm3Regs.PCCTL.bit.CHPEN 			= 0; 					// bypass the chopper
	// Trip Zone
	EPwm3Regs.TZCTL.bit.TZA 			= HIGH_SIDE_TRIP_EVENT;	//TZ_HIZ ; TZ_FORCE_HI ; TZ_FORCE_LO ; TZ_NO_CHANGE
	EPwm3Regs.TZCTL.bit.TZB 			= LOW_SIDE_TRIP_EVENT;	//TZ_HIZ ; TZ_FORCE_HI ; TZ_FORCE_LO ; TZ_NO_CHANGE
	//Event Trigger and Interrupt
	EPwm3Regs.ETSEL.bit.INTSEL 			= ET_CTR_ZERO;			//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm3Regs.ETSEL.bit.INTEN  			= 0;					//INT_ON ; INT_OFF
	EPwm3Regs.ETPS.bit.INTPRD  			= ET_1ST;				//ET_DISABLE ; ET_1ST ; ET_2ND ; ET_3RD
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC 	= 1;					//Synchronisation des PWM clocks enablen
	EDIS;
}

void InitEPWM_CH4(int period, int vala, int valb) {
	// Assumes ePWM1 clock is already enabled in InitSysCtrl();
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC 	= 0;					//Synchronisation des PWM clocks disablen
	// Time-base registers
	EPwm4Regs.TBPRD 					= period;           	// Set EPWM timer period
	EPwm4Regs.TBPHS.half.TBPHS 			= 0x0000;         		// Time-Base Phase Register
	EPwm4Regs.TBCTL.bit.CTRMODE 		= TB_COUNT_UPDOWN; 		// Count-up mode
	EPwm4Regs.TBCTL.bit.PHSEN 			= TB_DISABLE;      		// Disable phase loading
	EPwm4Regs.TBCTL.bit.PRDLD 			= TB_SHADOW;	    	// Set shadow
	EPwm4Regs.TBCTL.bit.SYNCOSEL 		= TB_CTR_ZERO;  		// gibt den Takt an
	// Setup shadow register load on ZERO
	EPwm4Regs.CMPCTL.bit.SHDWAMODE 		= CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.SHDWBMODE 		= CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.LOADAMODE 		= CC_CTR_ZERO; 			// load on CTR=Zero
	EPwm4Regs.CMPCTL.bit.LOADBMODE 		= CC_CTR_ZERO; 			// load on CTR=Zero
	// Action Qualifier Control
	EPwm4Regs.AQCTLA.bit.CAU 			= AQ_CLEAR;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	EPwm4Regs.AQCTLA.bit.CAD 			= AQ_SET;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	// Dead Band Generator
	EPwm4Regs.DBCTL.bit.OUT_MODE 		= DB_FULL_ENABLE; 		//DBA_ENABLE;	//DB_DISABLE ; DBA_ENABLE ; DBB_ENABLE; DB_FULL_ENABLE//SW: 25.03.15 "DBA_Enable" nur PWM1A macht Deadband, PWM1B wollen wir nicht, damit keine Schaltverluste bei konstanter Spannungsrichtung
	EPwm4Regs.DBCTL.bit.IN_MODE 		= DBA_ALL;				//DBA_ALL ; DBB_RED_DBA_FED; DBA_RED_DBB_FED;DBB_ALL //SW: 25.03.15 "DBA_ALL" beide Deadtimes werden bei PWM1A abgezogen

	EPwm4Regs.DBCTL.bit.POLSEL 			= PWM_GATE_POLARITY;	//defined in platform.h
	EPwm4Regs.DBRED 					= RISING_EDGE_DELAY;	//defined in platform.h
	EPwm4Regs.DBFED 					= FALLING_EDGE_DELAY;	//defined in platform.h

	EPwm4Regs.TBCTR 					= 0x0000;           	// Time-Base Counter Register
	EPwm4Regs.TBCTL.bit.PHSDIR 			= TB_UP; 				//SW: Nicht auskommentiert wegen DemonstratorV2
	EPwm4Regs.TBCTL.bit.HSPCLKDIV 		= TB_DIV1;     			// TBCLK = SYSCLKOUT
	EPwm4Regs.TBCTL.bit.CLKDIV 			= TB_DIV1;
	// Set Compare values
	EPwm4Regs.CMPA.half.CMPA 			= vala;
	EPwm4Regs.CMPB 						= valb;
	// Chopper
	EPwm4Regs.PCCTL.bit.CHPEN 			= 0; 					// bypass the chopper
	// Trip Zone
	EPwm4Regs.TZCTL.bit.TZA 			= HIGH_SIDE_TRIP_EVENT;	// trip events defined in platform.h
	EPwm4Regs.TZCTL.bit.TZB 			= LOW_SIDE_TRIP_EVENT;

	//Event Trigger and Interrupt
	EPwm4Regs.ETSEL.bit.INTSEL 			= ET_CTR_PRD;			//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm4Regs.ETSEL.bit.INTEN 			= 1;
	EPwm4Regs.ETPS.bit.INTPRD 			= ET_1ST;				//ET_DISABLE ; ET_1ST ; ET_2ND ; ET_3RD
	EPwm4Regs.ETSEL.bit.SOCAEN 			= 1;
	EPwm4Regs.ETSEL.bit.SOCASEL 		= ET_CTR_ZERO;			//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm4Regs.ETSEL.bit.SOCBEN 			= 0;					//INT_ON ; INT_OFF
	EPwm4Regs.ETSEL.bit.SOCBSEL 		= ET_CTR_ZERO;			//ET_CTR_PRD;SW: 25.03.15 angepasst	//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm4Regs.ETPS.bit.SOCACNT 			= ET_1ST;				//ET_DISABLE ; ET_1ST ; ET_2ND ; ET_3RD
	EPwm4Regs.ETPS.bit.SOCAPRD 			= ET_1ST;
	EPwm4Regs.ETPS.bit.SOCBPRD 			= ET_1ST;
	EPwm4Regs.ETPS.bit.SOCBCNT 			= ET_1ST;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC 	= 1;					//Synchronisation des PWM clocks enablen
	EDIS;
}

void InitEPWM_CH5(int period, int vala, int valb)
{

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;						//Synchronisation des PWM clocks disablen

	// Time Base
	EPwm5Regs.TBPRD 					= period;				// Wert der Periode in TBCLKs
	EPwm5Regs.TBPHS.half.TBPHS 			= 0x0000;				// Wert auf den TBCTR gesetzt wir wenn TBCLKSYNK
	EPwm5Regs.TBCTL.bit.CTRMODE 		= TB_COUNT_UPDOWN;		// TB_COUNT_UP ; TB_COUNT_DOWN ; TB_COUNT_UPDOWN ; TB_FREEZE
	EPwm5Regs.TBCTL.bit.PHSEN 			= TB_DISABLE;			// TB_DISABLE ; TB_ENABLE
	EPwm5Regs.TBCTL.bit.PRDLD 			= TB_SHADOW;			// TB_SHADOW ; TB_IMMEDIATE
	EPwm5Regs.TBCTL.bit.SYNCOSEL 		= TB_SYNC_IN;			//TB_SYNC_DISABLE ; TB_CTR_CMPB ; TB_CTR_ZERO ; TB_SYNC_IN
	// Setup shadow register load on ZERO
	EPwm5Regs.CMPCTL.bit.SHDWAMODE 		= CC_SHADOW;			//CC_SHADOW ; CC_IMMEDIATE
	EPwm5Regs.CMPCTL.bit.SHDWBMODE 		= CC_SHADOW;			//CC_SHADOW ; CC_IMMEDIATE
	EPwm5Regs.CMPCTL.bit.LOADAMODE 		= CC_CTR_ZERO; 			//CC_CTR_ZERO ; CC_CTR_PRD ; CC_CTR_ZERO_PRD ; CC_LD_DISABLE
	EPwm5Regs.CMPCTL.bit.LOADBMODE 		= CC_CTR_ZERO;			//CC_CTR_ZERO ; CC_CTR_PRD ; CC_CTR_ZERO_PRD ; CC_LD_DISABLE
	// Action Qualifier Control
	EPwm5Regs.AQCTLA.bit.CAU 			= AQ_CLEAR;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	EPwm5Regs.AQCTLA.bit.CAD 			= AQ_SET;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	// Dead Band Generator
	EPwm5Regs.DBCTL.bit.OUT_MODE 		= DB_FULL_ENABLE; 		//DBA_ENABLE;	//DB_DISABLE ; DBA_ENABLE ; DBB_ENABLE; DB_FULL_ENABLE//SW: 25.03.15 "DBA_Enable" nur PWM2A macht Deadband, PWM1B wollen wir nicht, damit keine Schaltverluste bei konstanter Spannungsrichtung
	EPwm5Regs.DBCTL.bit.IN_MODE 		= DBA_ALL;				//DBA_ALL ; DBB_RED_DBA_FED; DBA_RED_DBB_FED;DBB_ALL //SW: 25.03.15 "DBA_ALL" beide Deadtimes werden bei PWM2A abgezogen

	EPwm5Regs.DBCTL.bit.POLSEL 			= PWM_GATE_POLARITY;	//DB_ACTV_HI ; DB_ACTV_LOC; DB_ACTV_HIC; DB_ACTV_LO
	EPwm5Regs.DBRED 					= RISING_EDGE_DELAY;	//defined in platform.h
	EPwm5Regs.DBFED 					= FALLING_EDGE_DELAY;	//defined in platform.h

	EPwm5Regs.TBCTR						= 0x0000;				// Anfangswert des TBCounters
	EPwm5Regs.TBCTL.bit.PHSDIR 			= TB_DOWN;				//TB_UP ; TB_DOWN
	EPwm5Regs.TBCTL.bit.HSPCLKDIV 		= TB_DIV1;				// TB_DIV1 ; TB_DIV2 ; TB_DIV4
	EPwm5Regs.TBCTL.bit.CLKDIV 			= TB_DIV1;				// TB_DIV1 ; TB_DIV2 ; TB_DIV4
	// Set Compare Values
	EPwm5Regs.CMPA.half.CMPA 			= vala;					//Einstellung des Initialwertes für Comperator a
	EPwm5Regs.CMPB 						= valb;
	// Chopper
	EPwm5Regs.PCCTL.bit.CHPEN 			= 0; 					// bypass the chopper
	// Trip Zone
	EPwm5Regs.TZCTL.bit.TZA 			= HIGH_SIDE_TRIP_EVENT;	//TZ_HIZ ; TZ_FORCE_HI ; TZ_FORCE_LO ; TZ_NO_CHANGE
	EPwm5Regs.TZCTL.bit.TZB 			= LOW_SIDE_TRIP_EVENT;	//TZ_HIZ ; TZ_FORCE_HI ; TZ_FORCE_LO ; TZ_NO_CHANGE
	//Event Trigger and Interrupt
	EPwm5Regs.ETSEL.bit.INTSEL 			= ET_CTR_ZERO;			//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm5Regs.ETSEL.bit.INTEN  			= 0;					//INT_ON ; INT_OFF
	EPwm5Regs.ETPS.bit.INTPRD  			= ET_1ST;				//ET_DISABLE ; ET_1ST ; ET_2ND ; ET_3RD
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC 	= 1;					//Synchronisation des PWM clocks enablen
	EDIS;
}

void InitEPWM_CH6(int period, int vala, int valb)
{

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;	//Synchronisation des PWM clocks disablen

	// Time Base
	EPwm6Regs.TBPRD 					= period;				// Wert der Periode in TBCLKs
	EPwm6Regs.TBPHS.half.TBPHS 			= 0x0000;				// Wert auf den TBCTR gesetzt wir wenn TBCLKSYNK
	EPwm6Regs.TBCTL.bit.CTRMODE 		= TB_COUNT_UPDOWN;		// TB_COUNT_UP ; TB_COUNT_DOWN ; TB_COUNT_UPDOWN ; TB_FREEZE
	EPwm6Regs.TBCTL.bit.PHSEN 			= TB_DISABLE;			// TB_DISABLE ; TB_ENABLE
	EPwm6Regs.TBCTL.bit.PRDLD 			= TB_SHADOW;			// TB_SHADOW ; TB_IMMEDIATE
	EPwm6Regs.TBCTL.bit.SYNCOSEL 		= TB_SYNC_IN;			//TB_SYNC_DISABLE ; TB_CTR_CMPB ; TB_CTR_ZERO ; TB_SYNC_IN
	// Setup shadow register load on ZERO
	EPwm6Regs.CMPCTL.bit.SHDWAMODE 		= CC_SHADOW;			//CC_SHADOW ; CC_IMMEDIATE
	EPwm6Regs.CMPCTL.bit.SHDWBMODE 		= CC_SHADOW;			//CC_SHADOW ; CC_IMMEDIATE
	EPwm6Regs.CMPCTL.bit.LOADAMODE 		= CC_CTR_ZERO; 			//CC_CTR_ZERO ; CC_CTR_PRD ; CC_CTR_ZERO_PRD ; CC_LD_DISABLE
	EPwm6Regs.CMPCTL.bit.LOADBMODE 		= CC_CTR_ZERO;			//CC_CTR_ZERO ; CC_CTR_PRD ; CC_CTR_ZERO_PRD ; CC_LD_DISABLE
	// Action Qualifier Control
	EPwm6Regs.AQCTLA.bit.CAU 			= AQ_CLEAR;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	EPwm6Regs.AQCTLA.bit.CAD 			= AQ_SET;				//AQ_NO_ACTION ; AQ_CLEAR ; AQ_SET ; AQ_TOGGLE
	// Dead Band Generator
	EPwm6Regs.DBCTL.bit.OUT_MODE 		= DB_FULL_ENABLE; 		//DBA_ENABLE;	//DB_DISABLE ; DBA_ENABLE ; DBB_ENABLE; DB_FULL_ENABLE//SW: 25.03.15 "DBA_Enable" nur PWM2A macht Deadband, PWM1B wollen wir nicht, damit keine Schaltverluste bei konstanter Spannungsrichtung
	EPwm6Regs.DBCTL.bit.IN_MODE 		= DBA_ALL;				//DBA_ALL ; DBB_RED_DBA_FED; DBA_RED_DBB_FED;DBB_ALL //SW: 25.03.15 "DBA_ALL" beide Deadtimes werden bei PWM2A abgezogen

	EPwm6Regs.DBCTL.bit.POLSEL 			= PWM_GATE_POLARITY;	//DB_ACTV_HI ; DB_ACTV_LOC; DB_ACTV_HIC; DB_ACTV_LO
	EPwm6Regs.DBRED 					= RISING_EDGE_DELAY;	//defined in platform.h
	EPwm6Regs.DBFED 					= FALLING_EDGE_DELAY;	//defined in platform.h

	EPwm6Regs.TBCTR						= 0x0000;				// Anfangswert des TBCounters
	EPwm6Regs.TBCTL.bit.PHSDIR 			= TB_DOWN;				//TB_UP ; TB_DOWN
	EPwm6Regs.TBCTL.bit.HSPCLKDIV 		= TB_DIV1;				// TB_DIV1 ; TB_DIV2 ; TB_DIV4
	EPwm6Regs.TBCTL.bit.CLKDIV 			= TB_DIV1;				// TB_DIV1 ; TB_DIV2 ; TB_DIV4
	// Set Compare Values
	EPwm6Regs.CMPA.half.CMPA 			= vala;					//Einstellung des Initialwertes für Comperator a
	EPwm6Regs.CMPB 						= valb;
	// Chopper
	EPwm6Regs.PCCTL.bit.CHPEN 			= 0; 					// bypass the chopper
	// Trip Zone
	EPwm6Regs.TZCTL.bit.TZA 			= HIGH_SIDE_TRIP_EVENT;	//TZ_HIZ ; TZ_FORCE_HI ; TZ_FORCE_LO ; TZ_NO_CHANGE
	EPwm6Regs.TZCTL.bit.TZB 			= LOW_SIDE_TRIP_EVENT;	//TZ_HIZ ; TZ_FORCE_HI ; TZ_FORCE_LO ; TZ_NO_CHANGE
	//Event Trigger and Interrupt
	EPwm6Regs.ETSEL.bit.INTSEL 			= ET_CTR_ZERO;			//ET_CTR_ZERO ; ET_CTR_PRD ; ET_CTRU_CMPA ; ET_CTRD_CMPA ; ET_CTRU_CMPB ; ET_CTRD_CMPB
	EPwm6Regs.ETSEL.bit.INTEN  			= 0;					//INT_ON ; INT_OFF
	EPwm6Regs.ETPS.bit.INTPRD  			= ET_1ST;				//ET_DISABLE ; ET_1ST ; ET_2ND ; ET_3RD
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC 	= 1;					//Synchronisation des PWM clocks enablen
	EDIS;
}
#endif //(PLATFORM == PLAT_STEUER)
