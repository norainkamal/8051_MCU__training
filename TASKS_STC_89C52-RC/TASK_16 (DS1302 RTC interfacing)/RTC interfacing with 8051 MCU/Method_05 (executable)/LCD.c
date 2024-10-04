
#include <reg51.h>         // Include the header file for the 8051 microcontroller
#include "LCD.h"           // Include the header file for the LCD functions
#include <string.h>        // Include the string library for string manipulation functions

#define lcd_data P0        // Define the data bus for the LCD as port P0

// Define the control pins for the LCD
sbit RW = P2^5;            // RW (Read/Write) pin connected to P2.5
sbit RS = P2^6;            // RS (Register Select) pin connected to P2.6
sbit EN = P2^7;            // EN (Enable) pin connected to P2.7

// Function to initialize the LCD
void lcd_init() {
    lcd_command(0x38);     // Set LCD to 2 lines, 5x7 matrix (Function Set)
    lcd_command(0x0C);     // Display ON, Cursor OFF (Display Control)
    lcd_command(0x01);     // Clear display screen (Clear Display)
    lcd_command(0x80);     // Force cursor to the beginning of the first line (Set DDRAM address)
}

// Function to send a command to the LCD
void lcd_command(unsigned char cmd) {
    lcd_data = cmd;        // Load the command into the data bus
    RW = 0;                // Set RW to 0 for write operation
    RS = 0;                // Set RS to 0 to select the instruction register
    EN = 1;                // Generate a high-to-low pulse on EN to latch the command
    delay(1);              // Short delay for the command to be processed
    EN = 0;                // Disable the enable pin
    delay(2);              // Additional delay to ensure the command is executed
}

// Function to display a single character on the LCD
void lcd_char(unsigned char dat) {
    lcd_data = dat;        // Load the data (character) into the data bus
    RW = 0;                // Set RW to 0 for write operation
    RS = 1;                // Set RS to 1 to select the data register
    EN = 1;                // Generate a high-to-low pulse on EN to latch the data
    delay(100);            // Short delay for the character to be displayed
    EN = 0;                // Disable the enable pin
    delay(200);            // Additional delay for the display to update
}

// Function to display a string of characters on the LCD
void lcd_string(unsigned char *str) {
    int i;
    for(i = 0; str[i] != 0; i++) {  // Loop through each character in the string
        lcd_char(str[i]);           // Display each character on the LCD
    }
}

// Function to set the cursor position on the LCD
void lcd_set_cursor(unsigned char row, unsigned char col) {
    unsigned char position;
    if(row == 0) {
        position = 0x80 + col;      // Set cursor to the specified column on the first line
    } else {
        position = 0xC0 + col;      // Set cursor to the specified column on the second line
    }
    lcd_command(position);          // Send the position command to the LCD
}

// Function to create a delay
void delay(unsigned int time) {
    unsigned int i, j;
    for(i = 0; i < time; i++)       // Outer loop for the delay
        for(j = 0; j < 120; j++);   // Inner loop for the delay (adjustable for timing)
}





















//		#include <reg51.h>
//		#include "LCD.h"
//		#include <string.h>


//		#define lcd_data P0
//		sbit RW=P2^5;
//		sbit RS=P2^6;
//		sbit EN=P2^7;

//		void lcd_init()
//		{
//			lcd_command(0x38);
//			lcd_command(0x0C);
//			lcd_command(0x01);
//			lcd_command(0x80);
//		}

//		void lcd_command(unsigned char cmd)
//		{
//			lcd_data=cmd;
//			RW=0;
//			RS=0;
//			EN=1;
//			delay(1);
//			EN=0;
//			delay(2);
//		}
//		void lcd_char(unsigned char dat)
//		{
//			lcd_data=dat;
//			RW=0;
//			RS=1;
//			EN=1;
//			delay(100);
//			EN=0;
//			delay(200);
//		}
//		void lcd_string(unsigned char *str)
//		{
//			int i;
//			for(i=0; str[i]!=0; i++)
//			{
//				lcd_char(str[i]);
//			}
//			
//		}
//		void lcd_set_cursor(unsigned char row, unsigned char col)
//		{
//			unsigned char position;
//			if(row==0)
//			{
//				position=0x80+col;
//			}
//			else
//			{
//				position=0xC0+col;
//			}
//			lcd_command(position);
//		}

//		void delay (unsigned int time)
//		{
//			unsigned int i,j;
//			for(i=0; i<time; i++)
//			for(j=0; j<120; j++);
//		}



