#include "LCD.h"

void LCD_Init(void) {
    LCD_Command(0x38); // 8-bit mode
    LCD_Command(0x0C); // Display on, cursor off
    LCD_Command(0x01); // Clear display
    LCD_Command(0x06); // Increment cursor
}

void LCD_Command(unsigned char cmd) {
    RS = 0;  // Command mode
    RW = 0;  // Write mode
    P0 = cmd;
    EN = 1;
    EN = 0;
}

void LCD_Data(unsigned char dat) {
    RS = 1;  // Data mode
    RW = 0;  // Write mode
    P0 = dat;
    EN = 1;
    EN = 0;
}

void LCD_DisplayTimeDate(unsigned char timeDate[7]) {
    // Display HH:MM:SS
    LCD_Command(0x80); // Move cursor to the first line
    LCD_Data((timeDate[2] >> 4) + '0');  // Hours tens place
    LCD_Data((timeDate[2] & 0x0F) + '0');  // Hours ones place
    LCD_Data(':');
    LCD_Data((timeDate[1] >> 4) + '0');  // Minutes tens place
    LCD_Data((timeDate[1] & 0x0F) + '0');  // Minutes ones place
    LCD_Data(':');
    LCD_Data((timeDate[0] >> 4) + '0');  // Seconds tens place
    LCD_Data((timeDate[0] & 0x0F) + '0');  // Seconds ones place

    // Display DD/MM/YY
    LCD_Command(0xC0); // Move cursor to the second line
    LCD_Data((timeDate[4] >> 4) + '0');  // Date tens place
    LCD_Data((timeDate[4] & 0x0F) + '0');  // Date ones place
    LCD_Data('/');
    LCD_Data((timeDate[5] >> 4) + '0');  // Month tens place
    LCD_Data((timeDate[5] & 0x0F) + '0');  // Month ones place
    LCD_Data('/');
    LCD_Data((timeDate[6] >> 4) + '0');  // Year tens place
    LCD_Data((timeDate[6] & 0x0F) + '0');  // Year ones place
}
