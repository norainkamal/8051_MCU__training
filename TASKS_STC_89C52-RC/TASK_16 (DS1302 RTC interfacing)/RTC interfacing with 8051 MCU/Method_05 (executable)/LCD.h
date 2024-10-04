#ifndef LCD_H            // Include guard to prevent multiple inclusions
#define LCD_H

// Function prototypes
void lcd_init();                      // Initialize the LCD
void lcd_command(unsigned char cmd);  // Send a command to the LCD
void lcd_char(unsigned char dat);     // Display a single character on the LCD
void lcd_string(unsigned char *str);  // Display a string on the LCD
void delay(unsigned int time);        // Create a delay
void lcd_set_cursor(unsigned char row, unsigned char col); // Set cursor position on the LCD

#endif
