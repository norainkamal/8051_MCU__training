//#ifndef LCD_H
//#define LCD_H

//void lcd_init();
//void lcd_command(unsigned char cmd);
//void lcd_char(unsigned char dat);
//void lcd_string(unsigned char *str);
//void delay (unsigned int);
//void lcd_set_cursor(unsigned char row, unsigned char col);
//void lcd_display(unsigned char number);

//#endif

#ifndef LCD_H
#define LCD_H

void lcd_init();
void lcd_command(unsigned char cmd);
void lcd_char(unsigned char dat);
void lcd_string(unsigned char *str);
void delay (unsigned int);
void lcd_set_cursor(unsigned char row, unsigned char col);
void lcd_display(unsigned char number);

// Function prototypes
void extract_time_set_rtc(unsigned char *time_data);
void extract_date_set_rtc(unsigned char *date_data);
void DS1302_setdate(unsigned char day, unsigned char month, unsigned char year);

#endif
