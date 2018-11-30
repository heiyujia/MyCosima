/*
 * SPI_Master_Slave.h
 *
 *  Created on: 27.01.2015
 *      Author: Sebastian Wendel
 */

#ifndef SPI_Master_Slave_H_
#define SPI_Master_Slave_H_


// Number of bytes to send and receive.
#define SPI_MSG_LENGTH_IND_ETHERNET		 26

#if (ETHERNET_APP == INDUSTRIAL_ETHERNET)

#endif

//für SPI Initialisieren
void SPI_Master_Slave_initSPI();

#endif /* SPI_Master_Slave_H_ */
