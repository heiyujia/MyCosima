/*
 * adc.h
 *
 *  Created on: 13.11.2012
 *      Author: Philipp Löhdefink
 */

// Direkter Zugriff auf ADC Result Register vom M3

#ifndef CORE_V2_M3_ADC_H_
#define CORE_V2_M3_ADC_H_


#define ADRESSE_ADC1 (struct Adc1_Result *) 0x50001600  //Startadresse ADC1-Result-Register M3
#define ADRESSE_ADC2 (struct Adc2_Result *) 0x50001680  //Startadresse ADC2-Result-Register M3


struct Adc1_Result
{
	unsigned short I_L1_int;	//Ergebnis 1
    unsigned short I_L2_int;    //Ergebnis 2
    unsigned short I_U_int;     //Ergebnis 3
    unsigned short I_V_int;     //Ergebnis 4
    unsigned short U_L2_int;    //Ergebnis 5
    unsigned short U_V_int;     //Ergebnis 6
    unsigned short U_ZK1_int;   //Ergebnis 7
    unsigned short reserved0;   //Ergebnis 8
    unsigned short reserved1;   //Ergebnis 9
    unsigned short reserved2;   //Ergebnis 10
    unsigned short reserved3;   //Ergebnis 11
    unsigned short reserved4;   //Ergebnis 12
    unsigned short reserved5;   //Ergebnis 13
    unsigned short reserved6;   //Ergebnis 14
    unsigned short reserved7;   //Ergebnis 15
    unsigned short reserved8;   //Ergebnis 16

} __attribute__ ((packed));

struct Adc2_Result
{
    unsigned short I_L3_int;    //Ergebnis 1
    unsigned short I_W_int;     //Ergebnis 2
    unsigned short U_L1_int;    //Ergebnis 3
    unsigned short U_U_int;     //Ergebnis 4
    unsigned short U_L3_int;    //Ergebnis 5
    unsigned short U_W_int;     //Ergebnis 6
    unsigned short U_ZK2_int;   //Ergebnis 7
    unsigned short reserved1;   //Ergebnis 8
    unsigned short reserved2;   //Ergebnis 9
    unsigned short reserved3;   //Ergebnis 10
    unsigned short reserved4;   //Ergebnis 11
    unsigned short reserved5;   //Ergebnis 12
    unsigned short reserved6;   //Ergebnis 13
    unsigned short reserved7;   //Ergebnis 14
    unsigned short reserved8;   //Ergebnis 15
    unsigned short reserved9;   //Ergebnis 16

} __attribute__ ((packed));


#endif /* CORE_V2_M3_ADC_H_ */
