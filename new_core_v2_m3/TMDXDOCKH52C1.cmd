/*
 * Copyright (c) 2013, Texas Instruments Incorporated
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
 *  ======== TMDXDOCKH52C1.cmd ========
 *  Define the memory block start/length for the F28M35H52C1 M3
 */

MEMORY
{
//von Stefan
    BOOTROM (RX)    : origin = 0x0,        length = 0x10000
    FLASH_BOOT (RWX): origin = 0x200030,   length = 0x4
    FLASH (RWX)     : origin = 0x200034,   length = 0x7FF9C
    C03SRAM (RWX)   : origin = 0x20000000, length = 0x8000
    //S0 RAM (Parity, Shared) - S2 RAM (Parity, Shared) 2000 8000 - 2000 DFFF (S. 16 von Concerto Datasheet)
    S07SHRAM_1 (RWX): origin = 0x20008000, length = 0x6000
 	//S3 RAM (Parity, Shared) - S5 RAM (Parity, Shared) 2000 E000 – 2001 3FFF (S. 16 von Concerto Datasheet)
    // wird vom C2000 für Parameteridentifikationslauf genutzt !!!
    S07SHRAM_2 (RWX): origin = 0x2000E000, length = 0x6000
    //S6 RAM (Parity, Shared) - S7 RAM (Parity, Shared) 2001 4000 – 2001 7FFF (S. 16 von Concerto Datasheet)
    SO_RAM1 (R)		: origin = 0x20014000, length = 0x1018  	//SW: Only necessary for Ethernet-Control ans SoftOszi
    CTOM_SHARED (R)	: origin = 0x20015018, length = 0x100		//MH - 28.06.2016 necessary for Sitara-communication
    S07SHRAM_3 (RWX): origin = 0x20015118, length = 0x2EE8 //0x2EE8
    //CtoM MSG RAM (Parity) 2007 F000 – 2007 F7FF (S. 16 von Concerto Datasheet)
    CTOMMSGRAM (R)  : origin = 0x2007F000, length = 0x800 //0x800
  	//MtoC MSG RAM (Parity) 2007 F800 – 2007 FFFF (S. 16 von Concerto Datasheet)
    SO_Metadata (RW): origin = 0x2007F800, length = 0x3C  //0x28		//SW: Only necessary for Ethernet-Control ans SoftOszi
    MTOC_SHARED (RW): origin = 0x2007F83C, length = 0xEC  //0x100		//MH - 28.06.2016 necessary for Sitara-communication
    MTOCMSGRAM (RW) : origin = 0x2007F928, length = 0x6D8
}

SECTIONS
{
    /* Allocate program areas: */
    .text       : > FLASH
    .binit      : > FLASH
    .cinit      : > FLASH
    .pinit      : > FLASH

    /* Initialized sections go in Flash */
    .const      : > FLASH

    /* Allocate uninitalized data sections: */
    .data       		: > S07SHRAM_1
    .bss        		: >> C03SRAM | S07SHRAM_1
    .dma        		: > S07SHRAM_1
    .sysmem     		: > C03SRAM
    .stack      		: > S07SHRAM_1
    .cio        		: > C03SRAM
    .neardata   		: > C03SRAM
    .rodata     		: > C03SRAM
    .args       		: > C03SRAM
    .c28x_Data1  		: > SO_RAM1   		//SW: Only necessary for Ethernet-Control and SoftOszi
    .c28_to_m3_shared 	: > CTOM_SHARED		//MH: Shared data for Java or Industriel-Ethernet Applications
    .c28x_Meta  		: > SO_Metadata		//SW: Only necessary for Ethernet-Control and SoftOszi
    .m3_to_c28_shared 	: > MTOC_SHARED		//MH: Shared data for Java or Industriel-Ethernet Applications
}

__STACK_TOP = __stack + 256;
