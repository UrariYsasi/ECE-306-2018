//===============================================================
//  File Name: timerB0.c
//  Description: This file contains the functions needed for PWM
//                    operation with the left and right motors
//
//  Author: Sam Messick
//  Date Feb. 2018
//  Compiler Build with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//===============================================================

/* Note: Comments marked with *************'s should be reincorperated following demonstration 
 *       Comments marked with XXXXXXXXXXXXX's should be deleted following demonstration       */

#include "timerB0.h"
// Initialize motor PWM registers
void Init_Timer_B0(void){
  TB0CTL    = TASSEL__SMCLK;            // Select SMCLK for source
  TB0CTL   |= (MC_2|TBCLR);             // Continuous mode; clear Timer B0 register 
  TB0CCR4   = MOTOR_PERIOD;
  TB0CCR6   = MOTOR_PERIOD;

  TA1CCTL0 |= CCIE;                   // Enable clock interrupts every 1/20 second
  TA1CCTL1 &= ~CCIE;
  TA1CCTL2 &= ~CCIE;
  TA1CTL &= ~(TAIFG);                 // Clear Timer A1 interrupt flag and interrupt enable
  
  
// Set all motor pins to Reset/Set mode, 0% duty cycle
  TB0CCTL3 |= OUTMOD_7;                 //  Reset/Set mode
  TB0CCTL4 |= OUTMOD_7;                 
  TB0CCTL5 |= OUTMOD_7;                 
  TB0CCTL6 |= OUTMOD_7;
  TB0CCR3   = MOTOR_OFF;                //  Begin with motor off
  TB0CCR4   = MOTOR_OFF;                
  TB0CCR5   = MOTOR_OFF;                
  TB0CCR6   = MOTOR_OFF;    
}  
