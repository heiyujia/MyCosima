//=============================================================================
//=========================== main_m3.c =======================================
// Created by: Institut ELSYS - Technische Hochschule Nuernberg, 2017

//====================== INCLUDES AND DEFINES =================================
//=============================================================================
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/System.h>

#include <inc/hw_types.h>
#include <inc/hw_ipc.h>
#include <inc/hw_memmap.h>
#include "driverlib/ipc.h"
#include <driverlib/sysctl.h>
#include "inc/hw_sysctl.h"
#include "driverlib/ram.h"
#include "inc/hw_ram.h"
#include <driverlib/gpio.h>
#include <string.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/heaps/HeapBuf.h>

/* NDK BSD support */
#include <sys/socket.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>

/* Example/Board Header files */
#include "main_m3.h"
#include "../new_core_v2_c28/main_c28.h"
#include "../new_core_v2_c28/include/platform_selector.h"
#include "include/adc.h"
#include "include/Board.h"
#include "include/spiGPIO.h"		  // for Schieberegister
#include "include/SPI_Master_Slave.h" // ToDo: Maintenance needed !!!

/* Defines for RAM init function */
#define M3_MASTER 0
#define C28_MASTER 1
//====================== END INCLUDES AND DEFINES =============================
//=============================================================================

//=============== VARIABLES DEPENDING ON APPLICATION ==========================
//=============================================================================
/* Shared data structures for inter processor communication (IPC) */
c28_to_m3_shared_struct c28_to_m3_shared_data;
c28_to_m3_shared_struct c28_to_m3_shadow;
m3_to_c28_shared_struct m3_to_c28;
#pragma DATA_SECTION(c28_to_m3_shared_data, ".c28_to_m3_shared");
#pragma DATA_SECTION(m3_to_c28, ".m3_to_c28_shared");

/* Variables depending on used PLATFORM */
#if (PLATFORM  == PLAT_STEUER)
extern Void spiTask(UArg arg0, UArg arg1); // für Schieberegister SW 26.01.2015
#endif //(PLATFORM == PLAT_STEUER)

/* SPI Master Task function from SPI_Master_Slave.c */
extern Void masterTaskFxn(UArg arg0, UArg arg1);
//=============== END VARIABLES DEPENDING ON APPLICATION ======================
//=============================================================================

//====================== TCP WORKER FOR JAVA APP ==============================
//=============================================================================
Void tcpWorker(UArg arg0, UArg arg1) {
	int clientfd = (int) arg0;
	int nbytes;
	Bool flag = TRUE;
	Char *buffer;
	Error_Block eb;
	NetworkSendStruct nwsend;
	int foo;

	nwsend.status = 0x00;

	for (foo = 0; foo < NETWORK_SEND_FIELD_SIZE; foo++) {
		nwsend.slowDataContent[foo] = 0;
		nwsend.val_01_uint16[foo] = 0;
		nwsend.val_02_uint16[foo] = 0;
		nwsend.val_03_uint16[foo] = 0;
		nwsend.val_04_uint16[foo] = 0;
		nwsend.val_05_uint16[foo] = 0;
		nwsend.val_06_uint16[foo] = 0;
		nwsend.val_07_uint16[foo] = 0;
		nwsend.val_08_uint16[foo] = 0;
		nwsend.val_09_uint16[foo] = 0;
		nwsend.val_10_uint16[foo] = 0;
		nwsend.val_11_uint16[foo] = 0;
		nwsend.val_12_uint16[foo] = 0;
		nwsend.val_13_uint16[foo] = 0;
		nwsend.val_14_uint16[foo] = 0;
		nwsend.val_15_uint16[foo] = 0;
		nwsend.val_16_uint16[foo] = 0;
		nwsend.val_17_uint16[foo] = 0;
		nwsend.val_18_uint16[foo] = 0;
		nwsend.val_19_uint16[foo] = 0;
		nwsend.val_20_uint16[foo] = 0;
		nwsend.slowDataID[foo] = 0;
	}

	System_printf("tcpWorker: start clientfd = 0x%x\n", clientfd);

	// Make sure Error_Block is initialized
	Error_init(&eb);

	// Get a buffer to receive incoming packets. Use the default heap.
	buffer = Memory_alloc(NULL, TCPPACKETSIZE, 0, &eb);
	if (buffer == NULL) {
		System_printf("tcpWorker: failed to alloc memory\n");
		Task_exit();
	}

	// Loop while we receive data
	while (flag) {
		Uint32 command = 0;
		Uint8 i = 0;

		for (i = 0; i < NETWORK_SEND_FIELD_SIZE; i++) {
			// Is there any new data available?
			Semaphore_pend(semaphore_send, BIOS_WAIT_FOREVER);

			nwsend.slowDataContent[i] = c28_to_m3_shadow.slowDataContent;
			nwsend.val_01_uint16[i] = c28_to_m3_shadow.val[0];
			nwsend.val_02_uint16[i] = c28_to_m3_shadow.val[1];
			nwsend.val_03_uint16[i] = c28_to_m3_shadow.val[2];
			nwsend.val_04_uint16[i] = c28_to_m3_shadow.val[3];
			nwsend.val_05_uint16[i] = c28_to_m3_shadow.val[4];
			nwsend.val_06_uint16[i] = c28_to_m3_shadow.val[5];
			nwsend.val_07_uint16[i] = c28_to_m3_shadow.val[6];
			nwsend.val_08_uint16[i] = c28_to_m3_shadow.val[7];
			nwsend.val_09_uint16[i] = c28_to_m3_shadow.val[8];
			nwsend.val_10_uint16[i] = c28_to_m3_shadow.val[9];
			nwsend.val_11_uint16[i] = c28_to_m3_shadow.val[10];
			nwsend.val_12_uint16[i] = c28_to_m3_shadow.val[11];
			nwsend.val_13_uint16[i] = c28_to_m3_shadow.val[12];
			nwsend.val_14_uint16[i] = c28_to_m3_shadow.val[13];
			nwsend.val_15_uint16[i] = c28_to_m3_shadow.val[14];
			nwsend.val_16_uint16[i] = c28_to_m3_shadow.val[15];
			nwsend.val_17_uint16[i] = c28_to_m3_shadow.val[16];
			nwsend.val_18_uint16[i] = c28_to_m3_shadow.val[17];
			nwsend.val_19_uint16[i] = c28_to_m3_shadow.val[18];
			nwsend.val_20_uint16[i] = c28_to_m3_shadow.val[19];
			nwsend.slowDataID[i] = c28_to_m3_shadow.slowDataID;
		}

		nwsend.status = c28_to_m3_shadow.status_ctom;

		nbytes = send(clientfd, &nwsend, sizeof(nwsend), 0);
		asm(" nop");
		if (nbytes > 0) {
			nbytes = recv(clientfd, (char *) buffer, TCPPACKETSIZE, 0);
			asm(" nop");
			if (nbytes == 4) {
				command = *((Uint32*) buffer); // cast 4 bytes to Uint32
				if (command != 0) {
					// UInt16 temp = ipc_mtoc->counter;
					m3_to_c28.id = (Uint16) command; // Erste 2 Bytes: Commands in Form von Flags/Nummern
					m3_to_c28.value = (Int16) (command >> 16); // Letzte 2 Bytes: Zahlenwert Uebergabe
					IPCMtoCFlagSet(IPC_FLAG3);
				}
			}
			if (nbytes <= 0) {
				close(clientfd);
				flag = FALSE;
			}
		} else {
			close(clientfd);
			flag = FALSE;
		}
	}
	System_printf("tcpWorker stop clientfd = 0x%x\n", clientfd);

	// Free the buffer back to the heap
	Memory_free(NULL, buffer, TCPPACKETSIZE);

	Task_exit();
}
//====================== END TCP WORKER FOR JAVA APP ==========================
//=============================================================================

//====================== TCP HANDLER FOR TCP CONNECTION =======================
//=============================================================================
Void tcpHandler(UArg arg0, UArg arg1) {
	int status;
	int clientfd;
	int server;
	struct sockaddr_in localAddr;
	struct sockaddr_in clientAddr;
	int optval;
	int optlen = sizeof(optval);
	socklen_t addrlen = sizeof(clientAddr);
	Task_Handle taskHandle;
	Task_Params taskParams;
	Error_Block eb;

	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == -1) {
		System_printf("Error: socket not created.\n");
		goto shutdown;
	}

	memset(&localAddr, 0, sizeof(localAddr));
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons(arg0);

	status = bind(server, (struct sockaddr *) &localAddr, sizeof(localAddr));
	if (status == -1) {
		System_printf("Error: bind failed.\n");
		goto shutdown;
	}

	status = listen(server, NUMTCPWORKERS);
	if (status == -1) {
		System_printf("Error: listen failed.\n");
		goto shutdown;
	}

	optval = 1;
	if (setsockopt(server, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
		System_printf("Error: setsockopt failed\n");
		goto shutdown;
	}

	System_printf("tcpHandler: Waiting for connections\n");
	System_flush();

	while (TRUE) {
		/* Wait for incoming request */
		clientfd = accept(server, (struct sockaddr *) &clientAddr, &addrlen);
		System_printf("tcpHandler: Creating thread clientfd = %d\n", clientfd);

		/* Init the Error_Block */
		Error_init(&eb);

		/* Initialize the defaults and set the parameters. */
		Task_Params_init(&taskParams);
		taskParams.instance->name = "tcpWorkr";
		taskParams.arg0 = (UArg) clientfd;
		taskParams.stackSize = 2048;
		taskHandle = Task_create((Task_FuncPtr) tcpWorker, &taskParams, &eb);
		if (taskHandle == NULL) {
			System_printf("tcpHandler: Failed to create new Task\n");
		}

	}

	shutdown: if (server > 0) {
		System_printf("tcpHandler: Shutting Down, Error!");
		System_flush();
		close(server);
	}
}
//====================== END TCP HANDLER FOR TCP CONNECTION ===================
//=============================================================================

//================================ MAIN =======================================
//=============================================================================
Int main(Void) {
	Task_Handle taskHandle;
	Task_Params taskParams;
	Error_Block eb;

	/* Call board init functions */
	Board_initGeneral();
	Board_initEMAC();

	TMDXDOCKH52C1_initDMA();
#if (PLATFORM == PLAT_STEUER) // Is for the "Schieberegister", which is only in use with the "Steuerplatine"
	spiGPIO_init();			  // not for OHMrichter
#endif
#if (PLATFORM == PLAT_OHMrichter)
	SPI_Master_Slave_initSPI(); // Configure Pins for SPI communication
#endif
#if (PLATFORM != PLAT_DRV8301 && PLATFORM != PLAT_OHMrichter)
	Board_initGPIO();		// not for OHMrichter
#endif
	Error_init(&eb);

#if (PLATFORM == PLAT_OHMrichter) //Use this Pin-config for the small inverters
	GPIOPinConfigureCoreSelect(GPIO_PORTA_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT); // C28 controls complete PA -- PWM	// 3F statt FF beim DRV8301-Board!!
	GPIOPinConfigureCoreSelect(GPIO_PORTB_BASE, 0xFF, GPIO_PIN_C_CORE_SELECT); // C28 controls PB 6 / GPIO14 FAULT_N DRV MB 10.02.2015
	GPIOPinConfigureCoreSelect(GPIO_PORTC_BASE, 0xC0, GPIO_PIN_C_CORE_SELECT); // C28 controls PC 6+7 -- LED 2+3	// nur auf ControlCARD
	GPIOPinConfigureCoreSelect(GPIO_PORTH_BASE, GPIO_PIN_0, 0x01); // C28 controls PH 2 / GPIO48 - DRV-Fault für FIKAT-Kleinumrichter
	GPIOPinConfigureCoreSelect(GPIO_PORTH_BASE, GPIO_PIN_2, 0x01); // C28 controls PH 2 / GPIO50 - DRV-Enable für FIKAT-Kleinumrichter
	// PD 0-3 wird vom M3 für den SPI benutzt
	GPIOPinConfigureCoreSelect(GPIO_PORTD_BASE, 0xF0, GPIO_PIN_C_CORE_SELECT); // C28 controls PD 4-7 / GPIO 20-23 -- eQEP  << ok fuer DRV8301-Board
	GPIOPinConfigureCoreSelect(GPIO_PORTE_BASE, 0x07, GPIO_PIN_C_CORE_SELECT); // C28 controls PE 0-2 / GPIO 24-26 -- eCAP (Hall-Sensor)  << ok fuer DRV8301-Board

	//Beispiel es sollen PIN 3 von Port H durchgereicht werden zB. PH3_GPIO51
	//Register 0000 0000 in DEC
	//Pin 3 wählen 0000 1000
	//in Hex umrechnen -> 0x08
	// fertig -> GPIOPinConfigureCoreSelect(GPIO_PORTH_BASE, 0x08, GPIO_PIN_C_CORE_SELECT);
	//Für mehrere GPIOPinConfigureCoreSelect(GPIO_PORTH_BASE, (GPIO_PIN_3) | (),
#endif

#if (PLATFORM == PLAT_STEUER)// USe this pins for the big "Steuerplatine" of DHG
	GPIOPinConfigureCoreSelect(GPIO_PORTA_BASE, 0xFF, 0x01); // C28 controls complete PA -- PWM
	GPIOPinConfigureCoreSelect(GPIO_PORTB_BASE, 0xCF, 0x01);// C28 controls PB 0-3, 6+7 -- PWM, Fehlerpins
	GPIOPinConfigureCoreSelect(GPIO_PORTC_BASE, 0xC0, 0x01);// C28 controls PC 6+7 -- LED 2+3

	GPIOPinConfigureCoreSelect(GPIO_PORTD_BASE, 0xF0, 0x01);// C28 controls PD 0-7 / GPIO 16-23 -- eQEP etc.
	GPIOPinConfigureCoreSelect(GPIO_PORTE_BASE, 0x30, GPIO_PIN_C_CORE_SELECT);// C28 controls PE 4-5 / GPIO 28-29 -- SCI for Hiperface //SW: 13.05.2016

	GPIOPinConfigureCoreSelect(GPIO_PORTH_BASE, 0x04, 0x01);// C28 controls PH 2 / GPIO50 - iso. Dig In (X21)
	//GPIOPinConfigureCoreSelect(GPIO_PORTF_BASE, 0x04, 0x01); // C28 controls PF 2 / GPIO34 - iso. Dig In (X22)
	GPIOPinConfigureCoreSelect(GPIO_PORTF_BASE, 0x06, GPIO_PIN_C_CORE_SELECT);// C28 controls PF 1+2 / GPIO34 - iso. Dig In (X22) und GPIO33 - SCI for Hiperface //SW: 13.05.2016
#endif

	RAMMReqSharedMemAccess(S6_ACCESS, C28_MASTER);//Schreibberechtigung für C28 in den Shared Memeory Bereich S6

	System_printf("\nStarting the m3\n");
	/* SysMin will only print to the console when you call flush or exit */
	System_flush();

	// Wake up Control Subsystem
	while ((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCBOOTSTS) & 0xFFFF)
			!= CBROM_BOOTSTS_CTOM_CONTROL_SYSTEM_READY)
		;
	master_ram_init_control_m0m1_msgram_memories();
	master_ram_init_control_L0_L4_memories();
	IPCMtoCBootControlSystem(CBROM_MTOC_BOOTMODE_BOOT_FROM_FLASH);

	/*
	 *  Create the Task that farms out incoming TCP connections.
	 *  arg0 will be the port that this task listens to.
	 */
	Task_Params_init(&taskParams);
	Error_init(&eb);

#if (PLATFORM == PLAT_STEUER)// Is for the "Schieberegister", which is only in use with the "Steuerplatine"
	taskParams.stackSize = 1024;
	taskParams.priority = 1;
	taskParams.instance->name = "spiTask";
	taskHandle = Task_create((Task_FuncPtr)spiTask, &taskParams, &eb);
	if (taskHandle == NULL) {
		System_printf("main: Failed to create tcpHandler Task\n");

	}
#endif //(PLATFORM == PLAT_STEUER)

#if (PLATFORM == PLAT_OHMrichter)
	/* ================ Start SPI-Task configuration ================ */
	/* Create SPI-Master-task with priority 1 */
	Task_Params_init(&taskParams);
	taskParams.stackSize = 1024;
	taskParams.priority = 1;
	taskParams.instance->name = "masterTask";
	taskHandle = Task_create((Task_FuncPtr) masterTaskFxn, &taskParams, &eb);
	/* ================ End SPI-Task configuration ================ */
#endif //(PLATFORM == OHMrichter)

	System_printf("Starting the example\nSystem provider is set to SysMin. "
			"Halt the target and use ROV to view output.\n");
	/* SysMin will only print to the console when you call flush or exit */
	System_flush();

	/* Start BIOS */
	BIOS_start();

	return (0);
}
//================================ END MAIN ===================================
//=============================================================================

//============== INTERRUPT FOR INTER-PROCESSOR-COMMUNICATION ==================
//=============================================================================
void isr_ipc() {
	int ii = 0;

	c28_to_m3_shadow.status_ctom = c28_to_m3_shared_data.status_ctom;
	c28_to_m3_shadow.schiebereg_ausgaenge =
			c28_to_m3_shared_data.schiebereg_ausgaenge;

	for (ii = 0; ii < 20; ii++) {
		c28_to_m3_shadow.val[ii] = c28_to_m3_shared_data.val[ii];
	}

	c28_to_m3_shadow.slowDataContent = c28_to_m3_shared_data.slowDataContent;
	c28_to_m3_shadow.slowDataID = c28_to_m3_shared_data.slowDataID;

	Semaphore_post(semaphore_send);

	IPCCtoMFlagAcknowledge(IPC_FLAG3);
}
//============== END INTERRUPT FOR INTER-PROCESSOR-COMMUNICATION ==============
//=============================================================================

//=============================== AUX FUNCTIONS ===============================
//=============================================================================
void idle_send() {
}

void master_ram_init_control_m0m1_msgram_memories() {
	unsigned int ii = 0;

	//RAM INIT for M1, and CTOM MsgRAM - M0 RAM INIT is done by C-BootROM
	//#define CCORE_M0M1_CTOM_MSG_RAM_INIT_REG_ADDR 0x4920
	//#define CCORE_M0_RAM_INIT_BIT     0x01
	//#define CCORE_M1_RAM_INIT_BIT 0x04
	//#define CCORE_CTOM_MSG_RAM_INIT_BIT 0x10
	IPCLiteMtoCSetBits_Protected(IPC_FLAG1, 0x4920, 0x14, IPC_LENGTH_32_BITS,
	IPC_FLAG32);

	//wait until C-BootROM acks

	while (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG1)
		;

	//CHECK IF pass or fail
	if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG32) {
		//still set - so command failed.
		//error - blink SLOOOOOOOOWWW
		while (1) {
			//if you are here, this is bad
		}
	}

	//WAIT for RAM_INIT done of above.
	//GIVE some cycles delay until CCORE performs RAM INIT
	for (ii = 0; ii < 2048; ii++)
		;

	//READ RAM_INIT_DONE register
	//#define CCORE_M0M1_CTOM_MSG_RAM_INIT_DONE_REG_ADDR  0x4930
	//#define CCORE_M0_RAM_INIT_DONE_BIT      0x01
	//#define CCORE_M1_RAM_INIT_DONE_BIT 0x04
	//#define CCORE_CTOM_MSG_RAM_INIT_DONE_BIT 0x10

	do {
		IPCLiteMtoCDataRead(IPC_FLAG1, 0x4930, IPC_LENGTH_32_BITS, IPC_FLAG32);

		//wait until C-BootROM acks
		while (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG1)
			;

		//CHECK IF pass or fail
		if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG32) {
			//still set - so command failed.
			while (1) {
				//if you are here, this is bad
			}

		} else {
			if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAR) & 0x14) {
				//RAM_INIT completed - do nothing :-)
				break;
			} else {
				//RAM_INIT not done yet, so wait for more time and read ram init done register.
				//GIVE some cycles delay until CCORE performs RAM INIT
				for (ii = 0; ii < 2048; ii++)
					;
				continue;
			}
		}
	} while (1);
}

void master_ram_init_control_L0_L4_memories() {
	unsigned int ii = 0;

	//RAM INIT for L0,L1,L2,L3
	//#define CCORE_L0L4_RAM_INIT_REG_ADDR    0x4922
	//#define CCORE_L0_RAM_INIT_BIT     0x01
	//#define CCORE_L1_RAM_INIT_BIT 0x04
	//#define CCORE_L2_RAM_INIT_BIT 0x10
	//#define CCORE_L3_RAM_INIT_BIT 0x40
	IPCLiteMtoCSetBits_Protected(IPC_FLAG1, 0x4922, 0x55, IPC_LENGTH_32_BITS,
	IPC_FLAG32);

	//wait until C-BootROM acks
	while (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG1)
		;

	//CHECK IF pass or fail
	if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG32) {
		//still set - so command failed.
		while (1) {
			//if you are here, this is bad;
		}

	}

	//WAIT for RAM_INIT done of above.
	//GIVE some cycles delay until CCORE performs RAM INIT
	for (ii = 0; ii < 2048; ii++)
		;

	//READ RAM_INIT_DONE register
	//#define CCORE_L0_L4_RAM_INIT_DONE_REG_ADDR    0x4932
	//#define CCORE_L0_RAM_INIT_DONE_BIT      0x01
	//#define CCORE_L1_RAM_INIT_DONE_BIT 0x04
	//#define CCORE_L2_RAM_INIT_DONE_BIT 0x10
	//#define CCORE_L3_RAM_INIT_DONE_BIT 0x40

	do {
		IPCLiteMtoCDataRead(IPC_FLAG1, 0x4932, IPC_LENGTH_32_BITS, IPC_FLAG32);

		//wait until C-BootROM acks
		while (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG1)
			;

		//CHECK IF pass or fail
		if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG32) {
			//still set - so command failed.
			while (1) {
				//if you are here, this is bad
			}

		} else {
			if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAR) & 0x55) {
				//RAM_INIT completed - do nothing :-)
				break;
			} else {
				//RAM_INIT not done yet, so wait for more time and read ram init done register.
				//GIVE some cycles delay until CCORE performs RAM INIT
				for (ii = 0; ii < 2048; ii++)
					;
				continue;
			}
		}
	} while (1);

}
//=============================== END AUX FUNCTIONS ===========================
//=============================================================================

