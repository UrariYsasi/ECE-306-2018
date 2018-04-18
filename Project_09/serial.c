//===============================================================
//  File Name: serial.c
//  Description: This file contains the Initialization and
//                    interrupts for serial I/O
//
//  Author: Sam Messick
//  Date Mar. 2018
//  Compiler Build with IAR Embedded Workbench Version: V4.10A/W32 (7.11.2)
//===============================================================

#include "serial.h"

volatile uint8_t iot_tx_wr;                                   // IOT transmit buffer index
volatile uint8_t iot_status_reg = SOFT_RESET;                 // Register for managing IOT flags
uint8_t main_ring_rd;
int8_t chars_to_read; 
char IOT_Char_Rx[SMALL_RING_SIZE];
char IP_line1[COLUMN_NUM_COLUMNS];
char IP_line2[COLUMN_NUM_COLUMNS];
char Main_Char_Rx[SMALL_RING_SIZE];

void Init_Serial(void){
  Init_Serial_UCA0();
  Init_Serial_UCA3();
}

void Init_Serial_UCA0(void){
  // Confiure UART 0
  UCA0CTLW0  = REGISTER_WORD_SIZE;    // Use word-size register (16-bit)
  UCA0CTLW0 |= UCSSEL__SMCLK;         // Set SMCLK as frequency source
  UCA0CTLW0 |= UCSWRST;               // Set software reset enable
  UCA0BRW    = UCA_BRW_115;           // ~115200 Baud
  UCA0MCTLW  = UCA_MCTL_115;
  UCA0CTL1  &= ~UCSWRST;              // Release from software reset
  UCA0IE    |= UCRXIE;
  UCA0IFG   &= ~(UCRXIFG);
}

void Init_Serial_UCA3(void){
  // Confiure UART 3
  UCA3CTLW0  = REGISTER_WORD_SIZE;    // Use word-size register (16-bit)
  UCA3CTLW0 |= UCSSEL__SMCLK;         // Set SMCLK as frequency source
  UCA3CTLW0 |= UCSWRST;               // Set software reset enable
  UCA3BRW    = UCA_BRW_115;           // ~115200 Baud
  UCA3MCTLW  = UCA_MCTL_115;
  UCA3CTL1  &= ~UCSWRST;              // Release from software reset
  UCA3IE    |= UCRXIE;
  UCA3IFG   &= ~(UCRXIFG);
}

void transmit_charA3(char character){
  while (UCA3STATW & UCBUSY); // Transmit complete interrupt flag
    UCA3TXBUF = character;
}

void transmit_charA0(char character){
  while (UCA0STATW & UCBUSY); // Transmit complete interrupt flag
    UCA0TXBUF = character;
}

void check_for_input(void){
  
    if(IOT_STATUS(IP_READY))
    {
      // Print out WiFi module IP address on lines 3 and 4 of LCD
      strncpy(IP_line1, IOT_Char_Rx + CHAR2, CHAR7);
      strncpy(IP_line2, IOT_Char_Rx + CHAR9, CHAR7);
      
      word1 = "   ncsu   ";
      word2 = "          ";
      word3 = IP_line1;
      word4 = IP_line2;
      LCD_print(word1,word2,word3,word4);
      iot_tx_wr = BEGINNING;
      IOT_DISABLE(IP_READY);
      IOT_DISABLE(SOFT_RESET);
    }
    if(IOT_STATUS(CHECK_FOR_COMMAND))
      if(Main_Char_Rx[CHAR1] == COMMAND_START)
      {
        parse_command();
        IOT_DISABLE(CHECK_FOR_COMMAND);
      }
      else if(Main_Char_Rx[CHAR11] == COMMAND_START && !IOT_STATUS(COMMAND_EXECUTING))
      {
        parse_second_command();
        IOT_DISABLE(CHECK_FOR_COMMAND);
      }
      else if(Main_Char_Rx[CHAR1] == TEST_START)
      {
        parse_test();
        IOT_DISABLE(CHECK_FOR_COMMAND);
      }
    read_into_buffer();
    //UCA3IE |= UCRXIE;
}

// Updates main buffer alongside iot buffer
void read_into_buffer(void){
  static uint8_t rx_wr_temp;
  while(rx_wr_temp != iot_tx_wr)
    rx_wr_temp = iot_tx_wr;
  while(main_ring_rd != rx_wr_temp || chars_to_read > COUNTER_RESET)
  {
    Main_Char_Rx[main_ring_rd] = IOT_Char_Rx[main_ring_rd];
    chars_to_read--;
    if(Main_Char_Rx[main_ring_rd] == '\n' || ++main_ring_rd >= SMALL_RING_SIZE)
    {
      main_ring_rd = BEGINNING;
      IOT_ENABLE(CHECK_FOR_COMMAND);
      break;
    }
  }
}

void parse_command(void){
  char direction;
  uint16_t password_attempt;
  
  password_attempt = (Main_Char_Rx[CHAR2] - ASCII_NUM_SHIFT)*THOUSAND +
                     (Main_Char_Rx[CHAR3] - ASCII_NUM_SHIFT)*HUNDRED  +
                     (Main_Char_Rx[CHAR4] - ASCII_NUM_SHIFT)*TEN      +
                     (Main_Char_Rx[CHAR5] - ASCII_NUM_SHIFT);
  
  if(password_attempt == COMMAND_PASS)
  {
    
    // User verified; interpret command
    direction = Main_Char_Rx[CHAR6];
    
    // Retrieve duration from buffer (milliseconds)
    delay_time     +=(Main_Char_Rx[CHAR7] - ASCII_NUM_SHIFT)*THOUSAND +
                     (Main_Char_Rx[CHAR8] - ASCII_NUM_SHIFT)*HUNDRED  +
                     (Main_Char_Rx[CHAR9] - ASCII_NUM_SHIFT)*TEN      +
                     (Main_Char_Rx[CHAR10] - ASCII_NUM_SHIFT);
    switch(direction)
    {
    case 'S':
      Wheels_OFF();
      break;
    case 'F':
      Wheels_OFF();
      Left_Motor_ON_FORWARD(LEFT_FORWARD_SPEED);
      Right_Motor_ON_FORWARD(RIGHT_FORWARD_SPEED);
      break;
    case 'B':
      Wheels_OFF();
      Left_Motor_ON_REVERSE(LEFT_FORWARD_SPEED);
      Right_Motor_ON_REVERSE(RIGHT_FORWARD_SPEED);
      break;
    case 'R':
      Wheels_OFF();
      Left_Motor_ON_FORWARD(LEFT_FORWARD_SPEED);
      break;
    case'L':
      Wheels_OFF();
      Right_Motor_ON_FORWARD(RIGHT_FORWARD_SPEED);
      break;  
    }
    
    // Enable timer interrupt
    TA0CCTL0 |= CCIE;
    IOT_ENABLE(COMMAND_EXECUTING);
  }
}

void parse_second_command(void){
  char direction;
  uint16_t password_attempt;
  
  password_attempt = (Main_Char_Rx[CHAR12] - ASCII_NUM_SHIFT)*THOUSAND +
                     (Main_Char_Rx[CHAR13] - ASCII_NUM_SHIFT)*HUNDRED  +
                     (Main_Char_Rx[CHAR14] - ASCII_NUM_SHIFT)*TEN      +
                     (Main_Char_Rx[CHAR15] - ASCII_NUM_SHIFT);
  
  if(password_attempt == COMMAND_PASS)
  {
    
    // User verified; interpret command
    direction = Main_Char_Rx[CHAR16];
    
    // Retrieve duration from buffer (milliseconds)
    delay_time     +=(Main_Char_Rx[CHAR1] - ASCII_NUM_SHIFT)*THOUSAND +
                     (Main_Char_Rx[CHAR2] - ASCII_NUM_SHIFT)*HUNDRED  +
                     (Main_Char_Rx[CHAR3] - ASCII_NUM_SHIFT)*TEN      +
                     (Main_Char_Rx[CHAR4] - ASCII_NUM_SHIFT);
    switch(direction)
    {
    case 'S':
      Wheels_OFF();
      break;
    case 'F':
      Wheels_OFF();
      Left_Motor_ON_FORWARD(LEFT_FORWARD_SPEED);
      Right_Motor_ON_FORWARD(RIGHT_FORWARD_SPEED);
      break;
    case 'B':
      Wheels_OFF();
      Left_Motor_ON_REVERSE(LEFT_FORWARD_SPEED);
      Right_Motor_ON_REVERSE(RIGHT_FORWARD_SPEED);
      break;
    case 'R':
      Wheels_OFF();
      Left_Motor_ON_FORWARD(LEFT_FORWARD_SPEED);
      break;
    case'L':
      Wheels_OFF();
      Right_Motor_ON_FORWARD(RIGHT_FORWARD_SPEED);
      break;  
    }
    
    // Enable timer interrupt
    TA0CCTL0 |= CCIE;
  }
}

void parse_test(void){
  uint8_t string_index;
  static char* higher_baud = "460800\r\n";
  static char* lower_baud  = "115200\r\n";
  
  switch(Main_Char_Rx[CHAR2])
  {
  case 'S': // 115200 baud
    UCA3CTLW0 |= UCSWRST;               // Set software reset enable
    UCA3BRW = UCA_BRW_115;
    UCA3MCTLW  = UCA_MCTL_115;
    UCA3CTL1  &= ~UCSWRST;              // Release from software reset
    UCA3IE    |= UCRXIE;
    for(string_index = BEGINNING; string_index < BAUD_WORD_SIZE; string_index++)
      transmit_charA0(lower_baud[string_index]);
    break;
  case 'F': // 460800 baud
    UCA3CTLW0 |= UCSWRST;               // Set software reset enable
    UCA3BRW = UCA_BRW_460;
    UCA3MCTLW = UCA_MCTL_460;
    UCA3CTL1  &= ~UCSWRST;              // Release from software reset
    UCA3IE    |= UCRXIE;
    for(string_index = BEGINNING; string_index < BAUD_WORD_SIZE; string_index++)
      transmit_charA0(higher_baud[string_index]);
  }
}

#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void){
  switch(__even_in_range(UCA0IV, eight))
  {
  case RXIFG:
    if(UCA0RXBUF == '\0')
      break;
    // Transmit received character to IOT Module
    while(UCA3STATW & UCBUSY);
    UCA3TXBUF = UCA0RXBUF;
  }
}

#pragma vector = USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void){
  uint8_t temp;
  switch(__even_in_range(UCA3IV, eight))
  {
  case RXIFG:
    while(UCA0STATW & UCBUSY);
    UCA0TXBUF = UCA3RXBUF;             // Echo character to USB
    
    chars_to_read++;
    
    chars_to_read %= DOUBLE_COMMAND_LENGTH;
    temp = iot_tx_wr;
    IOT_Char_Rx[temp] = UCA3RXBUF;     // Read character into interrupt buffer
    
    if(++iot_tx_wr >= SMALL_RING_SIZE || UCA3RXBUF == '\n')
    {
      // Reset interrupt buffer index if end reached
      iot_tx_wr = BEGINNING;
      
      if(IOT_Char_Rx[CHAR1] == ':' &&
         IOT_Char_Rx[CHAR2] == IP_LEAD_BYTE &&
         IOT_STATUS(SOFT_RESET))
      {
        IOT_ENABLE(IP_READY);               // Notify main that IP address is received
      }
    }
    break;
  }
}