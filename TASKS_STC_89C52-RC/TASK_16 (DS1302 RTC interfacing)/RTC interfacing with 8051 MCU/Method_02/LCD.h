#ifndef LCD_H
#define LCD_H

#include <reg51.h>

// Pin Definitions
sbit RS = P2^6;    // Register Select
sbit RW = P2^5;    // Read/Write
sbit EN = P2^7;    // Enable

// Function Prototypes
void LCD_Init(void);
void LCD_Command(unsigned char cmd);
void LCD_Data(unsigned char dat);
void LCD_DisplayTimeDate(unsigned char timeDate[7]);

#endif
