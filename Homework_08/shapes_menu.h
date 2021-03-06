//===============================================================
//  File Name: shapes_menu.h
//  Description: This file contains the prototypes of all functions,
//                    definitions of macros and header files used by 
//                    shapes_menu.c
//  Author: Sam Messick
//  Date Feb. 2018
//  Compiler Build with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//===============================================================

#include "LCD.h"
#include "adc12_B.h"
#include "serial.h"
#include "wheels.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define COUNTER_RESET        (0)
#define ASCII_NUM_SHIFT      (48)
#define HTHOUSAND            (100000)
#define TTHOUSAND            (10000)
#define THOUSAND             (1000)
#define HUNDRED              (100)
#define TEN                  (10)
#define ONE                  (1)

#define UCA_BRW_115          (4)
#define UCA_BRW_460          (17)
#define UCA_MCTL_115         (0x5551)
#define UCA_MCTL_460         (0x4A10)
#define LOWER_BAUD           (115200)
#define HIGHER_BAUD          (460800)


void LCD_print(char first_line[COLUMN_NUM_COLUMNS], char second_line[COLUMN_NUM_COLUMNS], char third_line[COLUMN_NUM_COLUMNS], char fourth_line[COLUMN_NUM_COLUMNS]);   // Prints 4 lines to LCD screen
void hex_to_dec(char* output_line, uint32_t hexadecimal); // Converts hexadecimal to a 4-character decimal string for printing
void update_string(char *string_data, int string);
void print_baud_rate(void);

extern char display_line[LINE_NUM_LINES][COLUMN_NUM_COLUMNS];
extern char *display[LINE_NUM_LINES];
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned char display_changed;
extern int8_t menu_counter;            // displays which option has been chosen by the buttons
extern uint8_t calibrated;             // boolean to determine whether black and white calibration have ocurred

extern char* word1;
extern char* word2;
extern char* word3;
extern char* word4;