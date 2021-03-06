//===============================================================
//  File Name: clocks.h
//  Description: This file contains the prototypes of all functions,
//                    definitions of macros and header files used by 
//                    adc12_B.c
//  Author: Sam Messick
//  Date Feb. 2018
//  Compiler Build with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//===============================================================

#include "msp430.h"
#include "ports.h"
#include <stdbool.h>
#include <stdint.h>
#define ADC_RESET_STATE     (0)
#define STABILIZE_REFERENCE {__delay_cycles(10000);}
#define THUMB_ACTIVE_READING (1000)

void Init_ADC(void);                             // Initialize Analog-to-Digital-Converter
void calibrate_White(void);                      // Function for setting highest value for "white" in PID (via thumbwheel)
void calibrate_Black(void);                      // Function for setting lowest value  for "black" in PID (via thumbwheel)
void updateDisplay(void);                        // Function for printing ADC values for left detector to screen and updating ADC readings
void enable_adc(void);

extern volatile uint16_t ADC_Thumb;              // ADC reading from Thumbwheel
extern volatile uint16_t ADC_Right_Detector;     // ADC reading from right infrared detector
extern volatile uint16_t ADC_Left_Detector;      // ADC reading from left  infrared detector
extern volatile uint8_t  ir_ready_to_print;      // Boolean allowing TimerA0.0 to trigger wheel PID update and ADC value printing
extern uint16_t ir_black_reading;
extern uint16_t ir_white_reading;
extern uint8_t  calibrated;                      // Boolean allowing for regular wheel PID updates to begin and ADC value printing to halt
extern volatile uint8_t calibrating;             // Set false by a button press -- ends a calibrate_White and calibrate_Black functions

/* From menus.c */
#define ASCII_NUM_SHIFT      (48)

void defer_to_shape_menu(void);                  // Pulls up degraded shape menu for printing ADC values
void print_detector_values(void);                // prints ADC reading from left infrared detector to screen 
extern char* word4;                              // provided for ADC writing "Cal. Black" or "Cal. White" to screen
void LCD_print(char first_line[COLUMN_NUM_COLUMNS], char second_line[COLUMN_NUM_COLUMNS], char third_line[COLUMN_NUM_COLUMNS], char fourth_line[COLUMN_NUM_COLUMNS]);   // Prints 4 lines to LCD screen
void hex_to_dec(char* output_line, uint16_t hexadecimal);
extern volatile uint16_t circle_time;
extern volatile uint8_t waiting;