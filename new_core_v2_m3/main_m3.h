/*
 *  ======== main_m3.h ========
 */

#ifndef __MAIN_M3_H
#define __MAIN_M3_H

#ifdef __cplusplus
extern "C" {
#endif

//====================== SELECT ETHERNET APPLICATION ==========================
//=============================================================================
#define	  JAVA					20			// PC based Java applications
#define	  INDUSTRIAL_ETHERNET	30			// Profinet/Ethercat etc.

//Choose ethernet application
#define ETHERNET_APP			JAVA

//====================== DEFINE NETWORK SEND FIELD SIZE =======================
//=============================================================================
#define NETWORK_SEND_FIELD_SIZE 15 //Default: 15
#define TCPPACKETSIZE 1460 //Maximum TCPPaketSize -> Default: 1460
//-> Jumbo-Frames would enable a TCPPACKETSIZE of 8960
#define TCPPORT 1000	 //arbitrarily defined, defined again in tcpEchoHooks.c
#define NUMTCPWORKERS 3

//====================== DEFINITIONS OF DATA STRUCTURES =======================
//=============================================================================
#if (ETHERNET_APP == JAVA)

typedef struct
{
	Uint32 status;
	Uint32 slowDataContent[NETWORK_SEND_FIELD_SIZE];
	Int16 val_01_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_02_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_03_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_04_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_05_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_06_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_07_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_08_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_09_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_10_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_11_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_12_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_13_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_14_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_15_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_16_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_17_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_18_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_19_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 val_20_uint16[NETWORK_SEND_FIELD_SIZE];
	Int16 slowDataID[NETWORK_SEND_FIELD_SIZE];
} NetworkSendStruct;

#endif //(ETHERNET_APP == JAVA)

// ~~ ########### ~~ PROTOTYPES ~~ ########### ~~
void master_ram_init_control_m0m1_msgram_memories(void);
void master_ram_init_control_L0_L4_memories(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_M3_H */
