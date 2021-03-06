//------------------------------------------------------------------------------
//  File name: main.c
//  Description: This file contains the MSP430 Car's Operating System
//
//
//  Sam Messick
//  Jan 2018
//  Built with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//------------------------------------------------------------------------------

#include "main.h"

void main(void){
//------------------------------------------------------------------------------
// Main Program
// This is the main initialization of the car. Execution of code starts here.
// The operating system is Back Ground Fore Ground... but mainly Fore Ground lol
//
//------------------------------------------------------------------------------
  
  int string_index;
  
  Init_Ports();                        // Initialize Ports
// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;
  Init_Clocks();                       // Initialize Clock System
  Init_Conditions();                   // Initialize Variables and Initial Conditions
  Init_LCD();                          // Initialize LCD
  Init_LEDs();
  Init_Timers();                       // Initialize Timers
  //Init_PID();
  Init_ADC();
  Init_Serial();

// Perform user-controlled initialization
  
  //enable_display_update();
  word1 = " Waiting  ";
  word2 = "          ";
  word3 = "115200    ";
  word4 = "          ";
  LCD_print(word1,word2,word3,word4);
//------------------------------------------------------------------------------
// Begining of the Interrupt-Based Operating System
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// TimerA0: controls LCD update, button debouncing and timed delays
// -------          defers to state machine in shapes_menu.c following push-button
//                  interrupts.
//               
// TimerB0: controls wheel PWM
//------------------------------------------------------------------------------
  

  // Loop infinitely through transmitting and receiving messages
  for(;;)
  {
    if(transmitting)
    {
      for(string_index = BEGINNING; string_index < SMALL_RING_SIZE; string_index++)
      {
        transmit_char(Main_Char_Rx[string_index]);
      }
      transmitting = false;
    } 
  }
}