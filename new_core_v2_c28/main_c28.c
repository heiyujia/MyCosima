/*
 * main_c28.c
 */

//====================== INCLUDES AND DEFINES =================================
//=============================================================================
#include "DSP28x_Project.h"
#include "main_c28.h"
#include <string.h>

#include "include/platform.h"
#include "include/eCap.h"
#include "include/my_posspeed.h"
#include "include/isr.h"
#include "include/ipc_c28.h"

#include "include/application.h"
#include "include/javascope.h"
//====================== END INCLUDES AND DEFINES =============================
//=============================================================================

//============================ VARIABLES ======================================
//=============================================================================

DS_Data Global_Data = {0};

#if (PLATFORM  == PLAT_STEUER)
//extern Uint16 sig[8]; 	//SW: 20.05.2015Ausgeklammert, da vorerst nicht benötigt für MPC
//extern Uint16 g_schiebereg_out; 	//SW: 20.05.2015Ausgeklammert, da vorerst nicht benötigt für MPC
#endif
// shared data structs between M3 and C28 processors
m3_to_c28_shared_struct m3_to_c28, m3_to_c28_shadow;
c28_to_m3_shared_struct c28_to_m3;
#pragma DATA_SECTION(m3_to_c28, ".m3_to_c28_shared");
#pragma DATA_SECTION(c28_to_m3, ".c28_to_m3_shared");
Uint16 g_flag_new_m3_to_c28_ipc = 0;

POSSPEED qep_posspeed = POSSPEED_GENERIC;

// Variable fuer Zeitsteuerung in Sekunden -----------------------
// niemals direkt aendern, nur ueber die vorgesehenen Funktionen!!
Uint32 g_zeit_sekunden = 0;	// Datentyp reicht fuer einige Jahre
// ---------------------------------------------------------------
//========================== END VARIABLES ====================================
//=============================================================================

//================================ MAIN =======================================
//=============================================================================
int main(void) {

	volatile Uint32 waitcnt = 0;
	for (waitcnt = 0; waitcnt < 1000000; waitcnt++)
		asm(" nop");
	// wait some time so that ARM can init memories

// Step 1. Initialize System Control for Control and Analog Subsytems
// Enable Peripheral Clocks
// This example function is found in the F28M35x_SysCtrl.c file.
	InitSysCtrl();

// If project is linked into flash, copy critical code sections to RAM.
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t) &RamfuncsLoadSize);
// Step 2. Initialize GPIO:
	Initialize_GPIO();

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
	DINT;

// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the F28M35x_PieCtrl.c file.
	InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F28M35x_DefaultIsr.c.
// This function is found in F28M35x_PieVect.c.
	InitPieVectTable();

// Call Flash Initialization to setup flash waitstates
// This function must reside in RAM
	InitFlash();

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
	EALLOW;
	// interrupt for application
	PieVectTable.ADCINT1 = &adc1_isr;
	// interrupt for inter-processor communication
	PieVectTable.MTOCIPC_INT3 = &mtoc_isr;
	EDIS;


// Step 4. Initialize all the Device Peripherals:
// This function is found in F28M35x_InitPeripherals.c
//InitPeripherals(); // Not required for this example
	// Initialize Global_Data with platform parameters
	Initialize_Platform(&Global_Data);

	// Initialize Global_Data with application initial parameters
	Initialize_Application(&Global_Data);

	// Initialize analog hall interface
	Initialize_AnalogHall(&Global_Data);

	// Initialize digital hall interface
	Initialize_DigitalHall(&Global_Data);

	// Initialize Hiperface interface
	Initialize_Hiperface(&Global_Data);

	// Initialize incremental encoder with Global_Data init values
	Initialize_IncrementalEncoder(&Global_Data, &qep_posspeed);

// Configure 3

	// Configure ADC's
	Initialize_ADC_Conversion();

	// Configure ePWM module
	Initialize_PWM();

	InitCpuTimers();					// Init for ISR Timing measurement
	CpuTimer0Regs.TCR.bit.TSS = 0;		// Init for ISR Timing measurement

// Step 5. User specific code, enable interrupts:

// Enable ADCINT1 in PIE
	PieCtrlRegs.PIEIER1.bit.INTx1 = 1;  // Enable INT 1.1 in the PIE (ADC1)
	IER |= M_INT1;                      // Enable CPU Interrupt 1
	PieCtrlRegs.PIEIER11.bit.INTx3 = 1;	// Enable INT 11.3 in the PIE (IPC3)
	IER |= M_INT11;                      // Enable CPU Interrupt 11

	EINT;
	// Enable Global interrupt INTM
	ERTM;
	// Enable Global realtime interrupt DBGM

//JAVASCOPE
	Initialize_Javascope();



// Wait for an ADC Interrupt
while (1) {

static Uint32 sek_old = 0;






		if (get_zeit_sekunden() != sek_old)	// 1 sek vergangen
				{
			sek_old = get_zeit_sekunden();
		}



		// get control input from M3 via ethernet and GUI
		if (g_flag_new_m3_to_c28_ipc == 1)// Flag is set in isr.c in MTOC-INT
			{
			ipc_mtoc_func(m3_to_c28_shadow.id, m3_to_c28_shadow.value, &Global_Data);
			g_flag_new_m3_to_c28_ipc = 0;
			}

	}

	return 1;
}
//================================ END MAIN ===================================
//=============================================================================

//=============================== AUX FUNCTIONS ===============================
//=============================================================================

// software timer seconds
void zeit_sekunden_inc() {
	g_zeit_sekunden++;
}

Uint32 get_zeit_sekunden() {
	return g_zeit_sekunden;
}
//=============================== END AUX FUNCTIONS ===========================
//=============================================================================
