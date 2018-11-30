/*
 * qep.c
 *
 *  Created on: 22.04.2015
 *      Author: Matthias
 *      Changed: Sebastian 22.05.2015
 *
 *      Doku siehe am ENde!!!!!!!!!!!!
 */


#include "../include/my_posspeed.h"
#include "../include/platform.h"

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File



void  POSSPEED_Init(void)
{
	//SW: 22.05.2015 QUPRD = PeriodRegister: gibt vergleich zu Controllerfrequenz vor:
	//SW: QUPRD=15000 bei 150 MHz SYSCLKOUT bedeutet alle 10kHz abtasten der Drehzahl
    EQep1Regs.QUPRD=15000; //1500000;         // Unit Timer for 100Hz at 150 MHz SYSCLKOUT = 1500000

    EQep1Regs.QDECCTL.bit.QSRC=00;      // QEP quadrature count mode

    EQep1Regs.QEPCTL.bit.FREE_SOFT=2;
    EQep1Regs.QEPCTL.bit.PCRM=00;       // PCRM=00 mode - QPOSCNT reset on index event
    EQep1Regs.QEPCTL.bit.UTE=1;         // Unit Timeout Enable
    EQep1Regs.QEPCTL.bit.QCLM=1;        // Latch on unit time out
    EQep1Regs.QPOSMAX=0xffffffff; //10000;		SW: ausgetetsets							 // MB 22.04.2015		0xffffffff;
    EQep1Regs.QEPCTL.bit.QPEN=1;        // QEP enable

    //SW: 22.05.2015 QCAPCTL.bit.UPPS gibt an über wie viele Messungen gemittelt wird (Mittelwertbildung um Störungen zu filtern)
    EQep1Regs.QCAPCTL.bit.UPPS=2; //2      // 1/32 for unit position = 5 //SW 18.05.2015 SYSCLKOUT/1 =0
    //SW: 22.05.2015 QCAPCTL.bit.CCPS wird benutzt um die Drehzahl zu skalieren, wegen ^2 skalierung ist jedoch noch oft ein manueler Faktor in der ISR zu verwenden
    EQep1Regs.QCAPCTL.bit.CCPS=2;       // 1/128 for CAP clock = 7 % //SW 18.05.2015 QCLK/1 = 0
    EQep1Regs.QCAPCTL.bit.CEN=1;        // QEP Capture Enable
}

void POSSPEED_Calc(POSSPEED *p)
{
     long tmp;
     unsigned int pos16bval,temp1;
     _iq Tmp1,newp,oldp;
     extern float g_speed;

//**** Position calculation - mechanical and electrical motor angle  ****//
     p->DirectionQep = EQep1Regs.QEPSTS.bit.QDF;    // Motor direction: 0=CCW/reverse, 1=CW/forward

     pos16bval=(unsigned int)EQep1Regs.QPOSCNT;     // capture position once per QA/QB period
     p->theta_raw = pos16bval+ p->cal_angle;        // raw theta = current pos. + ang. offset from QA

     // The following lines calculate p->theta_mech ~= QPOSCNT/mech_scaler [current cnt/(total cnt in 1 rev.)]
     // where mech_scaler = 4000 cnts/revolution
     tmp = (long)((long)p->theta_raw*(long)p->mech_scaler);     // Q0*Q26 = Q26
     tmp &= 0x03FFF000;
     p->theta_mech = (int)(tmp>>11);                // Q26 -> Q15
     p->theta_mech &= 0x7FFF;

     // The following lines calculate p->elec_mech
     p->theta_elec = p->pole_pairs*p->theta_mech;  // Q0*Q15 = Q15
     p->theta_elec &= 0x7FFF;

// Check an index occurrence
     if (EQep1Regs.QFLG.bit.IEL == 1)
     {
        p->index_sync_flag = 0x00F0;
        EQep1Regs.QCLR.bit.IEL=1;                   // Clear interrupt flag
     }


//**** High Speed Calcultation using QEP Position counter ****//
// Check unit Time out-event for speed calculation:
// Unit Timer is configured for 100Hz in INIT function

    if(EQep1Regs.QFLG.bit.UTO==1)                    // If unit timeout (one 100Hz period)
    {
        /** Differentiator  **/
        // The following lines calculate position = (x2-x1)/4000 (position in 1 revolution)
        pos16bval=(unsigned int)EQep1Regs.QPOSLAT;                // Latched POSCNT value
        tmp = (long)((long)pos16bval*(long)p->mech_scaler);       // Q0*Q26 = Q26
        tmp &= 0x03FFF000;
        tmp = (int)(tmp>>11);                                     // Q26 -> Q15
        tmp &= 0x7FFF;
        newp=_IQ15toIQ(tmp);
        oldp=p->oldpos;

        if (p->DirectionQep==0)                     // POSCNT is counting down
        {
            if (newp>oldp)
                Tmp1 = - (_IQ(1) - newp + oldp);    // x2-x1 should be negative
            else
            Tmp1 = newp -oldp;
        }
        else if (p->DirectionQep==1)                // POSCNT is counting up
        {
            if (newp<oldp)
            Tmp1 = _IQ(1) + newp - oldp;
            else
            Tmp1 = newp - oldp;                     // x2-x1 should be positive
        }

        if (Tmp1>_IQ(1))
            p->Speed_fr = _IQ(1);
        else if (Tmp1<_IQ(-1))
            p->Speed_fr = _IQ(-1);
        else
            p->Speed_fr = Tmp1;

        // Update the electrical angle
        p->oldpos = newp;

        // Change motor speed from pu value to rpm value (Q15 -> Q0)
        // Q0 = Q0*GLOBAL_Q => _IQXmpy(), X = GLOBAL_Q
        p->SpeedRpm_fr = _IQmpy(p->BaseRpm,p->Speed_fr);
        //=======================================

        EQep1Regs.QCLR.bit.UTO=1;                   // Clear interrupt flag
    }

//**** Low-speed computation using QEP capture counter ****//
    if(EQep1Regs.QEPSTS.bit.UPEVNT==1)                 // Unit position event
    {
        if(EQep1Regs.QEPSTS.bit.COEF==0)               // No Capture overflow
            temp1=(unsigned long)EQep1Regs.QCPRDLAT;   // temp1 = t2-t1
        else                                           // Capture overflow, saturate the result
            temp1=0xFFFF;

        //p->SpeedScaler = 600; //SW: 22.05.2015  festlegen zur Drehzahlskalierung

        p->Speed_pr = _IQdiv(p->SpeedScaler,(unsigned long)temp1);    // p->Speed_pr = p->SpeedScaler/temp1

        Tmp1=p->Speed_pr;

        if (Tmp1>_IQ(1))
            p->Speed_pr = _IQ(1);
        else
            p->Speed_pr = Tmp1;



        // Convert p->Speed_pr to RPM
        if (p->DirectionQep==0){                           // Reverse direction = negative
        	//g_speed =  -(p->Speed_pr/6000.0)*0.8782 - 14.44667 + 18.607 ;
        	p->SpeedRpm_pr = -_IQmpy(p->BaseRpm,p->Speed_pr);   // Q0 = Q0*GLOBAL_Q => _IQXmpy(), X = GLOBAL_Q
        }else{                                                    // Forward direction = positive
        	//g_speed =  (p->Speed_pr/6000.0)*0.8782 - 14.44667 + 18.607  ;
        	p->SpeedRpm_pr = _IQmpy(p->BaseRpm,p->Speed_pr);    // Q0 = Q0*GLOBAL_Q => _IQXmpy(), X = GLOBAL_Q
        }



        EQep1Regs.QEPSTS.all=0x88;                  // Clear Unit position event flag
                                                    // Clear overflow error flag
    }

}
//SW: Doku zur EInstellung von QEP!

//###########################################################################
//
// FILE:    eqep_pos_speed_c28.c
// TITLE:   EQEP Speed and Position measurement (eqep_pos_speed)
//
//! \addtogroup control_example_list
//! <h1> EQEP Speed and Position measurement (eqep_pos_speed) </h1>
//!
//!    This example requires the following hardware connections from EPWM1 and
//!    GPIO pins (simulating QEP sensor) to QEP peripheral
//!
//! - GPIO20/EQEP1A <- GPIO0/EPWM1A (simulates EQEP Phase A signal)
//! - GPIO21/EQEP1B <- GPIO1/EPWM1B (simulates EQEP Phase B signal)
//! - GPIO23/EQEP1I <- GPIO4 (simulates EQEP Index Signal)
//!
//!    This example will provide position measurement using the capture unit, and
//!    speed measurement using unit time out. This example uses the IQMath library. It is used
//!    merely to simplify high-precision calculations.
//!
//!    See DESCRIPTION in Example_posspeed.c for more details on the calculations
//!    performed in this example.
//!
//! In addition to this file, the following files must be included in this project: \n
//! - Example_posspeed.c - includes all eQEP functions
//! - Example_EPwmSetup.c - sets up EPWM1A and EPWM1B as simulated QA and QB encoder signals
//! - Example_posspeed.h - includes initialization values for pos and speed structure
//!
//! \b Notes:
//! - Maximum speed is configured to 6000rpm(BaseRpm)
//! - Minimum speed is assumed at 10rpm for capture pre-scalar selection
//! - Pole pair is configured to 2 (pole_pairs)
//! - QEP Encoder resolution is configured to 4000counts/revolution (mech_scaler)
//!   which means: 4000/4 = 1000 line/revolution quadrature encoder (simulated by EPWM1)
//! - EPWM1 (simulating QEP encoder signals) is configured for 5kHz frequency or 300 rpm
//!      (=4*5000 cnts/sec * 60 sec/min)/4000 cnts/rev)
//!
//!     \b SPEEDRPM_FR: High Speed Measurement is obtained by counting the QEP input pulses
//!              for 10ms (unit timer set to 100Hz).
//!
//!     \b SPEEDRPM_FR = { (Position Delta)/10ms } * 60 rpm
//!
//!
//!     \b SPEEDRPM_PR: Low Speed Measurement is obtained by measuring time period of QEP edges.
//!              Time measurement is averaged over 64edges for better results and
//!              capture unit performs the time measurement using pre-scaled SYSCLK
//!
//!     Note that pre-scaler for capture unit clock is selected such that
//!     capture timer does not overflow at the required minimum RPM speed
//!
//!  \b Watch \b Variables:
//!  - \b qep_posspeed.SpeedRpm_fr - Speed meas. in rpm using QEP position counter
//!  - \b qep_posspeed.SpeedRpm_pr - Speed meas. in rpm using capture unit
//!  - \b qep_posspeed.theta_mech - Motor mechanical angle (Q15)
//!  - \b qep_posspeed.theta_elec - Motor electrical angle (Q15)
//
//###########################################################################
//
// FILE:    Example_posspeed.c
//
// TITLE:   Pos/speed measurement using EQEP peripheral
//
// DESCRIPTION:
//
// This file includes the EQEP initialization and position and speed calculation
// functions called by Example_F2806xEqep_posspeed.c.  The position and
// speed calculation steps performed by POSSPEED_Calc() at  SYSCLKOUT =  150 MHz are described below:
//
// For 150 MHz Operation:
// ----------------------
//
// 1. This program calculates: **theta_mech**
//
//    theta_mech = QPOSCNT/mech_Scaler = QPOSCNT/4000, where 4000 is the number of
//                 counts in 1 revolution.(4000/4 = 1000 line/rev. quadrature encoder)
//
// 2. This program calculates: **theta_elec**
//
//    theta_elec = (# pole pairs) * theta_mech = 2*QPOSCNT/4000 for this example
//
// 3. This program calculates: **SpeedRpm_fr**
//
//    SpeedRpm_fr = [(x2-x1)/4000]/T                                             - Equation 1
//    Note (x2-x1) = difference in number of QPOSCNT counts. Dividing (x2-x1) by
//    4000 gives position relative to Index in one revolution.
// If base RPM  = 6000 rpm:   6000 rpm = [(x2-x1)/4000]/10ms                     - Equation 2
//                                     = [(x2-x1)/4000]/(.01s*1 min/60 sec)
//                                     = [(x2-x1)/4000]/(1/6000) min
//                         max (x2-x1) = 4000 counts, or 1 revolution in 10 ms
//
//
// If both sides of Equation 2 are divided by 6000 rpm, then:
//                            1 = [(x2-x1)/4000] rev./[(1/6000) min * 6000rpm]
//							Because (x2-x1) must be <4000 (max) for QPOSCNT increment,
//						    (x2-x1)/4000 < 1 for CW rotation
//                          And because (x2-x1) must be >-4000 for QPOSCNT decrement,
//                          (x2-x1)/4000>-1  for CCW rotation
//						    speed_fr = [(x2-x1)/4000]/[(1/6000) min * 6000rpm]
//                                   = (x2-x1)/4000                              - Equation 3
//
// To convert speed_fr to RPM, multiply Equation 3 by 6000 rpm
//                           SpeedRpm_fr = 6000rpm *(x2-x1)/4000                 - Final Equation
//
//
// 2. **min rpm ** = selected at 10 rpm based on CCPS prescaler options available (128 is greatest)
//
// 3. **SpeedRpm_pr**
//    SpeedRpm_pr = X/(t2-t1)                                                    - Equation 4
//    where X = QCAPCTL [UPPS]/4000 rev. (position relative to Index in 1 revolution)
// If max/base speed = 6000 rpm:  6000 = (32/4000)/[(t2-t1)/(150MHz/128)]
//    where 32 = QCAPCTL [UPPS] (Unit timeout - once every 32 edges)
//          32/4000 = position in 1 revolution (position as a fraction of 1 revolution)
//          t2-t1/(150MHz/128),  t2-t1= # of QCAPCLK cycles, and
//		                  1 QCAPCLK cycle = 1/(150MHz/128)
//										  = QCPRDLAT
//
//		        So: 6000 rpm = [32(150MHz/128)*60s/min]/[4000(t2-t1)]
//		             t2-t1 = [32(150MHz/128)*60 s/min]/(4000*6000rpm)           - Equation 5
//		                   = 94 CAPCLK cycles = maximum (t2-t1) = SpeedScaler
//
// Divide both sides by (t2-t1), and:
//                   1 = 94/(t2-t1) = [32(150MHz/128)*60 s/min]/(4000*6000rpm)]/(t2-t1)
//				     Because (t2-t1) must be < 94 for QPOSCNT increment:
//				     94/(t2-t1) < 1 for CW rotation
//                   And because (t2-t1) must be >-94 for QPOSCNT decrement:
//				     94/(t2-t1)> -1 for CCW rotation
//
//					 speed_pr = 94/(t2-t1)
//                      or [32(150MHz/128)*60 s/min]/(4000*6000rpm)]/(t2-t1)  - Equation 6
//
// To convert speed_pr to RPM:
// Multiply Equation 6 by 6000rpm:
//                  SpeedRpm_fr  = 6000rpm * [32(150MHz/128)*60 s/min]/[4000*6000rpm*(t2-t1)]
//							                = [32(150MHz/128)*60 s/min]/[4000*(t2-t1)]
//                                        or [(32/4000)rev * 60 s/min]/[(t2-t1)(QCPRDLAT)]- Final Equation
//
