/*
 * SPI_Master_Slave.c
 *
 *  Created on: 27.01.2015
 *      Author: Sebastian Wendel
 * Description: This Task is for SPI-Master, SPI-Slave or both of them.
 * 				You have to set the right SSI (SPI) to the right GPIO in the "SPI_Master_Slave_initSPI".
 * 				The Master-Task is necessary if you want to use the board as SPI-Master.
 * 				The Slave-Task is necessary if you want to use the board as SPI-Slave.
 * 				You have to start the Tasks in the main.c and you have to call the
 * 				"SPI_Master_Slave_initSPI" at the beginning of the main.c
 * 				Don't forget "extern Void masterTaskFxn(UArg arg0, UArg arg1);" or
 * 				"extern Void slaveTaskFxn(UArg arg0, UArg arg1);" at the beginning of main.c
 * 				The cycle-time to send the telegram is set with "Task_sleep(time in micro seconds);"
 *
 * 				!!!!!!Achtung!!!!!!! da nicht genügend GPIO für die SPI-Ausgänge (SSI0-SSI3) zur Verfügung stehen, könnten
 * 				 zwar an der Docking-Station mehrere SPI verwendet werden, jedoch am der Umrichterplatine nur ein SPI.
 * 				 Hier wurde "PD0_GPIO16" - "PD3_GPIO19" mit SSI0 belegt. Wenn in der main.c der PortA oder in spiGPIO.c
 * 				 der PortE nicht belegt werden, können auch diese für SPI verwendet werden!
 *
 *				Falls die Daten bei mehreren Byte "verrutschen" bzw. falsche Zahlen ankommen, liegt es daran das der Mastertakt
 *				zu schnell ist, bzw. die Pause zwischen den Telegrammen zu klein!
 *
 **/

/* XDCtools Header files */
#include "../include/SPI_Master_Slave.h" //SW 26.01.2015

#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <inc/hw_gpio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_sysctl.h>
#include <inc/hw_types.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/ssi.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>

/* Example/Board Header files */
#include "../main_m3.h"
#include "../../new_core_v2_c28/main_c28.h"
#include "driverlib/ipc.h"

#include "../include/Board.h"
/* *************** */









uint8_t i = 0;		//Zählvariable für for-Schleife

int slaveSpiRxBufferINT[SPI_MSG_LENGTH_IND_ETHERNET];
int LivecheckSPI_Concerto = 0;





/*Initialisierung -> Anpassen jenachdem welche GPIO's genutzt werden sollen
 *  ======== SPI_Master_Slave_initSPI ========
 */
Void SPI_Master_Slave_initSPI(Void)
{
    //independent from Master or Slave
	// SSI0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//GPIO port D needs to be enabled so these pins can be used.

    GPIOPinConfigure(GPIO_PD0_SSI0TX);
    GPIOPinConfigure(GPIO_PD1_SSI0RX);
    GPIOPinConfigure(GPIO_PD2_SSI0CLK);
    GPIOPinConfigure(GPIO_PD3_SSI0FSS);

    GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 |
                                    GPIO_PIN_2 | GPIO_PIN_3);

    // SSI1
    ///* Wird nicht benutzt, da nur ein Port hardwareseitig zur Verfügung steht und dieser aktuell für SLave verwendet wird.
  /*  SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//GPIO port E needs to be enabled so these pins can be used.

    GPIOPinConfigure(GPIO_PE0_SSI1CLK);
    GPIOPinConfigure(GPIO_PE1_SSI1FSS);
    GPIOPinConfigure(GPIO_PE2_SSI1RX);
    GPIOPinConfigure(GPIO_PE3_SSI1TX);

    GPIOPinTypeSSI(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 |
                                    GPIO_PIN_2 | GPIO_PIN_3);
    //TMDXDOCKH52C1_initDMA();*/
    System_printf("SPI_Master_Slave.c: Init SPI-Port successful\n");
    SPI_init();
}


/*
 *  ======== slaveTaskFxn ========
 *  Task function for slave task.
 *
 *  This task runs on a higher priority, since slave
 *  has to be ready for master. Slave SPI sends a
 *  message to master and also receives message from
 *  master. Task for this function is created
 *  statically. See the project's .cfg file.
 */


//Void slaveTaskFxn (UArg arg0, UArg arg1)
//{
//    SPI_Handle slaveSpi;
//    SPI_Params slaveSpiParams;
//    SPI_Transaction slaveTransaction;
//    UInt transferOK;
//
//    SSIEnable(SSI0_BASE);
//    System_printf("SPI_Master_Slave.c: First time SPI-Task successful\n");
//
//    while(1)
//    {
//		SPI_init();
//		/* Initialize SPI handle with slave mode */
//		SPI_Params_init(&slaveSpiParams);
//		slaveSpiParams.transferMode = SPI_MODE_BLOCKING;
//		slaveSpiParams.transferCallbackFxn = NULL;
//		slaveSpiParams.mode = SPI_SLAVE;
//		slaveSpiParams.bitRate = 1000000;
//		slaveSpiParams.dataSize = 8;
//		slaveSpiParams.frameFormat = SPI_POL0_PHA0;
//		slaveSpi = SPI_open(Board_SPI0, &slaveSpiParams);
//
//		/* Initialize slave SPI transaction structure */
//		slaveTransaction.count = SPI_MSG_LENGTH_IND_ETHERNET;
//		slaveTransaction.txBuf = (Ptr)slaveSpiTxBuffer;
//		slaveTransaction.rxBuf = (Ptr)slaveSpiRxBuffer;
//
//		/* DATEN VOM C28 FÜR SPI ÜBERTRAGUNG VORBEREITEN - MH 30.06.2016 */
//		ConcertoToSitaraSPI[0] 	= c28_to_m3_shared_FIKAT_shadow.status;
//		ConcertoToSitaraSPI[1] 	= c28_to_m3_shared_FIKAT_shadow.fehler;
//		ConcertoToSitaraSPI[2] 	= c28_to_m3_shared_FIKAT_shadow.I_dHHbyte;
//		ConcertoToSitaraSPI[3] 	= c28_to_m3_shared_FIKAT_shadow.I_dLHbyte;
//		ConcertoToSitaraSPI[4] 	= c28_to_m3_shared_FIKAT_shadow.I_dHLbyte;
//		ConcertoToSitaraSPI[5] 	= c28_to_m3_shared_FIKAT_shadow.I_dLLbyte;
//		ConcertoToSitaraSPI[6] 	= c28_to_m3_shared_FIKAT_shadow.I_qHHbyte;
//		ConcertoToSitaraSPI[7] 	= c28_to_m3_shared_FIKAT_shadow.I_qLHbyte;
//		ConcertoToSitaraSPI[8] 	= c28_to_m3_shared_FIKAT_shadow.I_qHLbyte;
//		ConcertoToSitaraSPI[9] 	= c28_to_m3_shared_FIKAT_shadow.I_qLLbyte;
//		ConcertoToSitaraSPI[10] = c28_to_m3_shared_FIKAT_shadow.U_zkHHbyte;
//		ConcertoToSitaraSPI[11] = c28_to_m3_shared_FIKAT_shadow.U_zkLHbyte;
//		ConcertoToSitaraSPI[12] = c28_to_m3_shared_FIKAT_shadow.U_zkHLbyte;
//		ConcertoToSitaraSPI[13] = c28_to_m3_shared_FIKAT_shadow.U_zkLLbyte;
//		ConcertoToSitaraSPI[14] = c28_to_m3_shared_FIKAT_shadow.n_IstHHbyte;
//		ConcertoToSitaraSPI[15] = c28_to_m3_shared_FIKAT_shadow.n_IstLHbyte;
//		ConcertoToSitaraSPI[16] = c28_to_m3_shared_FIKAT_shadow.n_IstHLbyte;
//		ConcertoToSitaraSPI[17] = c28_to_m3_shared_FIKAT_shadow.n_IstLLbyte;
//		ConcertoToSitaraSPI[18] = c28_to_m3_shared_FIKAT_shadow.M_IstHHbyte;
//		ConcertoToSitaraSPI[19] = c28_to_m3_shared_FIKAT_shadow.M_IstLHbyte;
//		ConcertoToSitaraSPI[20] = c28_to_m3_shared_FIKAT_shadow.M_IstHLbyte;
//		ConcertoToSitaraSPI[21] = c28_to_m3_shared_FIKAT_shadow.M_IstLLbyte;
//		ConcertoToSitaraSPI[22] = c28_to_m3_shared_FIKAT_shadow.pos_IstHHbyte;
//		ConcertoToSitaraSPI[23] = c28_to_m3_shared_FIKAT_shadow.pos_IstLHbyte;
//		ConcertoToSitaraSPI[24] = c28_to_m3_shared_FIKAT_shadow.pos_IstHLbyte;
//		ConcertoToSitaraSPI[25] = c28_to_m3_shared_FIKAT_shadow.pos_IstLLbyte;
//
//		/* DATEN ZUM ABHOLEN IM SPI PUFFER BEREITSTELLEN - MH 30.06.2016 */
//		for(i=0; i < SPI_MSG_LENGTH_IND_ETHERNET; i++) {
//					slaveSpiTxBuffer[i] = (uint8_t)ConcertoToSitaraSPI[i]; //Hier passiert cast von Uint16 auf uint8
//					}
//
//    	//slaveSpiTxBuffer[9] = LivecheckSPI_Concerto;
//		/* Initiate SPI transfer */
//		transferOK = SPI_transfer(slaveSpi, &slaveTransaction);
//
//		if(transferOK) {
//
//			/* AUSLESEN DER DATEN VOM SITARA AUS DEM SPI PUFFER */
//			for(i=0; i < SPI_MSG_LENGTH_IND_ETHERNET; i++) {
//			SitaraToConcertoSPI[i] = slaveSpiRxBuffer[i];
//			}
//
//			/* DATEN VOM SITARA IN DEN SHARED RAM ZUM C28 SCHREIBEN - MH 27.06.2016 */
//			m3_to_c28_shared_FIKAT.vorgabe 			= SitaraToConcertoSPI[0];
//			m3_to_c28_shared_FIKAT.n_SollHighbyte 	= SitaraToConcertoSPI[1];
//			m3_to_c28_shared_FIKAT.n_SollLowbyte 	= SitaraToConcertoSPI[2];
//			m3_to_c28_shared_FIKAT.M_SollHighbyte 	= SitaraToConcertoSPI[3];
//			m3_to_c28_shared_FIKAT.M_SollLowbyte	= SitaraToConcertoSPI[4];
//			m3_to_c28_shared_FIKAT.pos_SollHighbyte	= SitaraToConcertoSPI[5];
//			m3_to_c28_shared_FIKAT.pos_SollLowbyte	= SitaraToConcertoSPI[6];
//			m3_to_c28_shared_FIKAT.gewichtung1		= SitaraToConcertoSPI[7];
//			m3_to_c28_shared_FIKAT.gewichtung2		= SitaraToConcertoSPI[8];
//			m3_to_c28_shared_FIKAT.gewichtung3		= SitaraToConcertoSPI[9];
//			m3_to_c28_shared_FIKAT.reserve1			= SitaraToConcertoSPI[10];
//			m3_to_c28_shared_FIKAT.reserve2			= SitaraToConcertoSPI[11];
//
//			SitaraToConcertoSPI[12]					= 0;	//Freie Plätze mit Dummys auffüllen
//			SitaraToConcertoSPI[13]					= 0;
//			SitaraToConcertoSPI[14]					= 0;
//			SitaraToConcertoSPI[15]					= 0;
//			SitaraToConcertoSPI[16]					= 0;
//			SitaraToConcertoSPI[17]					= 0;
//			SitaraToConcertoSPI[18]					= 0;
//			SitaraToConcertoSPI[19]					= 0;
//			SitaraToConcertoSPI[20]					= 0;
//			SitaraToConcertoSPI[21]					= 0;
//			SitaraToConcertoSPI[22]					= 0;
//			SitaraToConcertoSPI[23]					= 0;
//			SitaraToConcertoSPI[24]					= 0;
//			SitaraToConcertoSPI[25]					= 0;
//
//			IPCMtoCFlagSet(IPC_FLAG4); //Flag im C28 setzen, dass neue Daten vom M3 verfügbar //SW: geändert auf IPC_FLAG4 wegen SoftOszi
//		}
//		else {
//			System_printf("Unsuccessful slave SPI transfer");
//		}
//
//		SPI_close(slaveSpi);
//    }
//    /* Deinitialize SPI */
//    SPI_close(slaveSpi);
//}
/*
 *  ======== masterTaskFxn ========
 *  Task function for master task.
 *
 *  This task runs at a lower priority after the slave
 *  task to ensure it is ready for a transaction.
 *  Master SPI sends a message to slave and also
 *  receives message from slave. Task for this function
 *  is created statically. See the project's .cfg
 *  file.
 */

Void masterTaskFxn(UArg arg0, UArg arg1) {

#if (ETHERNET_APP == INDUSTRIAL_ETHERNET)
	while (1) {
		SPI_Handle masterSpi;
		SPI_Transaction masterTransaction;
		SPI_Params masterSpiParams;
		UInt transferOK;

		//Anpassung für SteperTreiber DRV8711
		SPI_Params_init(&masterSpiParams);
		masterSpiParams.transferMode = SPI_MODE_BLOCKING;
		masterSpiParams.transferCallbackFxn = NULL;
		masterSpiParams.mode = SPI_MASTER;
		masterSpiParams.bitRate = 1000000;
		masterSpiParams.dataSize = 8;
		masterSpiParams.frameFormat = SPI_POL0_PHA0;


		/* Initialize SPI handle as default master */
		masterSpi = SPI_open(Board_SPI0, &masterSpiParams);
		if (masterSpi == NULL) {
			System_abort("Error initializing SPI\n");
		} else {
			System_printf("SPI Master initialized\n");
		}

		/* Initialize master SPI transaction structure */
		masterTransaction.count = SPI_MSG_LENGTH_IND_ETHERNET;
		masterTransaction.txBuf = (Ptr) masterSpiTxBuffer;
		masterTransaction.rxBuf = (Ptr) masterSpiRxBuffer;
		// Enable the SSI0 module.
		SSIEnable(SSI0_BASE);

		/* Initiate SPI transfer */

		for (i = 0; i < SPI_MSG_LENGTH_IND_ETHERNET; i++) {
			masterSpiTxBuffer[i] = i;
		}
		transferOK = SPI_transfer(masterSpi, &masterTransaction);

		if (transferOK) {
			/* Print contents of master receive buffer */
			//System_printf("Master: %s\n", masterSpiRxBuffer);
		} else {
			//System_printf("Unsuccessful master SPI transfer");
		}
		//System_printf("\nMaster Loop\n");
		//System_flush();
		Task_sleep(500); //5000ms - 50000x100us //this time is responsible, how often the telegram will be send!

		/* Deinitialize SPI */
		SPI_close(masterSpi);

		System_printf("Done\n");

		System_flush();

	}
#endif //(ETHERNET_APP == INDUSTRIAL_ETHERNET)




}


