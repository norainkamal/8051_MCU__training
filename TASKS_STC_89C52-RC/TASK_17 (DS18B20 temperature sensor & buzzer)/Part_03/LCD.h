#ifndef lcd_H
#define lcd_H
#define LCD_data P0

sbit En=P2^7;
sbit Rs=P2^6;
sbit Wr=P2^5;

void lcd_init(void);
void lcd_write(unsigned char);
void LCD_Command (unsigned char);
void delay1(unsigned int);
void lcd_string(unsigned char *)
	
#endif