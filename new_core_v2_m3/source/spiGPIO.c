/*
 * spiGPIO.c
 *
 *  Created on: 15.10.2013
 *      Author: Philipp Löhdefink
 */


#include <inc/hw_types.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <ti/sysbios/knl/Task.h>
#include "inc/hw_memmap.h"
#include <ti/drivers/SPI.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>

#include "inc/hw_ipc.h"
#include "driverlib/ipc.h"

#include <ti/drivers/GPIO.h>

#include "../include/Board.h"
#include "../main_m3.h"
#include "../include/spiGPIO.h"
#include "../../new_core_v2_c28/main_c28.h"


UShort masterRxBuffer[SPI_MSG_LENGTH] = {0x0000};
UShort masterTxBuffer[SPI_MSG_LENGTH] = {0x0000};

extern c28_to_m3_shared_struct c28_to_m3_shadow;
extern m3_to_c28_shared_struct m3_to_c28;


/* Allocate buffers in .dma section of memory for concerto devices */
#ifdef MWARE
#pragma DATA_SECTION(masterRxBuffer, ".dma");
#pragma DATA_SECTION(masterTxBuffer, ".dma");
#endif


void spiGPIO_init()
{
	SysCtlPeripheralEnable(0x10000080);	// SSI3
	GPIOPinConfigure(GPIO_PE2_SSI3CLK);
	//GPIOPinConfigure(GPIO_PE3_SSI3FSS);
	GPIOPinConfigure(GPIO_PE1_SSI3RX);
	GPIOPinConfigure(GPIO_PE0_SSI3TX);
	GPIOPinTypeSSI(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 |
									GPIO_PIN_2 /*| GPIO_PIN_3*/);

	SPI_init();
}

Void spiTask(UArg arg0, UArg arg1)
{
	SPI_Handle masterSpi;
	SPI_Transaction masterTransaction;
	SPI_Params params;

	SPI_Params_init(&params);
	params.dataSize = 16;

	/* Initialize SPI handle as default master */
	masterSpi = SPI_open(TMDXDOCKH52C1_SPI3, &params);
	if (masterSpi == NULL) {
		System_abort("Error initializing SPI\n");
	}
	else {
		System_printf("SPI initialized\n");
	}
	Task_sleep(4);

	while (1)
	{
#if(PLATFORM == STEUER)
		if (c28_to_m3_shadow.schiebereg_ausgaenge == (1 << 2)) // Vorladeschuetz ein
			spiGPIO_setRelais(5, 1);
		else
			spiGPIO_setRelais(5, 0);

		if (c28_to_m3_shadow.schiebereg_ausgaenge == (1 << 5)) // Netzschuetz ein
		{
			spiGPIO_setRelais(6, 1);	// Netz-Schuetz
			spiGPIO_setRelais(4, 1); 	// Luefter
			spiGPIO_setRelais(3, 1); 	// InstaSPIN-Motorschuetz
		}
		else
		{
			spiGPIO_setRelais(6, 0);	// Netz-Schuetz
			spiGPIO_setRelais(4, 0);	// Luefter
			spiGPIO_setRelais(3, 0); 	// InstaSPIN-Motorschuetz
		}

		GPIO_write(SCHIEBEREGISTER_ENABLE_IN, 0);
		asm(" nop");
		GPIO_write(SCHIEBEREGISTER_ENABLE_IN, ~0);
#endif
		/* Initialize master SPI transaction structure */
		masterTransaction.count = SPI_MSG_LENGTH;
		masterTransaction.txBuf = (Ptr)masterTxBuffer;
		masterTransaction.rxBuf = (Ptr)masterRxBuffer;

		/* Initiate SPI transfer */
		SPI_transfer(masterSpi, &masterTransaction);

		GPIO_write(SCHIEBEREGISTER_ENABLE_OUT, ~0);
		asm(" nop");

		GPIO_write(SCHIEBEREGISTER_ENABLE_OUT, 0);

		Task_sleep(100);	//10ms - 100x100us
	}

	/* Deinitialize SPI */
	SPI_close(masterSpi);

	System_printf("Done\n");
}

Int8 spiGPIO_getInput(UInt8 number)
{
	UInt8 no = 0;
	if (number < 1 || number > 16)	return -1;	// Fehler!

	if (number <= 8)	// DI1-8, Bits 8-15 am SPI
		no = number + 7;
	else				// DI9-16, Bits 0-7 am SPI
		no = number - 9;
	if (masterRxBuffer[0] & (1 << no))	return 1;
	else return 0;
}

Int8 spiGPIO_setOutput(UInt8 number, UInt8 zustand)
{
	if (number < 1 || number > 6 || zustand > 1)	return -1;	// Fehler!
	if (zustand == 1)	// setze Ausgang auf "eingeschaltet"
		{
			masterTxBuffer[0] |= (1 << (number+7));
		}
		else if (zustand == 0)	// setze Ausgang auf "ausgeschaltet"
		{
			masterTxBuffer[0] &= ~(1 << (number+7));
		}
		return 0;
}

Int8 spiGPIO_setRelais(UInt8 number, UInt8 zustand)
{
	if (number < 1 || number > 6 || zustand > 1)	return -1;	// Fehler!
	if (zustand == 1)	// setze Relais auf "eingeschaltet"
	{
		masterTxBuffer[0] |= (1 << (number-1));
	}
	else if (zustand == 0)	// setze Relais auf "ausgeschaltet"
	{
		masterTxBuffer[0] &= ~(1 << (number-1));
	}
	return 0;
}
