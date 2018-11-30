/*
 * platform.h
 *
 *  Created on: 28.09.2018
 *      Author: Michael Hoerner
 */

#ifndef INCLUDE_PLATFORM_H_
#define INCLUDE_PLATFORM_H_

// INCLUDES
#include "../main_c28.h"
#include "../include/platform_selector.h"

#include "F28M35x_Adc.h"
#include "epwm.h"
#include "my_posspeed.h"

// PROTOTYPES
void Initialize_GPIO();
void Initialize_ADC_Conversion();
void Initialize_PWM();
void Initialize_Javascope();
void Initialize_AnalogHall(DS_Data* data);
void Initialize_DigitalHall(DS_Data* data);
void Initialize_Hiperface(DS_Data* data);
void Initialize_IncrementalEncoder(DS_Data* data, POSSPEED_handle);
void Reconfigure_AnalogHall(DS_Data* data);
void Reconfigure_DigitalHall(DS_Data* data);
void Reconfigure_Hiperface(DS_Data* data);
void Reconfigure_IncrementalEncoder(DS_Data* data, POSSPEED_handle);
void Initialize_Platform(DS_Data* data);
void CheckForRotorSensorUpdates(DS_Data* data, POSSPEED_handle);

// GENERAL DEFINES
/* PWM counter period value */
#define PERIOD 					7500 //=> 1/150MHz * 7500 * 2 = 100us => 10kHz

/* calc sampling time from PERIOD */
#define CALC_SAMPLINGTIME(a)	(1/150e6*a*2)
#define SAMPLINGTIME			CALC_SAMPLINGTIME(PERIOD)


// PLATFORM SPECIFIC DEFINES
#if (PLATFORM == PLAT_OHMrichter)
//Hardware defines for platform

//SAFETY LIMITS
#define SHORT_TERM_CURRENT_LIMIT			50.0 	// in A
#define SHORT_TERM_CURRENT_DURATION			10		// ISR ticks (e.g. @10 kHz -> 10 * 100 µs = 1 ms)
#define CONTINUOUS_CURRENT_LIMIT			20.0	// in A
#define SHORT_TERM_VOLTAGE_LIMIT			60.0	// in V
#define SHORT_TERM_VOLTAGE_DURATION			10		// ISR ticks
#define CONTINUOUS_VOLTAGE_LIMIT			60.0	// in V
#define SHORT_TERM_DC_LINK_VOLTAGE_LIMIT	60.0	// in V
#define SHORT_TERM_DC_LINK_VOLTAGE_DURATION	10		// ISR ticks

//ADC Pin configuration
#define ADC_I_U 				Adc1Result.ADCRESULT0	//Pin 57	ADC1_A0 SV3.1
#define ADC_I_V 				Adc1Result.ADCRESULT1	//Pin 69   	ADC1_B0 SV3.2
#define ADC_I_W 				Adc2Result.ADCRESULT0	//Pin 19    ADC2_A0 SV3.3
#define ADC_U_ZK				Adc1Result.ADCRESULT2	//Pin 11	ADC1_A4 SV4.1
#define ADC_U_U 				Adc1Result.ADCRESULT5	//Pin 		ADC1_A3 SV5.1
#define ADC_U_V					Adc2Result.ADCRESULT4	//Pin 		ADC2_A2 SV5.2
#define ADC_U_W					Adc1Result.ADCRESULT4	//Pin 		ADC1_A2 SV5.3
#define ADC_ANA_HALL_1			Adc1Result.ADCRESULT7	//Pin 		ADC1_A7 X9.1
#define ADC_ANA_HALL_2			Adc2Result.ADCRESULT6	//Pin 		ADC2_A6 X9.2
#define	ADC_ANA_HALL_3			Adc1Result.ADCRESULT6	//Pin 		ADC1_A6 X9.3

//ADC conversion factors
/* commonly used current sensors:
 *
 * LEM CASR6-NP				   (6 A)	 :				0.003985142
 * Allegro ACS723LLCTR-10AB-T2 (+/- 10 A):				0.00610172
 * Allegro ACS723LLCTR-20AB-T2 (+/- 10 A):				0.01220344
 *
 */
#define ADCCONVFACTORIPH 		0.003985142
#define ADCCONVFACTORVPH		0.03581786
#define ADCCONVFACTORIDC 		0.0
#define ADCCONVFACTORVDC 		0.03581786
#define ADCCONVFACTORTRQ 		0.0
#define ADCCONVFACTORTMP 		0.0
#define ADCCONVFACTORANALOGHALL 0.00122034

//GPIO Pin configuration

/* PWM Pins */
#define PWM_HB1_TOP_PIN			GPAMUX1.bit.GPIO0
#define PWM_HB1_BTM_PIN			GPAMUX1.bit.GPIO1
#define PWM_HB2_TOP_PIN			GPAMUX1.bit.GPIO2
#define PWM_HB2_BTM_PIN			GPAMUX1.bit.GPIO3
#define PWM_HB3_TOP_PIN			GPAMUX1.bit.GPIO4
#define PWM_HB3_BTM_PIN			GPAMUX1.bit.GPIO5

#define PWM_GATE_POLARITY		DB_ACTV_HI //DB_ACTV_HI ; DB_ACTV_LOC; DB_ACTV_HIC; DB_ACTV_LO

#define RISING_EDGE_DELAY		200
#define FALLING_EDGE_DELAY		200

#define HIGH_SIDE_TRIP_EVENT	TZ_FORCE_HI	//TZ_HIZ ; TZ_FORCE_HI ; TZ_FORCE_LO ; TZ_NO_CHANGE
#define LOW_SIDE_TRIP_EVENT		TZ_FORCE_LO

/* ECAP (digital Hall) */
#define ECAP_GPIO_1				24
#define ECAP_GPIO_2				25
#define ECAP_GPIO_3				26

#define ECAP_TRIP_SRC_1			ECAP_GPIO_1
#define ECAP_TRIP_SRC_2			ECAP_GPIO_2
#define ECAP_TRIP_SRC_3			ECAP_GPIO_3
#define	ECAP_PIN_1				CONCAT(GPIO, ECAP_GPIO_1)
#define	ECAP_PIN_2				CONCAT(GPIO, ECAP_GPIO_2)
#define	ECAP_PIN_3				CONCAT(GPIO, ECAP_GPIO_3)
#define ECAP_MUX				GPAMUX2
#define ECAP_TRIP_SEL_1			GPTRIP7SEL.bit.GPTRIP7SEL
#define ECAP_TRIP_SEL_2			GPTRIP8SEL.bit.GPTRIP8SEL
#define ECAP_TRIP_SEL_3			GPTRIP9SEL.bit.GPTRIP9SEL

/* QEP (incremental encoder) */
#define EQEP_GPIO_A				20
#define EQEP_GPIO_B				21
#define EQEP_GPIO_S				22
#define EQEP_GPIO_I				23

#define EQEP_MUX				GPAMUX2

#define EQEP_A_PIN				CONCAT(GPIO, EQEP_GPIO_A)
#define EQEP_B_PIN				CONCAT(GPIO, EQEP_GPIO_B)
#define EQEP_S_PIN				CONCAT(GPIO, EQEP_GPIO_S)
#define EQEP_I_PIN				CONCAT(GPIO, EQEP_GPIO_I)

#endif // (PLATFORM == PLAT_OHMrichter)




#if (PLATFORM == PLAT_STEUER)

//SAFETY LIMITS
#define SHORT_TERM_CURRENT_LIMIT			50.0 	// in A
#define SHORT_TERM_CURRENT_DURATION			10		// ISR ticks (e.g. @10 kHz -> 10 * 1/10 kHz = 1 ms)
#define CONTINUOUS_CURRENT_LIMIT			20.0	// in A
#define SHORT_TERM_VOLTAGE_LIMIT			60.0	// in V
#define SHORT_TERM_VOLTAGE_DURATION			10		// ISR ticks
#define CONTINUOUS_VOLTAGE_LIMIT			60.0	// in V
#define SHORT_TERM_DC_LINK_VOLTAGE_LIMIT	60.0	// in V
#define SHORT_TERM_DC_LINK_VOLTAGE_DURATION	10		// ISR ticks

//Hardware defines for platform
#define ADC_I_L1				0 //???
#define ADC_I_L2				0 //???
#define ADC_I_L3				0 //???
#define ADC_U_L1				0 //???
#define ADC_U_L2				0 //???
#define ADC_U_L3				0 //???
#define ADC_I_U					Adc1Result.ADCRESULT0; 	// 	Pin 57	ADC1_A0 (X52.1 auf DHG-Steuerplatine und "I_L1" bezeichnet)
#define ADC_I_V					Adc1Result.ADCRESULT1;	//  Pin 69 	ADC1_B0 (X53.1 auf DHG-Steuerplatine und "I_L2" bezeichnet)
#define ADC_I_W					Adc2Result.ADCRESULT0;	//  Pin 19 	ADC2_A0 (X54.1 auf DHG-Steuerplatine und "I_L3" bezeichnet)
#define ADC_U_ZK				0 //???
#define ADC_U_U					Adc2Result.ADCRESULT3;	//  Pin 21	ADC2_B4 (X72.5 auf DHG-Steuerplatine und "U_U" bezeichnet)
#define ADC_U_V					Adc1Result.ADCRESULT5;	//  Pin 71	ADC1_A3 (X72.7 auf DHG-Steuerplatine und "U_V" bezeichnet)
#define ADC_U_W					Adc2Result.ADCRESULT5;	//  Pin 15	ADC2_A3 (X72.9 auf DHG-Steuerplatine und "U_W" bezeichnet)
#define	ADC_ANA_HALL_1			0 //???
#define	ADC_ANA_HALL_2			0 //???
#define	ADC_ANA_HALL_3			0 //???

//ADC conversion factors
#define ADCCONVFACTORIPH_GRID	0.003985142
#define ADCCONVFACTORIPH_MOTOR	0.003985142
#define ADCCONVFACTORIDC		0.003985142
#define ADCCONVFACTORVPH_GRID	0.01602198
#define ADCCONVFACTORVPH_MOTOR	0.01602198
#define ADCCONVFACTORVDC		0.01602198
#define ADCCONVFACTORANALOGHALL	0.0
#define ADCCONVFACTORTMP		0.0
#define ADCCONVFACTORTRQ		0.0

//GPIO Pin configuration

/* PWM Pins */
#define PWM_HB1_TOP_PIN			GPAMUX1.bit.GPIO0
#define PWM_HB1_BTM_PIN			GPAMUX1.bit.GPIO1
#define PWM_HB2_TOP_PIN			GPAMUX1.bit.GPIO2
#define PWM_HB2_BTM_PIN			GPAMUX1.bit.GPIO3
#define PWM_HB3_TOP_PIN			GPAMUX1.bit.GPIO4
#define PWM_HB3_BTM_PIN			GPAMUX1.bit.GPIO5
#define PWM_HB4_TOP_PIN			GPAMUX1.bit.GPIO6
#define PWM_HB4_BTM_PIN			GPAMUX1.bit.GPIO7
#define PWM_HB5_TOP_PIN			GPAMUX1.bit.GPIO8
#define PWM_HB5_BTM_PIN			GPAMUX1.bit.GPIO9
#define PWM_HB6_TOP_PIN			GPAMUX1.bit.GPIO10
#define PWM_HB6_BTM_PIN			GPAMUX1.bit.GPIO11

#define PWM_GATE_POLARITY		DB_ACTV_HIC //DB_ACTV_HI ; DB_ACTV_LOC; DB_ACTV_HIC; DB_ACTV_LO

#define RISING_EDGE_DELAY		150
#define FALLING_EDGE_DELAY		150

#define HIGH_SIDE_TRIP_EVENT	TZ_FORCE_LO	//TZ_HIZ ; TZ_FORCE_HI ; TZ_FORCE_LO ; TZ_NO_CHANGE
#define LOW_SIDE_TRIP_EVENT		TZ_FORCE_LO

#endif // (PLATFORM == PLAT_STEUER)


#if (PLATFORM == PLAT_CUSTOM)

// for custom hardware define PIN's and conversion factors here

#endif // (PLATFORM == PLAT_CUSTOM)



#endif /* INCLUDE_PLATFORM_H_ */
