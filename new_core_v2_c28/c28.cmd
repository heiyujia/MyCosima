/*
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== demo_c28.cmd ========
 *  Define the memory block start/length for the TMDXDOCKH52C1 C28
 */

MEMORY
{
PAGE 0:    /* Program Memory */

    FLASH       : origin = 0x100000, length = 0x3FF80
    BEGIN       : origin = 0x13FFF0, length = 0x2
    
PAGE 1 :   /* Data Memory */

	//M0 RAM (ECC) - M1 RAM (ECC)  0000 0000 – 0000 07FF (S. 13 von Concerto Datasheet)
    M01SARAM    : origin = 0x0,      length = 0x800     /* on-chip RAM block M0, M1 */
    //PIEVECT     : origin = 0xD00,    length = 0x100
    //L0 RAM (ECC, Secure) - L3 RAM (ECC, Secure) 0000 8000 - 0000 BFFF  (S. 16 von Concerto Datasheet)
    L03SARAM    : origin = 0x8000,   length = 0x4000    /* on-chip RAM block L0-L3 */
    //S6 RAM (Parity, Shared) - S7 RAM (Parity, Shared) 0001 2000 – 0001 3FFF (S. 16 von Concerto Datasheet)
	SO_RAM1		: origin = 0x12000,   length = 0x80C	/* RAM 1 for Ethernetcommunication and SoftOszi */
  	CTOM_SHARED : origin = 0x1280C,   length = 0x80   	//MH - 28.06.2016 necessary for Sitara-communication
    S07SHRAM    : origin = 0x1288C,   length = 0x1774 //0x980 //0x74E 	//SW available shared memory for other applications
	//CtoM MSG RAM (Parity) 0003 F800 – 0003 FBFF (S. 16 von Concerto Datasheet)
    CTOMMSGRAM  : origin = 0x3F800,  length = 0x400  //0x400
   	//MtoC MSG RAM (Parity) 0003 FC00 – 0003 FFFF (S. 16 von Concerto Datasheet)
    MTOC_SO_Meta  : origin = 0x3FC00,  length = 0x01E	/* MetaData from M3 to c28x for Ethernetcommunication and SoftOszi */
    MTOC_SHARED : origin = 0x3FC1E,   length = 0x76   	//MH - 28.06.2016 necessary for Sitara-communication
    MTOCMSGRAM  : origin = 0x3FC94,  length = 0x36C
}

SECTIONS
{
    /* Allocate program areas: */
    .text       : > FLASH   PAGE = 0
    .cinit      : > FLASH   PAGE = 0
    .pinit      : > FLASH   PAGE = 0
    .binit      : > FLASH   PAGE = 0
    ramfuncs    : LOAD = FLASH      PAGE = 0,
                  RUN  = L03SARAM   PAGE = 1,
                  LOAD_START(_RamfuncsLoadStart),
                  LOAD_SIZE(_RamfuncsLoadSize),
                  LOAD_END(_RamfuncsLoadEnd),
                  RUN_START(_RamfuncsRunStart)

    /* Initalized sections go in Flash */
    .econst     : > FLASH   PAGE = 0
    .switch     : > FLASH   PAGE = 0
    .args       : > FLASH   PAGE = 0

    /* Allocate uninitalized data sections: */
    .stack      : > M01SARAM | L03SARAM PAGE = 1
    .ebss       : > M01SARAM | L03SARAM PAGE = 1
    .esysmem    : > L03SARAM | M01SARAM PAGE = 1
    .cio        : > L03SARAM | M01SARAM PAGE = 1
    .c28x_Ram1  : > SO_RAM1      			//SW: Necessary for Ethernet-Control and SoftOszi
    .c28_to_m3_shared : > CTOM_SHARED //MH - 28.06.2016 necessary for Sitara-communication
    .M3_meta  	: > MTOC_SO_Meta 			//SW: Necessary for Ethernet-Control and SoftOszi
	.m3_to_c28_shared : > MTOC_SHARED //MH - 28.06.2016 necessary for Sitara-communication
    codestart : > BEGIN	PAGE = 0
}
