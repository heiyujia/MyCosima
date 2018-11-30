/*
 * adc.c
 *
 *  Created on: 23.07.2012
 *      Author: Philipp Löhdefink
 *      Changed: Sebastian Wendel 13.04.2016
 */

#include "DSP28x_Project.h"
#include <string.h>
#include "../include/platform.h"

void Adc1_init( void )
{
	// Startup & init ADC1
	Uint32 i = 0;
	EALLOW;
	Adc1Regs.ADCCTL1.bit.ADCBGPWD  = 1;      // Power ADC1 BG
	Adc1Regs.ADCCTL1.bit.ADCREFPWD = 1;      // Power reference
	Adc1Regs.ADCCTL1.bit.ADCPWDN   = 1;      // Power ADC1
	Adc1Regs.ADCCTL1.bit.ADCENABLE = 1;      // Enable ADC1
	Adc1Regs.ADCCTL1.bit.ADCREFSEL = 0;      // Select internal BG
	EDIS;
	i=0;
	for (i=0; i<=5000000; i++)
	{
		asm(" nop"); // wait some time for ADC to init ...
	}

}

void Adc2_init( void )
{
	// Startup & init ADC2
	Uint32 i = 0;
	EALLOW;
	Adc2Regs.ADCCTL1.bit.ADCBGPWD  = 1;      // Power ADC2 BG
	Adc2Regs.ADCCTL1.bit.ADCREFPWD = 1;      // Power reference
	Adc2Regs.ADCCTL1.bit.ADCPWDN   = 1;      // Power ADC2
	Adc2Regs.ADCCTL1.bit.ADCENABLE = 1;      // Enable ADC2
	Adc2Regs.ADCCTL1.bit.ADCREFSEL = 0;      // Select internal BG
	EDIS;
	i=0;
	for (i=0; i<=5000000; i++)
	{
		asm(" nop"); // wait some time for ADC to init ...
	}
}


void ConfigAdc1( void ) // MASCHINE
{
    // Configure ADC
    EALLOW;

    Adc1Regs.ADCCTL2.bit.ADCNONOVERLAP = 1;     // Set ADC to non-overlap mode

    Adc1Regs.ADCCTL1.bit.INTPULSEPOS   = 1;     // EOC trips after conversion result is latched
    Adc1Regs.INTSEL1N2.bit.INT1E       = 1;     // Enabled ADCINT1
    Adc1Regs.INTSEL1N2.bit.INT1CONT    = 0;     // Disable ADCINT1 Continuous mode
    Adc1Regs.INTSEL1N2.bit.INT1SEL     = 6;     // setup EOC6 to trigger ADCINT1 - interrupt after 7th conversion
    //Adc1Regs.INTSEL1N2.bit.INT1SEL     = 7;     // setup EOC7 to trigger ADCINT1 - interrupt after 8th conversion

    Adc1Regs.ADCSAMPLEMODE.bit.SIMULEN0 = 0;	// single conversions
    Adc1Regs.ADCSAMPLEMODE.bit.SIMULEN2 = 0;
    Adc1Regs.ADCSAMPLEMODE.bit.SIMULEN4 = 0;
    Adc1Regs.ADCSAMPLEMODE.bit.SIMULEN6 = 0;
    Adc1Regs.ADCSAMPLEMODE.bit.SIMULEN8 = 0;
    Adc1Regs.ADCSAMPLEMODE.bit.SIMULEN10 = 0;
    Adc1Regs.ADCSAMPLEMODE.bit.SIMULEN12 = 0;
    Adc1Regs.ADCSAMPLEMODE.bit.SIMULEN14 = 0;

	#if (PLATFORM == PLAT_OHMrichter)
		Adc1Regs.ADCSOC0CTL.bit.CHSEL	= 0x0;		// ADC1A0 - I_U 	oder I_U1
		Adc1Regs.ADCSOC1CTL.bit.CHSEL	= 0x8;		// ADC1B0 - I_V		oder I_V1
		Adc1Regs.ADCSOC2CTL.bit.CHSEL	= 0x4;		// ADC1A4 - U_ZK	oder I_U2
		Adc1Regs.ADCSOC3CTL.bit.CHSEL	= 0xC;		// ADC1B4 - U_Res2 	oder I_V2
		Adc1Regs.ADCSOC4CTL.bit.CHSEL	= 0x2;		// ADC1A2 - U_W 	oder U_Geber_W1
		Adc1Regs.ADCSOC5CTL.bit.CHSEL	= 0x3;		// ADC1A3 - U_U 	oder U_Geber_U1
		Adc1Regs.ADCSOC6CTL.bit.CHSEL	= 0x6;		// ADC1A6 - Geber_W oder U_Geber_W2
		Adc1Regs.ADCSOC7CTL.bit.CHSEL	= 0x7;		// ADC1A7 - Geber_U oder U_Geber_U2
	#endif

	#if (PLATFORM == PLAT_STEUER)
		Adc1Regs.ADCSOC0CTL.bit.CHSEL	= 0x0;		// ADC1A0 - I_L1
		Adc1Regs.ADCSOC1CTL.bit.CHSEL	= 0x8;		// ADC1B0 - I_L2
		Adc1Regs.ADCSOC2CTL.bit.CHSEL	= 0x4;		// ADC1A4 - I_U
		Adc1Regs.ADCSOC3CTL.bit.CHSEL	= 0xC;		// ADC1B4 - I_V
		Adc1Regs.ADCSOC4CTL.bit.CHSEL	= 0x2;		// ADC1A2 - U_L2
		Adc1Regs.ADCSOC5CTL.bit.CHSEL	= 0x3;		// ADC1A3 - U_V
		Adc1Regs.ADCSOC6CTL.bit.CHSEL	= 0x6;		// ADC1A6 - U_ZK1 (Netzmessplatine, X_SPG1)
		//Adc1Regs.ADCSOC7CTL.bit.CHSEL	= 0x6;		// ADC1A6 - U_ZK1 (Netzmessplatine, X_SPG1)
		Adc1Regs.ADCSOC7CTL.bit.CHSEL	= 0x7;		// ADC1A7 - Res.1 (X51 auf neuer Steuerplatine)
	#endif


    // Selecting triggers for SOCs
    AnalogSysctrlRegs.TRIG5SEL.all     = 5;     // Assigning EPWM1SOCA to TRIGGER 5 of analog subsystem
    AnalogSysctrlRegs.TRIG6SEL.all     = 6;     // Assigning EPWM1SOCB to TRIGGER 6 of analog subsystem

    Adc1Regs.ADCSOC0CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC0 TRIGSEL
    Adc1Regs.ADCSOC1CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC1 TRIGSEL
    Adc1Regs.ADCSOC2CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC2 TRIGSEL
    Adc1Regs.ADCSOC3CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC3 TRIGSEL
    Adc1Regs.ADCSOC4CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC4 TRIGSEL
    Adc1Regs.ADCSOC5CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC5 TRIGSEL
    Adc1Regs.ADCSOC6CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC6 TRIGSEL
    Adc1Regs.ADCSOC7CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC6 TRIGSEL

    // Set S/H window of 7 ADC clock cycles
    Adc1Regs.ADCSOC0CTL.bit.ACQPS      = 6;     // S/H = ACQPS + 1
    Adc1Regs.ADCSOC1CTL.bit.ACQPS      = 6;
    Adc1Regs.ADCSOC2CTL.bit.ACQPS      = 6;
    Adc1Regs.ADCSOC3CTL.bit.ACQPS      = 6;
    Adc1Regs.ADCSOC4CTL.bit.ACQPS      = 6;
    Adc1Regs.ADCSOC5CTL.bit.ACQPS      = 6;
    Adc1Regs.ADCSOC6CTL.bit.ACQPS      = 6;
    Adc1Regs.ADCSOC7CTL.bit.ACQPS      = 6;

    EDIS;
}


void ConfigAdc2( void ) // NETZ!
{
    // Configure ADC
    EALLOW;

    Adc2Regs.ADCCTL2.bit.ADCNONOVERLAP = 1;     // Set ADC to non-overlap mode

    Adc2Regs.ADCCTL1.bit.INTPULSEPOS   = 1;     // EOC trips after conversion result is latched
    Adc2Regs.INTSEL1N2.bit.INT2E       = 1;     // Enabled ADCINT2
	Adc2Regs.INTSEL1N2.bit.INT2CONT    = 0;     // Disable ADCINT2 Continuous mode

	Adc2Regs.INTSEL1N2.bit.INT2SEL     = 6;	// interrupt after 7th conversion
	// -------------------------------------------------------------------------------------------------------------

    Adc2Regs.ADCSAMPLEMODE.bit.SIMULEN0 = 0;	// single conversions
    Adc2Regs.ADCSAMPLEMODE.bit.SIMULEN2 = 0;
    Adc2Regs.ADCSAMPLEMODE.bit.SIMULEN4 = 0;
    Adc2Regs.ADCSAMPLEMODE.bit.SIMULEN6 = 0;
    Adc2Regs.ADCSAMPLEMODE.bit.SIMULEN8 = 0;
    Adc2Regs.ADCSAMPLEMODE.bit.SIMULEN10 = 0;
    Adc2Regs.ADCSAMPLEMODE.bit.SIMULEN12 = 0;
    Adc2Regs.ADCSAMPLEMODE.bit.SIMULEN14 = 0;

	#if (PLATFORM == PLAT_OHMrichter)
		// ------------ Kleinumrichter für 		  Kleinantriebe-DRV oder Linearmotoren  ---------------------------------
		Adc2Regs.ADCSOC0CTL.bit.CHSEL	= 0x0;	// ADC2A0 - I_W 	oder I_W1
		Adc2Regs.ADCSOC1CTL.bit.CHSEL	= 0x4;	// ADC2A4 - U_Res2 	oder I_W2
		Adc2Regs.ADCSOC2CTL.bit.CHSEL	= 0x8;	// ADC2B0 -
		Adc2Regs.ADCSOC3CTL.bit.CHSEL	= 0xC;	// ADC2B4 -
		Adc2Regs.ADCSOC4CTL.bit.CHSEL	= 0x2;	// ADC2A2 - U_V 	oder U_Geber_V1
		Adc2Regs.ADCSOC5CTL.bit.CHSEL	= 0x3;	// ADC2A3 -
		Adc2Regs.ADCSOC6CTL.bit.CHSEL	= 0x6;	// ADC2A6 - Geber_V oder U_Geber_V2
		Adc2Regs.ADCSOC7CTL.bit.CHSEL	= 0x7;	// ADC2A7 -
	#endif
	#if (PLATFORM == PLAT_STEUER)
		// ------------ neue Steuerplatine ---------------------------------
		Adc2Regs.ADCSOC0CTL.bit.CHSEL	= 0x0;	// ADC2A0 - I_L3
		Adc2Regs.ADCSOC1CTL.bit.CHSEL	= 0x4;	// ADC2A4 - I_W
		Adc2Regs.ADCSOC2CTL.bit.CHSEL	= 0x8;	// ADC2B0 - U_L1
		Adc2Regs.ADCSOC3CTL.bit.CHSEL	= 0xC;	// ADC2B4 - U_U
		Adc2Regs.ADCSOC4CTL.bit.CHSEL	= 0x2;	// ADC2A2 - U_L3
		Adc2Regs.ADCSOC5CTL.bit.CHSEL	= 0x3;	// ADC2A3 - U_W
		Adc2Regs.ADCSOC6CTL.bit.CHSEL	= 0x6;	// ADC2A6 - U_ZK2 (Maschinenseite, X_SPG2)
		//Adc2Regs.ADCSOC7CTL.bit.CHSEL	= 0x6;	// ADC2A6 - U_ZK2 (Maschinenseite, X_SPG2)
		Adc2Regs.ADCSOC7CTL.bit.CHSEL	= 0x7;	// ADC2A7 - Res.2 (X50 auf neuer Steuerplatine)
	#endif


    // Selecing triggers for SOCs
    Adc2Regs.ADCSOC0CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC0 TRIGSEL
    Adc2Regs.ADCSOC1CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC1 TRIGSEL
    Adc2Regs.ADCSOC2CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC2 TRIGSEL
    Adc2Regs.ADCSOC3CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC3 TRIGSEL
    Adc2Regs.ADCSOC4CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC4 TRIGSEL
    Adc2Regs.ADCSOC5CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC5 TRIGSEL
    Adc2Regs.ADCSOC6CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC5 TRIGSEL
    Adc2Regs.ADCSOC7CTL.bit.TRIGSEL    = 9;     // Assign EPWM1SOCA to SOC5 TRIGSEL

    // Set S/H window of 7 ADC clock cycles
    Adc2Regs.ADCSOC0CTL.bit.ACQPS      = 6;     // S/H = ACQPS + 1
    Adc2Regs.ADCSOC1CTL.bit.ACQPS      = 6;
    Adc2Regs.ADCSOC2CTL.bit.ACQPS      = 6;
    Adc2Regs.ADCSOC3CTL.bit.ACQPS      = 6;
    Adc2Regs.ADCSOC4CTL.bit.ACQPS      = 6;
    Adc2Regs.ADCSOC5CTL.bit.ACQPS      = 6;
    Adc2Regs.ADCSOC6CTL.bit.ACQPS      = 6;
    Adc2Regs.ADCSOC7CTL.bit.ACQPS      = 6;


    EDIS;
}

