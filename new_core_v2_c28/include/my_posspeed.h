/*
 * qep.h
 *
 *  Created on: 22.04.2015
 *      Author: Matthias B. (MB)
 *      Modified: Sebastian W. (SW) 12.12.2015
 */

#ifndef HW_INIT_QEP_H_
#define HW_INIT_QEP_H_

#include "IQmathLib.h"         // Include header for IQmath library
#include "DSP28x_Project.h"       // Device Headerfile and Examples Include File

/*-----------------------------------------------------------------------------
Define the structure of the POSSPEED Object
-----------------------------------------------------------------------------*/
typedef struct {int theta_elec;         // Output: Motor Electrical angle (Q15)
                int theta_mech;         // Output: Motor Mechanical Angle (Q15)
                int DirectionQep;       // Output: Motor rotation direction (Q0)
                int QEP_cnt_idx;        // Variable: Encoder counter index (Q0)
                int theta_raw;          // Variable: Raw angle from Timer 2 (Q0)
                int mech_scaler;        // Parameter: 0.9999/total count, total count = 4000 (Q26)
                int pole_pairs;         // Parameter: Number of pole pairs (Q0)
                int cal_angle;          // Parameter: Raw angular offset between encoder and phase a (Q0)
                int index_sync_flag;    // Output: Index sync status (Q0)

                Uint32 SpeedScaler;     // Parameter :  Scaler converting 1/N cycles to a GLOBAL_Q speed (Q0) - independently with global Q
                _iq Speed_pr;           // Output :  speed in per-unit
                Uint32 BaseRpm;         // Parameter : Scaler converting GLOBAL_Q speed to rpm (Q0) speed - independently with global Q
                int32 SpeedRpm_pr;      // Output : speed in r.p.m. (Q0) - independently with global Q

                _iq  oldpos;            // Input: Electrical angle (pu)
                _iq Speed_fr;           // Output :  speed in per-unit
                int32 SpeedRpm_fr;      // Output : Speed in rpm  (Q0) - independently with global Q
                void (*init)();         // Pointer to the init funcion
                void (*calc)();         // Pointer to the calc funtion
                }  POSSPEED;

/*-----------------------------------------------------------------------------
Define a POSSPEED_handle
-----------------------------------------------------------------------------*/
typedef POSSPEED *POSSPEED_handle;

/*-----------------------------------------------------------------------------
Default initializer for the POSSPEED Object.
-----------------------------------------------------------------------------*/

  #define POSSPEED_GENERIC {0x0, 0x0,0x0,0x0,0x0,16776,2,0,0x0,\
        94,0,6000,0,\
        0,0,0,\
        (void (*)(long))POSSPEED_Init,\
        (void (*)(long))POSSPEED_Calc }

//calculation example:
//Speed Maximum = 6000 rpm
//Aufl�sung vom Geber = 4 x 2500 Striche
//mech_scaler = 6710 = 0.9999/(4* 2500) * 67108864 (Q26 -> * 67108864)  und 1 Polpaar
//SpeedScaler = maximum(t2-t1) = 150 =  [4(150MHz/4)*60 s/min]/((4* 2500)*6000rpm)

/*-----------------------------------------------------------------------------
Prototypes for the functions in posspeed.c
-----------------------------------------------------------------------------*/
void POSSPEED_Init(void);
void POSSPEED_Calc(POSSPEED_handle);




#endif /* HW_INIT_QEP_H_ */
