#include<reg51.h>
#include "lcd.h"
#include "delay.h"

 
#define dataport P0       // LCD dataport connected to PORT1
sbit rs = P2^6;  //RS pin connected to pin 2 of port 3
sbit rw = P2^5;  // RW pin connected to pin 3 of port 3
sbit en  =  P2^7;
 
 
 
/* 16x2 lcd Specification */
#define LCDMaxLines 2
#define LCDMaxChars 16
#define LineOne 0x80
#define LineTwo 0xc0
 
#define BlankSpace ' '
 
//LCD initilization
void LCD_Init()
{
    delay_us(5000);
   lcd_WriteCmd(0x02);  //initilize the LCD in 4bit Mode
   lcd_WriteCmd(0x28);
   lcd_WriteCmd(0x0C);  // display ON cursor ON
   lcd_WriteCmd(0x01);  // clear the LCD
   lcd_WriteCmd(0x80);  // move the Cursor to First line First Position
 
}
 
 
// lcd Write command function
void lcd_WriteCmd( char a)
{
 
   dataport=(a & 0xf0);        
   rs=0;                      
   rw=0;                 
 	 en=1;                 
   delay_us(1);
   en=0;
   delay_us(1);                  
   dataport=((a<<4) & 0xf0);   
   rs=0;                    
   rw=0;                    
   en=1;                      
   delay_us(1);
   en=0;
   delay_us(1);
}
 
 
 // lcd writedata function
void lcd_Writedata( char a)
{
 
 
   dataport=(a & 0xf0);        
     rs=1;                      
     rw=0;                      
     en=1;                      
    delay_us(1);
     en=0;
 
   delay_us(1);                
 
   dataport=((a <<4) & 0xf0);  
     rs=1;                    
     rw=0;                     
     en=1;                      
    delay_us(1);
     en=0;
 
   delay_us(1);
 
}

 
 //lcd gotolinetwo fuction
void lcd_GoToLineTwo()
{
   lcd_WriteCmd(LineTwo);   // move the Cursor to Second line First Position
}
 
 
 
//lcd go to X(first row), Y(second row) line
void lcd_GoToXY(char row, char col)
{
   char pos;
 
    if(row<LCDMaxLines)
      {
         pos= LineOne | (row << 6);
                                    
 
        if(col<LCDMaxChars)
           pos= pos+col;            
                                    
 
         lcd_WriteCmd(pos);        
       }
}
 

//lcd display string function
void lcd_DisplayString(char *string_ptr)
{
   while(*string_ptr)
    lcd_Writedata(*string_ptr++);
    }
 
 
//lcd display rtc time function
void lcd_DisplayRtcTime(char hour,char min,char sec)
{
	char t=0x01;
	if((hour&0x80)==0)
		t=0x03;
    lcd_Writedata(((hour>>4) & t) + 0x30);
    lcd_Writedata((hour & 0x0f) + 0x30);
    lcd_Writedata(':');
 
    lcd_Writedata(((min>>4) & 0x0f) + 0x30);
    lcd_Writedata((min & 0x0f) + 0x30);
    lcd_Writedata(':');
 
    lcd_Writedata(((sec>>4) & 0x0f) + 0x30);
    lcd_Writedata((sec & 0x0f) + 0x30);
 
 }
 
 
 // lcd display rtc date function
void lcd_DisplayRtcDate(char day,char month,char year)
 {
     lcd_Writedata(((day>>4) & 0x0f) + 0x30);
     lcd_Writedata((day & 0x0f) + 0x30);
     lcd_Writedata('/');
 
     lcd_Writedata(((month>>4) & 0x0f) + 0x30);
     lcd_Writedata((month & 0x0f) + 0x30);
     lcd_Writedata('/');
 
     lcd_Writedata(((year>>4) & 0x0f) + 0x30);
     lcd_Writedata((year & 0x0f) + 0x30);
 
 }