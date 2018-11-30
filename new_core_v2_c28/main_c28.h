/*
 *  main_c28.h
 */

#ifndef INCLUDE_MAIN_C28_H
#define INCLUDE_MAIN_C28_H

#ifdef __cplusplus
extern "C" {
#endif

#include "include/platform_selector.h"

//================================= DEFINES ===================================
//=============================================================================
#define PI						3.14159265358979
#define PI2						6.28318530717958
/* Helper for combining two defines (e.g. 'GPIO' and '24' to -> 'GPIO24') */
#define CONCAT2(a, b)			a ## b
#define CONCAT(a, b)			CONCAT2(a, b)
//==============================END DEFINES ===================================
//=============================================================================

//============================ VARIABLES ======================================
//=============================================================================
// JAVASCOPE
typedef struct {
	Uint32 status_ctom;
	Uint16 schiebereg_ausgaenge;
	Uint32 slowDataContent;
	Uint16 slowDataID;
	Int16 val[20];
} c28_to_m3_shared_struct;

typedef struct {
	Uint16 id;
	Int16 value;
	Uint16 digInputs;
} m3_to_c28_shared_struct;
//END JAVASCOPE


#if (PLATFORM == PLAT_OHMrichter)
// PERIPHERAL STRUCTS
typedef struct _incrementalEncoderParams_ {
	Uint16 newEncoderValuesFlag;
	Uint16 newEncoderValuesAck;
	Uint16 countsPerRevolution;
	Uint16 maxSpeedRPM;
	float  encoderOffset;
} incrementalEncoderParams;

typedef struct _rotorSensorSelection_ {
	Uint16 rotorSensorType;
	Uint16 digHallSensorType;
} rotorSensorSelection;

typedef struct _peripheralConfiguration_ {
	rotorSensorSelection rSS;
	incrementalEncoderParams iEP;
} peripheralConfiguration;

typedef struct {
	float H1;
	float H2;
	float H3;
} Hall_struct_Analog;

typedef struct {
	Bool H1;
	Bool H2;
	Bool H3;
} Hall_struct_Digital;

//APPLICATION VARIABLES
typedef struct _applicationVariables_ {
	Uint16 accept;
	Uint16 reset;
	float sampleTimeISR;
	float polePairs;
	float dutyCyc;
	float n_ref_measurement;
	float n_ref;
	Uint16 identLq;
	Uint16 controlType;
	Uint16 VibON;
	float VibAmp;
	Uint16 VibFreq;
	float i_d_ref;
	float i_q_ref;
	Uint16 MotorID;
	float Kp_id;
	float Tn_id;
	float Kp_iq;
	float Tn_iq;
	float Kp_n;
	float Tn_n;
	Uint16 identR;
	float identRAmp;
	Uint16 AMM_on;
	Uint16 AMM_run;
	float IDstart;
	float IDstop;
	float IDstepsize;
	float IQstart;
	float IQstop;
	float IQstepsize;
	float LogIntervals;
	Uint32 NumberOfSamples;
	float settlingTime;
	float WindingTemp;
	float R_s_ref;
	float Temp_ref;
	float Trq_ref;
	Uint16 FOCFeedForward;
	Uint16 modulationMethod;
	Uint16 controlReference;
	Uint16 reset_error;
	Uint16 ADCconvFactorReadRequest;
	float ADCconvFactorReadback;
	Uint16 activeState;
	float R_s;
	float L_d;
	float L_q;
	float psiPM;
	float rotorInertia;
	Uint16 Polpaarzahl;
	float u_d;
	float u_q;
	float i_d;
	float i_q;
	float speed;
	float ud_ref;
	float uq_ref;
	float geber_offset;
	Uint16 offsetLock;
} applicationVariables;

typedef enum {
	flagDisabled = 0, flagEnabled
} enableFlag;

typedef enum {
	valueFalse = 0, valueTrue
} boolean;

typedef struct _rawADCvaluesAndOffsets_ {
	Uint16 I_U_int;
	Uint16 I_V_int;
	Uint16 I_W_int;
	Uint16 U_U_int;
	Uint16 U_V_int;
	Uint16 U_W_int;
	Uint16 U_ZK_int;
	Uint16 AnalogHall_H1_int;
	Uint16 AnalogHall_H2_int;
	Uint16 AnalogHall_H3_int;
	float i_U_offset;
	float i_V_offset;
	float i_W_offset;
	float u_U_offset;
	float u_V_offset;
	float u_W_offset;
	float AnalogHall_H1_offset;
	float AnalogHall_H2_offset;
	float AnalogHall_H3_offset;
} rawADCvaluesAndOffsets;

typedef struct _platformRelatedParameters_ {
	float ADCConversionFactorIPH;
	float ADCConversionFactorVPH;
	float ADCConversionFactorIDC;
	float ADCConversionFactorVDC;
	float ADCConversionFactorTRQ;
	float ADCConversionFactorTMP;
	float ADCConversionFactorANALOGHALL;
	int sensorType;
} platformRelatedParameters;

typedef struct _controlWord_ {
	enableFlag initSystemDone :1;
	enableFlag ADCoffsetCalculationDone :1;
	enableFlag initApplicationDone :1;
	enableFlag enableSystem :1;
	enableFlag enableControl :1;
	boolean rsvd1 :1;
	boolean rsvd2 :1;
} controlWord;

typedef struct _statusWord_ {
	boolean rsvd1 :1;
	boolean rsvd2 :1;
	boolean rsvd3 :1;
	boolean rsvd4 :1;
} statusWord;

typedef struct _statusReset_ {
	boolean rsvd1 :1;
	boolean rsvd2 :1;
	boolean rsvd3 :1;
	boolean rsvd4 :1;
} statusReset;

typedef struct _errorWord_ {
	int voltageErrorCounterAC;
	int currentErrorCounterAC;
	int voltageErrorCounterDC;
	boolean overCurrentDetectedAC :1;
	boolean overVoltageDetectedAC :1;
	boolean overVoltageDetectedDClink :1;
	boolean rsvd1 :1;
	boolean rsvd2 :1;
	boolean rsvd3 :1;
	boolean rsvd4 :1;
} errorWord;

typedef struct _errorReset_ {
	boolean rsvd1 :1;
	boolean rsvd2 :1;
	boolean rsvd3 :1;
	boolean rsvd4 :1;
} errorReset;

typedef struct _motorRelatedParameters_ {
	float motorPolePairNumber;
} motorRelatedParameters;

typedef struct _actualValues_ {
	float I_L1; // grid
	float I_L2;
	float I_L3;
	float U_L1;
	float U_L2;
	float U_L3;
	float I_U;  // motor
	float I_V;
	float I_W;
	float U_U;
	float U_V;
	float U_W;
	float U_ZK;
	float U_ZK2;
	float Res1;
	float Res2;
	float mechanicalRotorSpeed; //rpm
	float mechanicalPosition; //m
	float mechanicalTorque; // Nm
	float I_d;
	float I_q;
	float U_d;
	float U_q;
	float theta_elec;
	float theta_mech;
	float theta_offset; //in rad/s
	float temperature;
	Uint32 heartbeatframe_content;
	Hall_struct_Digital DigHallState;
	Hall_struct_Analog AnalogHallState;
} actualValues;

typedef struct _DS_Data_ {
	rawADCvaluesAndOffsets rawADC;
	platformRelatedParameters prp;
	peripheralConfiguration pC;
	applicationVariables appV;
	motorRelatedParameters mrp;
	errorWord ew;
	statusWord sw;
	controlWord cw;
	actualValues av;
} DS_Data;
#endif // (PLATFORM == PLAT_OHMrichter)


#if (PLATFORM == PLAT_STEUER)
// PERIPHERAL STRUCTS
typedef struct _incrementalEncoderParams_ {
	Uint16 newEncoderValuesFlag;
	Uint16 newEncoderValuesAck;
	Uint16 countsPerRevolution;
	Uint16 maxSpeedRPM;
	float  encoderOffset;
} incrementalEncoderParams;

typedef struct _rotorSensorSelection_ {
	Uint16 rotorSensorType;
	Uint16 digHallSensorType;
} rotorSensorSelection;

typedef struct _peripheralConfiguration_ {
	rotorSensorSelection rSS;
	incrementalEncoderParams iEP;
} peripheralConfiguration;

typedef struct {
	float H1;
	float H2;
	float H3;
} Hall_struct_Analog;

typedef struct {
	Bool H1;
	Bool H2;
	Bool H3;
} Hall_struct_Digital;

//APPLICATION VARIABLES
typedef struct _applicationVariables_ {
	float TestVariable1;
} applicationVariables;

typedef enum {
	flagDisabled = 0, flagEnabled
} enableFlag;

typedef enum {
	valueFalse = 0, valueTrue
} boolean;

typedef struct _rawADCvaluesAndOffsets_ {
	Uint16 I_L1_int;	//grid
	Uint16 I_L2_int;
	Uint16 I_L3_int;
	Uint16 U_L1_int;
	Uint16 U_L2_int;
	Uint16 U_L3_int;
	Uint16 I_U_int;		// motor
	Uint16 I_V_int;
	Uint16 I_W_int;
	Uint16 U_U_int;
	Uint16 U_V_int;
	Uint16 U_W_int;
	Uint16 U_ZK_int;
	Uint16 AnalogHall_H1_int;
	Uint16 AnalogHall_H2_int;
	Uint16 AnalogHall_H3_int;
	float i_L1_offset;
	float i_L2_offset;
	float i_L3_offset;
	float u_L1_offset;
	float u_L2_offset;
	float u_L3_offset;
	float i_U_offset;
	float i_V_offset;
	float i_W_offset;
	float u_U_offset;
	float u_V_offset;
	float u_W_offset;
	float AnalogHall_H1_offset;
	float AnalogHall_H2_offset;
	float AnalogHall_H3_offset;
} rawADCvaluesAndOffsets;

typedef struct _platformRelatedParameters_ {
	float ADCConversionFactorIPH_GRID;
	float ADCConversionFactorVPH_GRID;
	float ADCConversionFactorIPH_MOTOR;
	float ADCConversionFactorVPH_MOTOR;
	float ADCConversionFactorIDC;
	float ADCConversionFactorVDC;
	float ADCConversionFactorTRQ;
	float ADCConversionFactorTMP;
	float ADCConversionFactorANALOGHALL;
	int sensorType;
} platformRelatedParameters;

typedef struct _controlWord_ {
	enableFlag initSystemDone :1;
	enableFlag ADCoffsetCalculationDone :1;
	enableFlag initApplicationDone :1;
	enableFlag enableSystem :1;
	enableFlag enableControl :1;
	boolean rsvd1 :1;
	boolean rsvd2 :1;
} controlWord;

typedef struct _statusWord_ {
	boolean rsvd1 :1;
	boolean rsvd2 :1;
	boolean rsvd3 :1;
	boolean rsvd4 :1;
} statusWord;

typedef struct _statusReset_ {
	boolean rsvd1 :1;
	boolean rsvd2 :1;
	boolean rsvd3 :1;
	boolean rsvd4 :1;
} statusReset;

typedef struct _errorWord_ {
	int voltageErrorCounterAC;
	int currentErrorCounterAC;
	int voltageErrorCounterDC;
	boolean overCurrentDetectedAC :1;
	boolean overVoltageDetectedAC :1;
	boolean overVoltageDetectedDClink :1;
	boolean rsvd1 :1;
	boolean rsvd2 :1;
	boolean rsvd3 :1;
	boolean rsvd4 :1;
} errorWord;

typedef struct _errorReset_ {
	boolean rsvd1 :1;
	boolean rsvd2 :1;
	boolean rsvd3 :1;
	boolean rsvd4 :1;
} errorReset;

typedef struct _motorRelatedParameters_ {
	float motorPolePairNumber;
} motorRelatedParameters;

typedef struct _actualValues_ {
	float I_L1; // grid
	float I_L2;
	float I_L3;
	float U_L1;
	float U_L2;
	float U_L3;
	float I_U;  // motor
	float I_V;
	float I_W;
	float U_U;
	float U_V;
	float U_W;
	float U_ZK;
	float U_ZK2;
	float Res1;
	float Res2;
	float mechanicalRotorSpeed; //rpm
	float mechanicalPosition; //m
	float mechanicalTorque; // Nm
	float I_d;
	float I_q;
	float U_d;
	float U_q;
	float theta_elec;
	float theta_mech;
	float theta_offset; //in rad/s
	float temperature;
	Uint32 heartbeatframe_content;
	Hall_struct_Digital DigHallState;
	Hall_struct_Analog AnalogHallState;
} actualValues;

typedef struct _DS_Data_ {
	rawADCvaluesAndOffsets rawADC;
	platformRelatedParameters prp;
	peripheralConfiguration pC;
	applicationVariables appV;
	motorRelatedParameters mrp;
	errorWord ew;
	statusWord sw;
	controlWord cw;
	actualValues av;
} DS_Data;
#endif // (PLATFORM == PLAT_STEUER)


#if (PLATFORM == PLAT_CUSTOM)

#endif // (PLATFORM == PLAT_CUSTOM)

void zeit_sekunden_inc();
Uint32 get_zeit_sekunden();


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_C28_H */
