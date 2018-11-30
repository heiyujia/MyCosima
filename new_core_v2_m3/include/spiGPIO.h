/*
 * spiGPIO.h
 *
 *  Created on: 15.10.2013
 *      Author: Philipp Löhdefink
 */

#ifndef CORE_V2_M3_SPIGPIO_H_
#define CORE_V2_M3_SPIGPIO_H_

#define SPI_MSG_LENGTH 1


void spiGPIO_init();
Int8 spiGPIO_getInput(UInt8 number);
Int8 spiGPIO_setOutput(UInt8 number, UInt8 zustand);
Int8 spiGPIO_setRelais(UInt8 number, UInt8 zustand);

#endif /* CORE_V2_M3_SPIGPIO_H_ */
