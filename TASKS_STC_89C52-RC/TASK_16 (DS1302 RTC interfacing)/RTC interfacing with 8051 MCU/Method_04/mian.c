#include <reg51.h> 
#include "lcd.h"  	  
#include "delay.h"
//#include <DS1302.h>

#define  ADR_WR_CREG    0x8E  //Control Register Write
#define  ADR_RD_CREG    0x8F  //Control Register Read
#define  ADR_WR_TCR     0x90  //Trickle Charge Register Write
#define  ADR_RD_TCR     0x91  //Trickle Charge Register Read
#define  ADR_WR_BURST   0xBE  //Burst Write
#define  ADR_RD_BURST   0xBF  //Burst Read

#define  ADR_WR_SEC    0x80  //Seconds Write
#define  ADR_RD_SEC    0x81  //Seconds Read
#define  ADR_WR_MIN    0x82  //Mins Write
#define  ADR_RD_MIN    0x83  //Mins Read
#define  ADR_WR_HR     0x84   //
#define  ADR_RD_HR     0x85
#define  ADR_WR_DATE   0x86
#define  ADR_RD_DATE   0x87
#define  ADR_WR_MON    0x88
#define  ADR_RD_MON    0x89
#define  ADR_WR_DAY    0x8A
#define  ADR_RD_DAY    0x8B
#define  ADR_WR_YR     0x8C
#define  ADR_RD_YR     0x8D

int clock_data[8];
int temp_byte[8];
int temp;
int x;
unsigned int i;
unsigned int j;
char array[32];
char jj=0,ii=0,kk=0;

unsigned char sec,min,hour,day,month,year;





sbit CE=P3^5;
sbit CLk=P3^6;
sbit io=P3^4;
 



void Ext_int_Init()				
{
	EA  = 1;		/* Enable global interrupt */
	ES = 1;  		/* Enable serial interrupt */			
}

void UART_Init()
{
	TMOD = 0x20;		/* Timer 1, 8-bit auto reload mode */
	TH1 = 0xFD;		/* Load value for 9600 baud rate */
	TL1=0xFF;
	SCON = 0x50;		/* Mode 1, reception enable */
	TR1 = 1;		/* Start timer 1 */
}
void Serial_ISR() interrupt 4    
{ if(RI==1){
	array[jj]=SBUF; 
	RI = 0;	/* Clear RI flag */
	jj++;
	}	

} 
 void SCLK_Pulse (void)
{
	CLk=1;
	CLk=0;
}
void Send_Bit (unsigned int value)
{
    if (value != 0){
        io=1;
		}
    else {
        io=0;
    }
}
 void Write_Byte(int content)
{

    for (i=0;i<8;i++){
             x = (content & (1 << i));  //Write to Address
             Send_Bit(x);
             SCLK_Pulse();
        }
}

void Read_Burst (void)
{
    CE=1;
    Write_Byte(ADR_RD_BURST);
    for (i=0;i<8;i++){
        temp = 0;
    for (j=0;j<8;j++){
            temp >>= 1;
                if (io == 1){

                    temp |= 0x80;
                       }
                       else {

                    temp  &= 0x7F;
                       }

                SCLK_Pulse();
                }
    clock_data[i] = temp;
    }
    CE=0;
}

void Read_Byte(int addr)
{
    temp = 0;
    Write_Byte(addr);
    for (j=0;j<8;j++){
        temp >>= 1;
				
				if (io == 1)
						temp |= 0x80;
				else 
						temp  &= 0x7F;

				SCLK_Pulse();
				}
}
void write_data(char cmd,char value)
{
	CE=1;
	Write_Byte(cmd);  //cmd
  Write_Byte(value);  //data
	CE=0;
}
 void set_time(char hrs, char mins, char secs , char days,char month,char year)
 {
	write_data(ADR_WR_SEC,secs);//Go to seconds register
	
	write_data(ADR_WR_MIN,mins);  //Go to minute register
	
	write_data(ADR_WR_HR,hrs);  //Go to hour register

	write_data(ADR_WR_DATE,days);  //Go to date register
	
	write_data(ADR_WR_MON,month);  //Go to month register
	
	write_data(ADR_WR_DAY,0x01);  //Go to day register
	
	write_data(ADR_WR_YR,year);  //Go to year register
 }	 
 void time_cvt()
 {
	 	 sec=((array[1]-0x030) | ((array[0]-0x30)<<4));
	 min=((array[4]-0x030) | ((array[3]-0x30)<<4));
	 hour=((array[7]-0x030) | ((array[17] & 0xD0)<<1) |((array[6]-0x30)<<4));
	 day=((array[10]-0x030) | ((array[9]-0x30)<<4));
	 month=((array[13]-0x030) | ((array[12]-0x30)<<4));
	 year=((array[16]-0x030) | ((array[15]-0x30)<<4));
		set_time(hour,min,sec,day,month,year);

 }
/* main program */
void main() 
{
	
  LCD_Init();
	
	write_data(ADR_WR_CREG,0x00);
  
	write_data(ADR_WR_TCR,0xAB);//Go to Trickle charge 
  //Enable Charger, 2 diodes, 8kOhm
	
  set_time(0xB1,0x59,0x00,0x21,0x012,0x22);

	Ext_int_Init();  	/* Call Ext. interrupt initialize */
	UART_Init();
		
  lcd_DisplayString("TIME  ");
 
   /* Display "date" on Second line*/
  lcd_GoToLineTwo();
  lcd_DisplayString("DATE  ");
 
   /* Display the time and date continously */ 
   while(1)
    {
			if (jj>ii)
			{
				if(array[ii-1]== 0x5C && array[ii]== 'r' ){
						EA  = 0;
						ES = 0; 
					time_cvt();
					ii=0;jj=0;
				}
				ii++;
			}
			CE=1;
			Read_Burst();
			CE=0;

 
      lcd_GoToXY(0,6); 		 
			lcd_DisplayRtcTime(clock_data[2],clock_data[1],clock_data[0]);
			if((clock_data[2]&0x80)>0){
				if((clock_data[2]&0x20)>0)
					lcd_DisplayString("PM");
				else
					lcd_DisplayString("AM");
			}
			else
				lcd_DisplayString("  ");
			
			lcd_GoToXY(1,6); 		 
			lcd_DisplayRtcDate(clock_data[3],clock_data[4],clock_data[6]);
     }
}

