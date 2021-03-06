//===============================================================
//  File Name: menu.h
//  Description: This file contains the prototypes of all functions,
//                    definitions of macros and header files used by 
//                    display.c
//  Author: Sam Messick
//  Date Feb. 2018
//  Compiler Build with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//===============================================================

#include "LCD.h"
#include "wheels.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

void update_menu(void);
void LCD_print(char first_line[10], char second_line[10], char third_line[10], char fourth_line[10]);
void update_string(char *string_data, int string);
void delay(uint16_t msecs);


extern char display_line[LINE_NUM_LINES][COLUMN_NUM_COLUMNS];
extern char *display[LINE_NUM_LINES];
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned char display_changed;
extern int8_t menu_counter;            // displays which option has been chosen by the buttons
