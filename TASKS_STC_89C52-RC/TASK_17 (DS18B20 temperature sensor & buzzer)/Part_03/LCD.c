#include<reg51.h>
#include "main.h"

#define LCD_data P0

sbit En=P2^7;
sbit Rs=P2^6;
sbit Wr=P2^5;


void lcd_init()
{
	delay1(20);		/* LCD Power ON Initialization time >15ms */
	LCD_Command (0x38);	/* Initialization of 16X2 LCD in 8bit mode */
	LCD_Command (0x0E);	/* Display ON Cursor blinking */
	LCD_Command (0x06);	/* Auto Increment cursor */
	LCD_Command (0x01);	/* clear display */
	LCD_Command (0x80);	/* cursor at home position */

}
void lcd_string(unsigned char *lcd_data)
{
   while(*lcd_data)
   {
   lcd_write(*lcd_data++);
   }

}

void LCD_Command (unsigned char cmd)	
{
	LCD_data= cmd;
	Rs=0;				/* command reg. */
	Wr=0;				/* Write operation */
	En=1; 
	delay1(1);
	En=0;
	delay1(5);
}
void lcd_write(unsigned char lcd_data)
{
LCD_data=lcd_data;
Rs=1;
Wr=0;
En=1;
delay1(1);
En=0;

}
void delay1(unsigned int count)  /* Function to provide delay Approx 1ms */
{
	int i,j;
	for(i=0;i<count;i++)
	for(j=0;j<112;j++);
}
