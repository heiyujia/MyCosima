/*
 * isr.h
 *
 *  Created on: 06.11.2013
 *      Author: ELSYS
 *      Changed: Sebastian Wendel 15.11.2015
 */

#ifndef ISR_H_
#define ISR_H_

#include "../main_c28.h"

#define IPC_FLAG3  	0x00000004  // IPC FLAG 3
#define IPC_FLAG4  	0x00000008  // IPC FLAG 4

interrupt void  adc1_isr();
interrupt void  mtoc_isr();
interrupt void Eecap1_isr();
interrupt void Eecap2_isr();
interrupt void Eecap3_isr();

#endif /* ISR_H_ */
