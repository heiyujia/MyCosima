/*
 * javascope.c
 *
 *  Created on: 23.04.2018
 *      Author: LoehdefinkPh
 */

#include "DSP28x_Project.h"
#include "../main_c28.h"
#include "../include/javascope.h"

float myIQfactor[15] = {1.0, 2.0, 4.0, 8.0, 16.0, 32.0, 64.0, 128.0, 256.0, 512.0, 1024.0, 2048.0, 4096.0, 8192.0, 16384.0};
// Int16 Datentyp geht von −32.768 bis 32.767
// Wertebereiche mit myIQfactor:
// 0  :  −32768 bis 32767				Auflösung 1.0
// 1  :  −16384 bis 16383.5				Auflösung 0.5
// 2  :  −8192  bis 8191.75				Auflösung 0.25
// 3  :  −4096  bis 4095.875			Auflösung 0.125
// 4  :  −2048  bis 2047.9375			Auflösung 0.0625
// 5  :  −1024  bis 1023.96875			Auflösung 0.03125
// 6  :  −512   bis 511.984375			Auflösung 0.015625
// 7  :  −256   bis 255.9921875			Auflösung 0.0078125
// 8  :  −128   bis 127.99609375  		Auflösung 0.00390625
// 9  :  −64    bis 63.998046875		Auflösung 0.001953125
// 10 :  −32    bis 31.999023437		Auflösung 0.000976563
// 11 :  −16    bis 15.999511719		Auflösung 0.000488281
// 12 :  −8     bis 7.999755859			Auflösung 0.000244141
// 13 :  −4     bis 3.999877930			Auflösung 0.000122070
// 14 :  −2 	bis 1.999938965			Auflösung 0.000061035


extern c28_to_m3_shared_struct c28_to_m3;
Uint32 cnt_javascope=0, cnt_slowData=0;

Uint16 js_factor1 = 0, js_factor2 = 0, js_factor3 = 0, js_factor4 = 0;

// Channel-pointer and pointer array
float *js_ptr_arr[JSO_ENDMARKER];
float *js_ptr[4];	// channel ptr
float zerovalue = 0.0;

union SlowData js_slowDataArray[JSSD_ENDMARKER];

void js_init()
{
	int j=0;

	for (j=0; j<JSO_ENDMARKER; j++)
	{
		js_ptr_arr[j] = &zerovalue;
	}

	js_ptr[0] = &zerovalue;
	js_ptr[1] = &zerovalue;
	js_ptr[2] = &zerovalue;
	js_ptr[3] = &zerovalue;

	for (j=0; j<JSSD_ENDMARKER; j++)
	{
		js_slowDataArray[j].i = (Uint32) zerovalue;
	}

}


void js_fetchData4CH()
{
	static Int16 values[20];

	Int16 data1_int = (Int16)( myIQfactor[js_factor1] * *js_ptr[0]);
	Int16 data2_int = (Int16)( myIQfactor[js_factor2] * *js_ptr[1]);
	Int16 data3_int = (Int16)( myIQfactor[js_factor3] * *js_ptr[2]);
	Int16 data4_int = (Int16)( myIQfactor[js_factor4] * *js_ptr[3]);

	if (cnt_javascope < 5)
	{
		values[cnt_javascope] = (Int16) data1_int;
		values[5+cnt_javascope] = (Int16) data2_int;
		values[10+cnt_javascope] = (Int16) data3_int;
		values[15+cnt_javascope] = (Int16) data4_int;
	}

	cnt_javascope++;

	if (cnt_javascope >= 5)
	{
		int j=0;
		for (j=0; j<20; j++)
		{
			c28_to_m3.val[j] = values[j];
		}

		memcpy(&(c28_to_m3.slowDataContent), &(js_slowDataArray[cnt_slowData]), sizeof(Int32));		// copy without automatic float -> int conversion
		c28_to_m3.slowDataID = cnt_slowData;
		cnt_slowData++;
		if (cnt_slowData >= JSSD_ENDMARKER)
			cnt_slowData = 0;


		cnt_javascope = 0;

		CtoMIpcRegs.CTOMIPCSET.bit.IPC3 = 1;	// notify m3 that new data is available
	}
}

void js_fetchData2CH()
{
	static Int16 values[20];

	Int16 data1_int = (Int16)( myIQfactor[js_factor1] * *js_ptr[0]);
	Int16 data2_int = (Int16)( myIQfactor[js_factor2] * *js_ptr[1]);

	if (cnt_javascope < 10)
	{
		values[cnt_javascope] = (Int16) data1_int;
		values[10+cnt_javascope] = (Int16) data2_int;
	}

	cnt_javascope++;

	if (cnt_javascope >= 10)
	{
		int j=0;
		for (j=0; j<20; j++)
		{
			c28_to_m3.val[j] = values[j];
		}

		memcpy(&(c28_to_m3.slowDataContent), &(js_slowDataArray[cnt_slowData]), sizeof(Int32));		// copy without automatic float -> int conversion
		c28_to_m3.slowDataID = cnt_slowData;
		cnt_slowData++;
		if (cnt_slowData >= JSSD_ENDMARKER)
			cnt_slowData = 0;

		cnt_javascope = 0;

		CtoMIpcRegs.CTOMIPCSET.bit.IPC3 = 1;	// notify m3 that new data is available
	}
}



