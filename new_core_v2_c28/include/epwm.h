/*
 * epwm.h
 *
 *  Created on: 28.09.2018
 *      Author: Michael Hoerner
 */

#ifndef INCLUDE_EPWM_H_
#define INCLUDE_EPWM_H_


void InitEPWM_CH1( int period, int vala, int valb );
void InitEPWM_CH2( int period, int vala, int valb );
void InitEPWM_CH3( int period, int vala, int valb );
void InitEPWM_CH4( int period, int vala, int valb );
void InitEPWM_CH5( int period, int vala, int valb );
void InitEPWM_CH6( int period, int vala, int valb );

void PWM_HB_OFF(int HalfBridgeNumber);

void PWM_HB_ON(int HalfBridgeNumber);


#endif /* INCLUDE_EPWM_H_ */
