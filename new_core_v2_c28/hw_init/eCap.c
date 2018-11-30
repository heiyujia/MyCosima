/*
 * eCap.c
 *
 *  Created on: 13.11.2015
 *      Author: Sebastian W.
 *
 *      Für das Einlesen von rechteckigen Hall-Signalen
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "../include/eCap.h"

//---------------------------------------------------------------------------
// InitECap:
//---------------------------------------------------------------------------
// This function initializes the eCAP(s) to a known state.
//
void InitECap(void) {
	// Initialize eCAP1/2/3

	InitECap1();
	InitECap2();
	InitECap3();

}

void InitECap1(void) {
//	EALLOW;
//		ECap1Regs.ECEINT.all = 0x0000; 			// Disable all capture interrupts
//		ECap1Regs.ECCLR.all = 0xFFFF; 			// Clear all CAP interrupt flags
//		ECap1Regs.ECCTL1.bit.CAPLDEN = 0; 		// Disable CAP1-CAP4 register loads
//		ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0; 	// Make sure the counter is stopped
//
//		ECap1Regs.ECCTL1.bit.CAP1POL = 0;		// 0 = Rising Edge, 1 = Falling Edge
//		ECap1Regs.ECCTL1.bit.CAP2POL = 1;		// 0 = Rising Edge, 1 = Falling Edge
//		ECap1Regs.ECCTL1.bit.CAP3POL = 0;		// 0 = Rising Edge, 1 = Falling Edge
//		ECap1Regs.ECCTL1.bit.CAP4POL = 1;		// 0 = Rising Edge, 1 = Falling Edge
//		ECap1Regs.ECCTL1.bit.CTRRST1 = 1;		// 1 = Reset TSCTR-value at CAP-Event1, 0 = do not reset
//		ECap1Regs.ECCTL1.bit.CTRRST2 = 1;		// 1 = Reset TSCTR-value at CAP-Event2, 0 = do not reset
//		ECap1Regs.ECCTL1.bit.CTRRST3 = 1;		// 1 = Reset TSCTR-value at CAP-Event3, 0 = do not reset
//		ECap1Regs.ECCTL1.bit.CTRRST4 = 1;		// 1 = Reset TSCTR-value at CAP-Event4, 0 = do not reset
//		ECap1Regs.ECCTL1.bit.CAPLDEN = 1;
//		ECap1Regs.ECCTL1.bit.PRESCALE = 0;
//		ECap1Regs.ECCTL2.bit.CAP_APWM = 0;
//		ECap1Regs.ECCTL2.bit.CONT_ONESHT = 0;
//		ECap1Regs.ECCTL2.bit.SYNCO_SEL = 2;
//		ECap1Regs.ECCTL2.bit.SYNCI_EN = 0;
//		ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;
//	//	ECap1Regs.ECCTL2.bit.REARM = 1; 		// 1 = reset MOD4-CTR -> always start with CEVT1?
//		ECap1Regs.ECEINT.bit.CEVT1 = 1; 		// 1 Capture Event 1 Interrupt Enable
//		ECap1Regs.ECEINT.bit.CEVT2 = 1; 		// 2 Capture Event 2 Interrupt Enable
//		ECap1Regs.ECEINT.bit.CEVT3 = 1; 		// 3 Capture Event 3 Interrupt Enable
//		ECap1Regs.ECEINT.bit.CEVT4 = 1; 		// 4 Capture Event 4 Interrupt Enable
//		ECap1Regs.ECEINT.bit.CTROVF = 0; 		// 5 Counter Overflow Interrupt Enable
//		ECap1Regs.ECEINT.bit.CTR_EQ_PRD1 = 0; 	// 6 Period Equal Interrupt Enable
//		ECap1Regs.ECEINT.bit.CTR_EQ_CMP = 0; 	// 7 Compare Equal Interrupt Enable
//
//		// ECAP interrupt clear
//		ECap1Regs.ECCLR.bit.INT = 1; // 0 Global Flag
//		ECap1Regs.ECCLR.bit.CEVT1 = 1; 			// 1 Capture Event 1 Interrupt Flag
//		ECap1Regs.ECCLR.bit.CEVT2 = 1; 			// 2 Capture Event 2 Interrupt Flag
//		ECap1Regs.ECCLR.bit.CEVT3 = 1; 			// 3 Capture Event 3 Interrupt Flag
//		ECap1Regs.ECCLR.bit.CEVT4 = 1; 			// 4 Capture Event 4 Interrupt Flag
//		ECap1Regs.ECCLR.bit.CTROVF = 1; 		// 5 Counter Overflow Interrupt Flag
//		ECap1Regs.ECCLR.bit.CTR_EQ_PRD1 = 1; 	// 6 Period Equal Interrupt Flag
//		EDIS;
	EALLOW;
	ECap1Regs.ECEINT.all = 0x0000;             // Disable all capture interrupts
	ECap1Regs.ECCLR.all = 0xFFFF;              // Clear all CAP interrupt flags
	ECap1Regs.ECCTL1.bit.CAPLDEN = 0; // Disable CAP1-CAP4 register loads  !!!!Wichtig
	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;      // Make sure the counter is stopped

	// Configure peripheral registers
	ECap1Regs.ECCTL2.bit.CAP_APWM = 0;		// SW: 0= Operate in CAP-Mode
	ECap1Regs.ECCTL2.bit.CONT_ONESHT = 1;      //1 One-shot
	ECap1Regs.ECCTL2.bit.STOP_WRAP = 0;        //3 Stop at 4 events
	ECap1Regs.ECCTL2.bit.SYNCI_EN = 1;         // Enable sync in
	ECap1Regs.ECCTL2.bit.SYNCO_SEL = 0;        // Pass through

	ECap1Regs.ECCTL1.bit.CAP1POL = 0;          // Falling edge
	ECap1Regs.ECCTL1.bit.CAP2POL = 1;          // Rising edge
	ECap1Regs.ECCTL1.bit.CAP3POL = 1;          // Falling edge
	ECap1Regs.ECCTL1.bit.CAP4POL = 0;          // Rising edge
	ECap1Regs.ECCTL1.bit.CTRRST1 = 1;          // Difference operation
	ECap1Regs.ECCTL1.bit.CTRRST2 = 1;          // Difference operation
	ECap1Regs.ECCTL1.bit.CTRRST3 = 1;          // Difference operation
	ECap1Regs.ECCTL1.bit.CTRRST4 = 1;          // Difference operation

	ECap1Regs.ECCTL1.bit.PRESCALE = 0;   //Brauch ich des?????

	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;        // Start Counter
	ECap1Regs.ECCTL2.bit.REARM = 1;            // arm one-shot
	ECap1Regs.ECCTL1.bit.CAPLDEN = 1;        // Enable capture units !!!!Wichtig
	ECap1Regs.ECEINT.bit.CEVT1 = 1; // 4 events = interrupt SW: schon nach einem Event den Interrupt schalten
	ECap1Regs.ECEINT.bit.CEVT2 = 1; // 4 events = interrupt SW: schon nach einem Event den Interrupt schalten

	//  ECap1Regs.ECFLG.bit.CEVT1 = 1;            // zeigt nur an das ein Interrupt schaltet

	EDIS;
}

void InitECap2(void) {
//	EALLOW;
//		ECap2Regs.ECEINT.all = 0x0000; 			// Disable all capture interrupts
//		ECap2Regs.ECCLR.all = 0xFFFF; 			// Clear all CAP interrupt flags
//		ECap2Regs.ECCTL1.bit.CAPLDEN = 0; 		// Disable CAP1-CAP4 register loads
//		ECap2Regs.ECCTL2.bit.TSCTRSTOP = 0; 	// Make sure the counter is stopped
//
//		ECap2Regs.ECCTL1.bit.CAP1POL = 0;		// 0 = Rising Edge, 1 = Falling Edge
//		ECap2Regs.ECCTL1.bit.CAP2POL = 1;		// 0 = Rising Edge, 1 = Falling Edge
//		ECap2Regs.ECCTL1.bit.CAP3POL = 0;		// 0 = Rising Edge, 1 = Falling Edge
//		ECap2Regs.ECCTL1.bit.CAP4POL = 1;		// 0 = Rising Edge, 1 = Falling Edge
//		ECap2Regs.ECCTL1.bit.CTRRST1 = 1;		// 1 = Reset TSCTR-value at CAP-Event1, 0 = do not reset
//		ECap2Regs.ECCTL1.bit.CTRRST2 = 1;		// 1 = Reset TSCTR-value at CAP-Event2, 0 = do not reset
//		ECap2Regs.ECCTL1.bit.CTRRST3 = 1;		// 1 = Reset TSCTR-value at CAP-Event3, 0 = do not reset
//		ECap2Regs.ECCTL1.bit.CTRRST4 = 1;		// 1 = Reset TSCTR-value at CAP-Event4, 0 = do not reset
//		ECap2Regs.ECCTL1.bit.CAPLDEN = 1;
//		ECap2Regs.ECCTL1.bit.PRESCALE = 0;
//		ECap2Regs.ECCTL2.bit.CAP_APWM = 0;
//		ECap2Regs.ECCTL2.bit.CONT_ONESHT = 0;
//		ECap2Regs.ECCTL2.bit.SYNCO_SEL = 2;
//		ECap2Regs.ECCTL2.bit.SYNCI_EN = 0;
//		ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;
//	//	ECap2Regs.ECCTL2.bit.REARM = 1; 		// 1 = reset MOD4-CTR -> always start with CEVT1
//		ECap2Regs.ECEINT.bit.CEVT1 = 1; 		// 1 Capture Event 1 Interrupt Enable
//		ECap2Regs.ECEINT.bit.CEVT2 = 1; 		// 2 Capture Event 2 Interrupt Enable
//		ECap2Regs.ECEINT.bit.CEVT3 = 1; 		// 3 Capture Event 3 Interrupt Enable
//		ECap2Regs.ECEINT.bit.CEVT4 = 1; 		// 4 Capture Event 4 Interrupt Enable
//		ECap2Regs.ECEINT.bit.CTROVF = 0;		// 5 Counter Overflow Interrupt Enable
//		ECap2Regs.ECEINT.bit.CTR_EQ_PRD1 = 0; 	// 6 Period Equal Interrupt Enable
//		ECap2Regs.ECEINT.bit.CTR_EQ_CMP = 0; 	// 7 Compare Equal Interrupt Enable
//
//		// ECAP interrupt clear
//		ECap2Regs.ECCLR.bit.INT = 1; 			// 0 Global Flag
//		ECap2Regs.ECCLR.bit.CEVT1 = 1; 			// 1 Capture Event 1 Interrupt Flag
//		ECap2Regs.ECCLR.bit.CEVT2 = 1; 			// 2 Capture Event 2 Interrupt Flag
//		ECap2Regs.ECCLR.bit.CEVT3 = 1; 			// 3 Capture Event 3 Interrupt Flag
//		ECap2Regs.ECCLR.bit.CEVT4 = 1; 			// 4 Capture Event 4 Interrupt Flag
//		ECap2Regs.ECCLR.bit.CTROVF = 1; 		// 5 Counter Overflow Interrupt Flag
//		ECap2Regs.ECCLR.bit.CTR_EQ_PRD1 = 1;	// 6 Period Equal Interrupt Flag
//		EDIS;
	EALLOW;
	ECap2Regs.ECEINT.all = 0x0000;             // Disable all capture interrupts
	ECap2Regs.ECCLR.all = 0xFFFF;              // Clear all CAP interrupt flags
	ECap2Regs.ECCTL1.bit.CAPLDEN = 0;        // Disable CAP1-CAP4 register loads
	ECap2Regs.ECCTL2.bit.TSCTRSTOP = 0;      // Make sure the counter is stopped

	// Configure peripheral registers
	ECap2Regs.ECCTL2.bit.CAP_APWM = 0;		// SW: 0= Operate in CAP-Mode
	ECap2Regs.ECCTL2.bit.CONT_ONESHT = 1;     //1 One-shot = 1 und Continous = 0
	ECap2Regs.ECCTL2.bit.STOP_WRAP = 0; //3 Stop at 4 events in One-Shot and Wrap after Capture Event 1
	ECap2Regs.ECCTL1.bit.CAP1POL = 1;          // Falling edge
	ECap2Regs.ECCTL1.bit.CAP2POL = 0;          // Rising edge
	ECap2Regs.ECCTL1.bit.CAP3POL = 1;          // Falling edge
	ECap2Regs.ECCTL1.bit.CAP4POL = 0;          // Rising edge
	ECap2Regs.ECCTL1.bit.CTRRST1 = 1;          // Difference operation
	ECap2Regs.ECCTL1.bit.CTRRST2 = 1;          // Difference operation
	ECap2Regs.ECCTL1.bit.CTRRST3 = 1;          // Difference operation
	ECap2Regs.ECCTL1.bit.CTRRST4 = 1;          // Difference operation
	ECap2Regs.ECCTL2.bit.SYNCI_EN = 1; //1 Enable sync in -> Startet einer Timer wenn ein Sync-Signal kommt
	// ECap2Regs.ECCTL2.bit.SWSYNC = 1;         //????????n -> Startet einer Timer wenn ein Sync-Signal kommt

	ECap2Regs.ECCTL2.bit.SYNCO_SEL = 0;        // Pass through
	ECap2Regs.ECCTL1.bit.CAPLDEN = 1;          // Enable capture units

	ECap2Regs.ECCTL1.bit.PRESCALE = 0;   //Brauch ich des?????

	ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;       //1 Start Counter 1 = free running
	ECap2Regs.ECCTL2.bit.REARM = 1; //1 arm one-shot brauch ich nicht, da kein One-Shot verwendet wird
	ECap2Regs.ECCTL1.bit.CAPLDEN = 1;         // Enable CAP1-CAP4 register loads
	ECap2Regs.ECEINT.bit.CEVT1 = 1; // 4 events = interrupt  SW: schon nach einem Event den Interrupt schalten
	ECap2Regs.ECEINT.bit.CEVT2 = 1; // 4 events = interrupt  SW: schon nach einem Event den Interrupt schalten
	ECap2Regs.ECEINT.bit.CEVT3 = 1; // 4 events = interrupt  SW: schon nach einem Event den Interrupt schalten
	ECap2Regs.ECEINT.bit.CEVT4 = 1; // 4 events = interrupt  SW: schon nach einem Event den Interrupt schalten
	EDIS;
}

void InitECap3(void) {
//	EALLOW;
//		ECap3Regs.ECEINT.all = 0x0000; 			// Disable all capture interrupts
//		ECap3Regs.ECCLR.all = 0xFFFF; 			// Clear all CAP interrupt flags
//		ECap3Regs.ECCTL1.bit.CAPLDEN = 0; 		// Disable CAP1-CAP4 register loads
//		ECap3Regs.ECCTL2.bit.TSCTRSTOP = 0; 	// Make sure the counter is stopped
//
//		ECap3Regs.ECCTL1.bit.CAP1POL = 0;		// 0 = Rising Edge, 1 = Falling Edge
//		ECap3Regs.ECCTL1.bit.CAP2POL = 1;		// 0 = Rising Edge, 1 = Falling Edge
//		ECap3Regs.ECCTL1.bit.CAP3POL = 0;		// 0 = Rising Edge, 1 = Falling Edge
//		ECap3Regs.ECCTL1.bit.CAP4POL = 1;		// 0 = Rising Edge, 1 = Falling Edge
//		ECap3Regs.ECCTL1.bit.CTRRST1 = 1;		// 1 = Reset TSCTR-value at CAP-Event1, 0 = do not reset
//		ECap3Regs.ECCTL1.bit.CTRRST2 = 1;		// 1 = Reset TSCTR-value at CAP-Event2, 0 = do not reset
//		ECap3Regs.ECCTL1.bit.CTRRST3 = 1;		// 1 = Reset TSCTR-value at CAP-Event3, 0 = do not reset
//		ECap3Regs.ECCTL1.bit.CTRRST4 = 1;		// 1 = Reset TSCTR-value at CAP-Event4, 0 = do not reset
//		ECap3Regs.ECCTL1.bit.CAPLDEN = 1;
//		ECap3Regs.ECCTL1.bit.PRESCALE = 0;
//		ECap3Regs.ECCTL2.bit.CAP_APWM = 0;
//		ECap3Regs.ECCTL2.bit.CONT_ONESHT = 0;
//		ECap3Regs.ECCTL2.bit.SYNCO_SEL = 2;
//		ECap3Regs.ECCTL2.bit.SYNCI_EN = 0;
//		ECap3Regs.ECCTL2.bit.TSCTRSTOP = 1;
//	//	ECap3Regs.ECCTL2.bit.REARM = 1; 		// 1 = reset MOD4-CTR -> always start with CEVT1
//		ECap3Regs.ECEINT.bit.CEVT1 = 1; 		// 1 Capture Event 1 Interrupt Enable
//		ECap3Regs.ECEINT.bit.CEVT2 = 1; 		// 2 Capture Event 2 Interrupt Enable
//		ECap3Regs.ECEINT.bit.CEVT3 = 1; 		// 3 Capture Event 3 Interrupt Enable
//		ECap3Regs.ECEINT.bit.CEVT4 = 1; 		// 4 Capture Event 4 Interrupt Enable
//		ECap3Regs.ECEINT.bit.CTROVF = 0; 		// 5 Counter Overflow Interrupt Enable
//		ECap3Regs.ECEINT.bit.CTR_EQ_PRD1 = 0; 	// 6 Period Equal Interrupt Enable
//		ECap3Regs.ECEINT.bit.CTR_EQ_CMP = 0; 	// 7 Compare Equal Interrupt Enable
//
//		// ECAP interrupt clear
//		ECap3Regs.ECCLR.bit.INT = 1; 			// 0 Global Flag
//		ECap3Regs.ECCLR.bit.CEVT1 = 1; 			// 1 Capture Event 1 Interrupt Flag
//		ECap3Regs.ECCLR.bit.CEVT2 = 1; 			// 2 Capture Event 2 Interrupt Flag
//		ECap3Regs.ECCLR.bit.CEVT3 = 1; 			// 3 Capture Event 3 Interrupt Flag
//		ECap3Regs.ECCLR.bit.CEVT4 = 1; 			// 4 Capture Event 4 Interrupt Flag
//		ECap3Regs.ECCLR.bit.CTROVF = 1; 		// 5 Counter Overflow Interrupt Flag
//		ECap3Regs.ECCLR.bit.CTR_EQ_PRD1 = 1; 	// 6 Period Equal Interrupt Flag
//		EDIS;
	EALLOW;
	ECap3Regs.ECEINT.all = 0x0000;             // Disable all capture interrupts
	ECap3Regs.ECCLR.all = 0xFFFF;              // Clear all CAP interrupt flags
	ECap3Regs.ECCTL1.bit.CAPLDEN = 0;        // Disable CAP1-CAP4 register loads
	ECap3Regs.ECCTL2.bit.TSCTRSTOP = 0;      // Make sure the counter is stopped

	// Configure peripheral registers
	ECap3Regs.ECCTL2.bit.CAP_APWM = 0;		// SW: 0= Operate in CAP-Mode
	ECap3Regs.ECCTL2.bit.CONT_ONESHT = 1;      //1 One-shot
	ECap3Regs.ECCTL2.bit.STOP_WRAP = 0;        //3 Stop at 4 events
	ECap3Regs.ECCTL1.bit.CAP1POL = 0;          // Falling edge
	ECap3Regs.ECCTL1.bit.CAP2POL = 0;          // Rising edge
	ECap3Regs.ECCTL1.bit.CAP3POL = 1;          // Falling edge
	ECap3Regs.ECCTL1.bit.CAP4POL = 0;          // Rising edge
	ECap3Regs.ECCTL1.bit.CTRRST1 = 1;          // Difference operation
	ECap3Regs.ECCTL1.bit.CTRRST2 = 1;          // Difference operation
	ECap3Regs.ECCTL1.bit.CTRRST3 = 1;          // Difference operation
	ECap3Regs.ECCTL1.bit.CTRRST4 = 1;          // Difference operation
	ECap3Regs.ECCTL2.bit.SYNCI_EN = 1;         // Enable sync in
	ECap3Regs.ECCTL2.bit.SYNCO_SEL = 0;        // Pass through
	ECap3Regs.ECCTL1.bit.CAPLDEN = 1;          // Enable capture units

	// ECap3Regs.ECCTL2.bit.SWSYNC = 1;

	ECap3Regs.ECCTL1.bit.PRESCALE = 0;   //Brauch ich des?????

	ECap3Regs.ECCTL2.bit.TSCTRSTOP = 1;        // Start Counter
	ECap3Regs.ECCTL2.bit.REARM = 1;            // arm one-shot
	ECap3Regs.ECCTL1.bit.CAPLDEN = 1;         // Enable CAP1-CAP4 register loads
	ECap3Regs.ECEINT.bit.CEVT1 = 1; // 4 events = interrupt  SW: schon nach einem Event den Interrupt schalten
	EDIS;
}
