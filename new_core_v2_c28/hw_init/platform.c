/*
 * platform.c
 *
 *  Created on: 28.09.2018
 *      Author: Michael Hoerner
 */

#include "DSP28x_Project.h"
#include "../main_c28.h"

#include "../include/platform.h"
#include "../include/adc.h"
#include "../include/epwm.h"
#include "../include/my_posspeed.h"

#include "../include/application.h"
#include "../include/javascope.h"

#if (PLATFORM == PLAT_OHMrichter)
void Initialize_Platform(DS_Data* data) {

	//ADC conversion factors
	data->prp.ADCConversionFactorIPH 		= ADCCONVFACTORIPH;
	data->prp.ADCConversionFactorVPH 		= ADCCONVFACTORVPH;
	data->prp.ADCConversionFactorIDC 		= ADCCONVFACTORIDC;
	data->prp.ADCConversionFactorVDC 		= ADCCONVFACTORVDC;
	data->prp.ADCConversionFactorTRQ 		= ADCCONVFACTORTRQ;
	data->prp.ADCConversionFactorTMP 		= ADCCONVFACTORTMP;
	data->prp.ADCConversionFactorANALOGHALL = ADCCONVFACTORANALOGHALL;
}

void Initialize_GPIO() {
	InitGpio();		// init all GPIO's as input and synced to SYSCLKOUT

	// --------- Set GPIO's to PWM functionality ---
	GpioCtrlRegs.PWM_HB1_TOP_PIN = 1;
	GpioCtrlRegs.PWM_HB1_BTM_PIN = 1;
	GpioCtrlRegs.PWM_HB2_TOP_PIN = 1;
	GpioCtrlRegs.PWM_HB2_BTM_PIN = 1;
	GpioCtrlRegs.PWM_HB3_TOP_PIN = 1;
	GpioCtrlRegs.PWM_HB3_BTM_PIN = 1;

	GpioG1CtrlRegs.GPADIR.bit.GPIO0 = 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO1 = 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO2 = 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO3 = 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO4 = 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO5 = 1; // Output

	// --------- Analog Hall interface ------------

	// --------- Digital Hall interface ------------
	EALLOW;
	GpioCtrlRegs.ECAP_MUX.bit.ECAP_PIN_1 	= 1;
	GpioTripRegs.ECAP_TRIP_SEL_1 			= ECAP_GPIO_1;
	GpioCtrlRegs.ECAP_MUX.bit.ECAP_PIN_2 	= 1;
	GpioTripRegs.ECAP_TRIP_SEL_2 			= ECAP_GPIO_2;
	GpioCtrlRegs.ECAP_MUX.bit.ECAP_PIN_3 	= 1;
	GpioTripRegs.ECAP_TRIP_SEL_3 			= ECAP_GPIO_3;
	EDIS;

	EALLOW;
	// --------- Incremental Sensor interface ------------
	GpioCtrlRegs.EQEP_MUX.bit.EQEP_A_PIN	= 1;
	GpioCtrlRegs.EQEP_MUX.bit.EQEP_B_PIN	= 1;
	GpioCtrlRegs.EQEP_MUX.bit.EQEP_S_PIN	= 1; // not used (strobe signal)
	GpioCtrlRegs.EQEP_MUX.bit.EQEP_I_PIN	= 1;
	EDIS;
}

void Initialize_PWM() {
	InitEPWM_CH1(PERIOD, 0, 0);
	InitEPWM_CH2(PERIOD, 0, 0);
	InitEPWM_CH3(PERIOD, 0, 0);
}

void Initialize_ADC_Conversion() {
	Adc1_init();
	Adc2_init();
	ConfigAdc1();
	ConfigAdc2();
}

void Initialize_Javascope() {
	js_init();
}

void Initialize_AnalogHall(DS_Data* data) {

}

void Initialize_DigitalHall(DS_Data* data) {
	InitECap();
}

void Initialize_Hiperface(DS_Data* data) {

}

void Initialize_IncrementalEncoder(DS_Data* data, POSSPEED *p) {
	float tmp_SpeedScaler = (4.0 * (150e6 / 4.0) * 60.0) / (4.0 * data->pC.iEP.countsPerRevolution * data->pC.iEP.maxSpeedRPM);
	p->mech_scaler = (int) ((0.9999 / (4 * data->pC.iEP.countsPerRevolution) * 67108864));
	p->pole_pairs = (int) (data->mrp.motorPolePairNumber);
	p->SpeedScaler = (Uint32) (tmp_SpeedScaler);
	p->BaseRpm = (Uint32) (data->pC.iEP.maxSpeedRPM);
	p->init(p);

}

void Reconfigure_AnalogHall(DS_Data* data) {

}

void Reconfigure_DigitalHall(DS_Data* data) {

}

void Reconfigure_Hiperface(DS_Data* data) {

}

void Reconfigure_IncrementalEncoder(DS_Data* data, POSSPEED *p) {
	float tmp_SpeedScaler = (4.0 * (150e6 / 4.0) * 60.0) / (4.0 * data->pC.iEP.countsPerRevolution * data->pC.iEP.maxSpeedRPM);
		p->mech_scaler = (int) ((0.9999 / (4 * data->pC.iEP.countsPerRevolution) * 67108864));
		p->pole_pairs = (int) (data->mrp.motorPolePairNumber);
		p->SpeedScaler = (Uint32) (tmp_SpeedScaler);
		p->BaseRpm = (Uint32) (data->pC.iEP.maxSpeedRPM);
		p->init(p);
}

void CheckForRotorSensorUpdates(DS_Data* data, POSSPEED *p) {
	if (data->pC.iEP.newEncoderValuesFlag == 1) {
				data->pC.iEP.newEncoderValuesFlag = 0;
				if (data->cw.enableSystem == 0 || data->cw.enableControl == 0) { //be sure that power stage and controller are off

					switch (data->pC.rSS.rotorSensorType) {

					case 0: //analog hall
						Reconfigure_AnalogHall(data);
						data->pC.iEP.newEncoderValuesAck = 1;
						break;

					case 1: //digital hall
						Reconfigure_DigitalHall(data);
						data->pC.iEP.newEncoderValuesAck = 1;
						break;

					case 2: //Hiperface
						Reconfigure_Hiperface(data);
						data->pC.iEP.newEncoderValuesAck = 1;
						break;

					case 3: //incremental encoder
						Reconfigure_IncrementalEncoder(data, p);
						data->pC.iEP.newEncoderValuesAck = 1;
						break;
					default:
						data->pC.iEP.newEncoderValuesAck = 0;
						break;

					}

				}
			}
}
#endif // (PLATFORM == PLAT_OHMrichter)



#if (PLATFORM == PLAT_STEUER)
void Initialize_Platform(DS_Data* data) {

	//ADC conversion factors
	data->prp.ADCConversionFactorIPH_GRID	= ADCCONVFACTORIPH_GRID;
	data->prp.ADCConversionFactorVPH_GRID	= ADCCONVFACTORVPH_GRID;
	data->prp.ADCConversionFactorIPH_MOTOR	= ADCCONVFACTORIPH_MOTOR;
	data->prp.ADCConversionFactorVPH_MOTOR	= ADCCONVFACTORVPH_MOTOR;
	data->prp.ADCConversionFactorIDC 		= ADCCONVFACTORIDC;
	data->prp.ADCConversionFactorVDC 		= ADCCONVFACTORVDC;
	data->prp.ADCConversionFactorTRQ 		= ADCCONVFACTORTRQ;
	data->prp.ADCConversionFactorTMP 		= ADCCONVFACTORTMP;
	data->prp.ADCConversionFactorANALOGHALL = ADCCONVFACTORANALOGHALL;
}

void Initialize_GPIO() {
	InitGpio();		// init all GPIO's as input and synced to SYSCLKOUT

	// --------- Set GPIO's to PWM functionality ---
	GpioCtrlRegs.PWM_HB1_TOP_PIN = 1;
	GpioCtrlRegs.PWM_HB1_BTM_PIN = 1;
	GpioCtrlRegs.PWM_HB2_TOP_PIN = 1;
	GpioCtrlRegs.PWM_HB2_BTM_PIN = 1;
	GpioCtrlRegs.PWM_HB3_TOP_PIN = 1;
	GpioCtrlRegs.PWM_HB3_BTM_PIN = 1;
	GpioCtrlRegs.PWM_HB4_TOP_PIN = 1;
	GpioCtrlRegs.PWM_HB4_BTM_PIN = 1;
	GpioCtrlRegs.PWM_HB5_TOP_PIN = 1;
	GpioCtrlRegs.PWM_HB5_BTM_PIN = 1;
	GpioCtrlRegs.PWM_HB6_TOP_PIN = 1;
	GpioCtrlRegs.PWM_HB6_BTM_PIN = 1;

	GpioG1CtrlRegs.GPADIR.bit.GPIO0 	= 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO1 	= 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO2 	= 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO3 	= 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO4 	= 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO5 	= 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO6 	= 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO7 	= 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO8 	= 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO9 	= 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO10 	= 1; // Output
	GpioG1CtrlRegs.GPADIR.bit.GPIO11 	= 1; // Output

	// --------- Analog Hall interface ------------

	// --------- Digital Hall interface ------------
//	EALLOW;
//	GpioCtrlRegs.ECAP_MUX.bit.ECAP_PIN_1 	= 1;
//	GpioTripRegs.ECAP_TRIP_SEL_1 			= ECAP_GPIO_1;
//	GpioCtrlRegs.ECAP_MUX.bit.ECAP_PIN_2 	= 1;
//	GpioTripRegs.ECAP_TRIP_SEL_2 			= ECAP_GPIO_2;
//	GpioCtrlRegs.ECAP_MUX.bit.ECAP_PIN_3 	= 1;
//	GpioTripRegs.ECAP_TRIP_SEL_3 			= ECAP_GPIO_3;
//	EDIS;


	// --------- Incremental Sensor interface ------------
//	EALLOW;
//	GpioCtrlRegs.EQEP_MUX.bit.EQEP_A_PIN	= 1;
//	GpioCtrlRegs.EQEP_MUX.bit.EQEP_B_PIN	= 1;
//	GpioCtrlRegs.EQEP_MUX.bit.EQEP_S_PIN	= 1; // not used (strobe signal)
//	GpioCtrlRegs.EQEP_MUX.bit.EQEP_I_PIN	= 1;
//	EDIS;
}

void Initialize_PWM() {
	InitEPWM_CH1(PERIOD, 0, 0);
	InitEPWM_CH2(PERIOD, 0, 0);
	InitEPWM_CH3(PERIOD, 0, 0);
	InitEPWM_CH4(PERIOD, 0, 0);
	InitEPWM_CH5(PERIOD, 0, 0);
	InitEPWM_CH6(PERIOD, 0, 0);
}

void Initialize_ADC_Conversion() {
	Adc1_init();
	Adc2_init();
	ConfigAdc1();
	ConfigAdc2();
}

void Initialize_Javascope() {
	js_init();
}

void Initialize_AnalogHall(DS_Data* data) {

}

void Initialize_DigitalHall(DS_Data* data) {
	//InitECap();
}

void Initialize_Hiperface(DS_Data* data) {

}

void Initialize_IncrementalEncoder(DS_Data* data, POSSPEED *p) {
//	float tmp_SpeedScaler = (4.0 * (150e6 / 4.0) * 60.0) / (4.0 * data->pC.iEP.countsPerRevolution * data->pC.iEP.maxSpeedRPM);
//	p->mech_scaler = (int) ((0.9999 / (4 * data->pC.iEP.countsPerRevolution) * 67108864));
//	p->pole_pairs = (int) (data->mrp.motorPolePairNumber);
//	p->SpeedScaler = (Uint32) (tmp_SpeedScaler);
//	p->BaseRpm = (Uint32) (data->pC.iEP.maxSpeedRPM);
//	p->init(p);

}

void Reconfigure_AnalogHall(DS_Data* data) {

}

void Reconfigure_DigitalHall(DS_Data* data) {

}

void Reconfigure_Hiperface(DS_Data* data) {

}

void Reconfigure_IncrementalEncoder(DS_Data* data, POSSPEED *p) {
//	float tmp_SpeedScaler = (4.0 * (150e6 / 4.0) * 60.0) / (4.0 * data->pC.iEP.countsPerRevolution * data->pC.iEP.maxSpeedRPM);
//	p->mech_scaler = (int) ((0.9999 / (4 * data->pC.iEP.countsPerRevolution) * 67108864));
//	p->pole_pairs = (int) (data->mrp.motorPolePairNumber);
//	p->SpeedScaler = (Uint32) (tmp_SpeedScaler);
//	p->BaseRpm = (Uint32) (data->pC.iEP.maxSpeedRPM);
//	p->init(p);
}

void CheckForRotorSensorUpdates(DS_Data* data, POSSPEED *p) {
//	if (data->pC.iEP.newEncoderValuesFlag == 1) {
//				data->pC.iEP.newEncoderValuesFlag = 0;
//				if (data->cw.enableSystem == 0 || data->cw.enableControl == 0) { //be sure that power stage and controller are off
//
//					switch (data->pC.rSS.rotorSensorType) {
//
//					case 0: //analog hall
//						Reconfigure_AnalogHall(data);
//						data->pC.iEP.newEncoderValuesAck = 1;
//						break;
//
//					case 1: //digital hall
//						Reconfigure_DigitalHall(data);
//						data->pC.iEP.newEncoderValuesAck = 1;
//						break;
//
//					case 2: //Hiperface
//						Reconfigure_Hiperface(data);
//						data->pC.iEP.newEncoderValuesAck = 1;
//						break;
//
//					case 3: //incremental encoder
//						Reconfigure_IncrementalEncoder(data, p);
//						data->pC.iEP.newEncoderValuesAck = 1;
//						break;
//					default:
//						data->pC.iEP.newEncoderValuesAck = 0;
//						break;
//
//					}
//
//				}
//			}
}
#endif // (PLATFORM == PLAT_STEUER)


#if (PLATFORM == PLAT_CUSTOM)

#endif // (PLATFORM == PLAT_CUSTOM)


