/*
 * javascope.h
 *
 *  Created on: 23.04.2018
 *      Author: LoehdefinkPh
 */

#ifndef INCLUDE_JAVASCOPE_H_
#define INCLUDE_JAVASCOPE_H_


extern float zerovalue;

// Do not change the first (zero) and last (end) entries.
enum JS_OberservableData {
    JSO_ZEROVALUE=0,
	JSO_SineWave1,
    JSO_Sawtooth1,
    JSO_SineWave2,
	JSO_Sawtooth2,
	JSO_ua,
	JSO_ub,
	JSO_uc,
	JSO_ia,
	JSO_ib,
	JSO_ic,
	JSO_id,
	JSO_iq,
	JSO_ud,
	JSO_uq,
	JSO_speed,
    JSO_ENDMARKER
};

// slowData Naming Convention: Use INT or FLOAT to automatically identify Data Type in Java.
// Do not change the first (zero) and last (end) entries.
enum JS_SlowData {
	JSSD_ZEROVALUE=0,
	JSSD_INT_SecondsSinceSystemStart,
	JSSD_FLOAT_uSecPerIsr,
	JSSD_FLOAT_Sine,
	JSSD_FLOAT_FreqReadback,
	JSSD_INT_Milliseconds,
	JSSD_FLOAT_ADCconvFactorReadback,
	JSSD_FLOAT_Rs,
	JSSD_FLOAT_Ld,
	JSSD_FLOAT_Lq,
	JSSD_FLOAT_psiPM,
	JSSD_FLOAT_J,
	JSSD_INT_polePairs,
	JSSD_INT_activeState,
	JSSD_FLOAT_u_d,
	JSSD_FLOAT_u_q,
	JSSD_FLOAT_i_d,
	JSSD_FLOAT_i_q,
	JSSD_FLOAT_speed,
	JSSD_FLOAT_torque,
	JSSD_FLOAT_encoderOffset,
	JSSD_FLOAT_u_d_ref,
	JSSD_FLOAT_u_q_ref,
	JSSD_FLOAT_CustomValue1,
	JSSD_FLOAT_CustomValue2,
	JSSD_FLOAT_CustomValue3,
	JSSD_FLOAT_CustomValue4,
	JSSD_FLOAT_CustomValue5,
	JSSD_FLOAT_CustomValue6,
	JSSD_FLOAT_CustomValue7,
	JSSD_FLOAT_CustomValue8,
	JSSD_ENDMARKER
};


union SlowData {
   Int32 i;
   float32 f;
};


extern float *js_ptr_arr[JSO_ENDMARKER];
extern float *js_ptr[4];	// channel ptr

extern union SlowData js_slowDataArray[JSSD_ENDMARKER];


void js_init();

//void js_fetchData(float data1, float data2, float data3, float data4);
void js_fetchData4CH();
void js_fetchData2CH();


#endif /* INCLUDE_JAVASCOPE_H_ */
