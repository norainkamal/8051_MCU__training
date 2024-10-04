/* Only applicable for Time */

#include <reg51.h>
#include "LCD.h"
#include <string.h>
#include <stdio.h>

sbit DS1302_IO = P3^4;
sbit DS1302_CE = P3^5;
sbit DS1302_CLK = P3^6;

#define DS1302_READ_BURST 0xBF
#define DS1302_WRITE_BURST 0xBE
#define DS1302_SECOND 0x80
#define DS1302_SECOND_READ 0x81
#define DS1302_MINUTE 0x82
#define DS1302_MINUTE_READ 0x83
#define DS1302_HOUR 0x84 
#define DS1302_HOUR_READ 0x85
#define DS1302_DATE 0x86
#define DS1302_MONTH 0x88
#define DS1302_DAY 0x8A
#define DS1302_YEAR 0x8C
#define DS1302_CONTROL 0x8E
#define DS1302_CHARGER 0x90
#define DS1302_RAMBURST_READ 0xFF
#define DS1302_RAMBURST_WRITE 0xFE
#define CLK_DELAY 10

void write_byte(unsigned int byte);
unsigned char read_byte(unsigned char cmd);
void DS1302_settime(unsigned char hour, unsigned char minute, unsigned char second);
void DS1302_setdate(unsigned char day, unsigned char month, unsigned char year);
void DS1302_gettime(void);
void DS1302_getdate(void);
void send_bit(unsigned int bitt);
void DS1302_init();
void DS1302_readbyte();
void UART_receivedata();
void UART_init();
void extract_time_set_rtc(char *timestring);
void lcd_display(unsigned char number);

unsigned int xdata read_bytes[12]; // Moved to XDATA segment to reduce DATA usage
unsigned char xdata receivemsg[100]; // Moved to XDATA segment
unsigned char receivedata;
unsigned int index = 0;
bit messagereceived = 0;

void extract_time_set_rtc(char *timestring)
{
		unsigned int hour, minute, second;
		char timeformatted[7];
		sscanf(timestring, "%hhu:%hhu:%hhu", &hour, &minute, &second);
		sprintf(timeformatted, "%02u,%02u,%02u", hour, minute, second);
		lcd_set_cursor(0, 0);
		lcd_string("TIME:");
		lcd_set_cursor(0, 6);
		lcd_string(timeformatted);
		sscanf(timestring, "0x%hhx:0x%hhx:0x%hhx", &hour, &minute, &second);
		DS1302_settime((unsigned char)hour, (unsigned char)minute, (unsigned char)second);
}

void UART_init()
{
		TMOD = 0x20;
		TH1 = 0xFD; // 9600 baud rate
		SCON = 0x50;
		TR1 = 1;
}

void UART_receivedata()
{
		while (RI == 0);
		RI = 0;
		receivedata = SBUF;
		messagereceived = 1;
}

void SCLK_Pulse()
{
		DS1302_CLK = 1;
		DS1302_CLK = 0;
}

void send_bit(unsigned int bitt)
{
		DS1302_IO = (bitt != 0) ? 1 : 0;
}

void write_byte(unsigned int byte)
{
		unsigned int i, Bit;
		for (i = 0; i < 8; i++)
		{ // RTC write byte
				Bit = byte & (1 << i);
				send_bit(Bit);
				SCLK_Pulse();
		}
}

unsigned char read_byte(unsigned char cmd)
{
		unsigned int j, dat = 0;
		write_byte(cmd);
		for (j = 0; j < 8; j++)
		{
				dat >>= 1;
				if (DS1302_IO)
				{
						dat |= 0x80;
				}
				SCLK_Pulse();
		}
		return dat;
}

void DS1302_writebyte(unsigned char cmd, unsigned char value)
{
		DS1302_CE = 1;
		write_byte(cmd); // RTC write byte at address
		write_byte(value);
		DS1302_CE = 0;
}

void DS1302_readbyte()
{
		unsigned int dat = 0, i, j;
		DS1302_CE = 1;

		write_byte(DS1302_READ_BURST);
		for (i = 0; i < 8; i++) // Store in array
		{
				dat = 0;
				for (j = 0; j < 8; j++) // Bit by bit read
				{
						dat >>= 1;
						if (DS1302_IO)
						{ // Reading ta enable
								dat |= 0x80;
						}
						SCLK_Pulse();
				}
				read_bytes[i] = dat;
		}
		DS1302_CE = 0;
}

void DS1302_init()
{
		unsigned char r;
		DS1302_CE = 0;
		DS1302_CLK = 0;
		DS1302_writebyte(DS1302_CONTROL, 0x00);
		DS1302_writebyte(DS1302_CHARGER, 0xAB);

		r = read_byte(DS1302_SECOND_READ);
		if ((r & DS1302_SECOND_READ) != 0)
				DS1302_writebyte(DS1302_SECOND, 0); // Zarah value zero ka
}

void DS1302_settime(unsigned char hour, unsigned char minute, unsigned char second)
{
		DS1302_writebyte(DS1302_SECOND, second);
		DS1302_writebyte(DS1302_MINUTE, minute);
		DS1302_writebyte(DS1302_HOUR, hour);
}

void lcd_display(unsigned char number)
{
		lcd_char(((number >> 4) & 0x0F) + 0x30); // Ten
		lcd_char((number & 0x0F) + 0x30); // Unit
}

void DS1302_gettime(void)
{
		lcd_set_cursor(0, 0);
		lcd_set_cursor(0, 6);
		lcd_display(read_bytes[2]);
		lcd_char(':');
		delay(10);

		lcd_set_cursor(0, 9);
		lcd_display(read_bytes[1]);
		lcd_char(':');
		delay(10);

		lcd_set_cursor(0, 12);
		lcd_display(read_bytes[0]);
		delay(10);
}

void DS1302_setdate(unsigned char day, unsigned char month, unsigned char year)
{
		DS1302_writebyte(DS1302_DATE, day);
		DS1302_writebyte(DS1302_MONTH, month);
		DS1302_writebyte(DS1302_YEAR, year);
}

void DS1302_getdate(void)
{
		lcd_set_cursor(1, 0);
		lcd_set_cursor(1, 6);
		lcd_display(read_bytes[3]);
		lcd_char(':');
		delay(10);

		lcd_set_cursor(1, 9);
		lcd_display(read_bytes[4]);
		lcd_char(':');
		delay(10);

		lcd_set_cursor(1, 12);
		lcd_display(read_bytes[6]);
		delay(10);
}

void main()
{
		lcd_init();
		DS1302_init();
		UART_init();

		while (1)
		{
				UART_receivedata();
				if (messagereceived)
				{
						if (receivedata == 'n')
						{
								receivemsg[index] = '\0';
								if (receivemsg[0] == 't')
								{
										lcd_command(0x80);
										delay(1000);
										extract_time_set_rtc(&receivemsg[1]);

										while (1)
										{
												DS1302_CE = 1;
												DS1302_readbyte();
												DS1302_CE = 0;
												DS1302_gettime();
										}

										index = 0;
										memset(receivemsg, 0, sizeof(receivemsg));
										messagereceived = 0;
								}
						}
						else
						{
								receivemsg[index] = receivedata;
								index++;
						}
				}
				else
				{
						lcd_string("nodata");
				}
		}
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




//#include <reg51.h>
//#include "LCD.h"
//#include <string.h>
//#include <stdio.h>

//// Pin definitions for DS1302
//sbit DS1302_IO = P3^4;
//sbit DS1302_CE = P3^5;
//sbit DS1302_CLK = P3^6;

//// DS1302 Command Definitions
//#define DS1302_READ_BURST 0xBF
//#define DS1302_WRITE_BURST 0xBE
//#define DS1302_SECOND 0x80
//#define DS1302_SECOND_READ 0x81
//#define DS1302_MINUTE 0x82
//#define DS1302_MINUTE_READ 0x83
//#define DS1302_HOUR 0x84 
//#define DS1302_HOUR_READ 0x85
//#define DS1302_DATE 0x86
//#define DS1302_MONTH 0x88
//#define DS1302_DAY 0x8A
//#define DS1302_YEAR 0x8C
//#define DS1302_CONTROL 0x8E
//#define DS1302_CHARGER 0x90
//#define DS1302_RAMBURST_READ 0xFF
//#define DS1302_RAMBURST_WRITE 0xFE
//#define CLK_DELAY 10

//// Function Prototypes
//void write_byte(unsigned int byte);
//unsigned char read_byte(unsigned char cmd);
//void DS1302_settime(unsigned char hour, unsigned char minute, unsigned char second);
//void DS1302_setdate(unsigned char day, unsigned char month, unsigned char year);
//void DS1302_gettime(void);
//void DS1302_getdate(void);
//void send_bit(unsigned int bitt);
//void DS1302_init();
//void DS1302_readbyte();
//void UART_receivedata();
//void UART_init();
//void extract_time_set_rtc(unsigned char xdata *timestring);

//// Global Variables (moved to xdata space where necessary)
//unsigned int xdata read_bytes[12];
//unsigned char receivedata;
//unsigned char xdata receivemsg[100];
//unsigned int index = 0;
//bit messagereceived = 0;

//void extract_time_set_rtc(unsigned char xdata *timestring)
//{
//    unsigned int hour, minute, second;
//    char timeformatted[7];
//    sscanf(timestring, "%hhu:%hhu:%hhu", &hour, &minute, &second);
//    sprintf(timeformatted, "%02u,%02u,%02u", hour, minute, second);
//    lcd_set_cursor(0, 0);
//    lcd_string("TIME:");
//    lcd_set_cursor(0, 6);
//    lcd_string(timeformatted);
//    sscanf(timestring, "0x%hhx:0x%hhx:0x%hhx", &hour, &minute, &second);
//    DS1302_settime((unsigned char)hour, (unsigned char)minute, (unsigned char)second);
//}

//void UART_init()
//{
//    TMOD = 0x20;
//    TH1 = 0xFD; // 9600 baud rate
//    SCON = 0x50;
//    TR1 = 1;
//}

//void UART_receivedata()
//{
//    while(RI == 0);
//    RI = 0;
//    receivedata = SBUF;
//    messagereceived = 1;
//}

//void SCLK_Pulse()
//{
//    DS1302_CLK = 1;
//    DS1302_CLK = 0;
//}

//void send_bit(unsigned int bitt)
//{
//    if(bitt != 0)
//    {
//        DS1302_IO = 1; // write one bit
//    }
//    else
//    {
//        DS1302_IO = 0;
//    }
//}

//void write_byte(unsigned int byte)
//{
//    unsigned int i, Bit;
//    for(i = 0; i < 8; i++)
//    { // write byte to RTC
//        Bit = byte & (1 << i);
//        send_bit(Bit);
//        SCLK_Pulse();
//    }
//}

//unsigned char read_byte(unsigned char cmd)
//{
//    unsigned int j, dat = 0;
//    write_byte(cmd);
//    for(j = 0; j < 8; j++)
//    {
//        dat >>= 1;
//        if(DS1302_IO)
//        {
//            dat |= 0x80;
//        }
//        else
//        {
//            dat &= 0x7F;
//        }
//        SCLK_Pulse();
//    }
//    return dat;
//}

//void DS1302_writebyte(unsigned char cmd, unsigned char value)
//{
//    DS1302_CE = 1;
//    write_byte(cmd); // write byte to RTC at address
//    write_byte(value);
//    DS1302_CE = 0;
//}

//void DS1302_readbyte()
//{
//    unsigned int dat = 0, i, j;
//    DS1302_CE = 1;
//    write_byte(DS1302_READ_BURST);
//    for(i = 0; i < 8; i++) // store in array
//    {
//        dat = 0;
//        for(j = 0; j < 8; j++) // read bit by bit
//        {
//            dat >>= 1;
//            if(DS1302_IO)
//            { // read to enable
//                dat |= 0x80;
//            }
//            else
//            {
//                dat &= 0x7F;
//            }
//            SCLK_Pulse();
//        }
//        read_bytes[i] = dat;
//    }
//    DS1302_CE = 0;    
//}

//void DS1302_init()
//{
//    unsigned char r;
//    DS1302_CE = 0; 
//    DS1302_CLK = 0;
//    DS1302_writebyte(DS1302_CONTROL, 0x00);
//    DS1302_writebyte(DS1302_CHARGER, 0xAB);
//    
//    r = read_byte(DS1302_SECOND_READ);                
//    if((r & DS1302_SECOND_READ) != 0)
//        DS1302_writebyte(DS1302_SECOND, 0); // reset value to zero
//}

//void DS1302_settime(unsigned char hour, unsigned char minute, unsigned char second)
//{
//    DS1302_writebyte(DS1302_SECOND, second);
//    DS1302_writebyte(DS1302_MINUTE, minute);
//    DS1302_writebyte(DS1302_HOUR, hour);
//}

//void lcd_display(unsigned char number)
//{
//    lcd_char(((number >> 4) & 0x0F) + 0x30); // tens
//    lcd_char((number & 0x0F) + 0x30); // units
//}

//void DS1302_gettime(void)
//{
//    lcd_set_cursor(0, 0);
//    
//    lcd_set_cursor(0, 6);
//    lcd_display(read_bytes[2]);
//    lcd_char(':');
//    delay(10);
//    
//    lcd_set_cursor(0, 9);
//    lcd_display(read_bytes[1]);
//    lcd_char(':');
//    delay(10);
//    
//    lcd_set_cursor(0, 12);
//    lcd_display(read_bytes[0]);
//    delay(10);
//}

//void DS1302_setdate(unsigned char day, unsigned char month, unsigned char year)
//{
//    DS1302_writebyte(DS1302_DATE, day);
//    DS1302_writebyte(DS1302_MONTH, month);
//    DS1302_writebyte(DS1302_YEAR, year);
//}

//void DS1302_getdate(void)
//{
//    lcd_set_cursor(1, 0);
//    
//    lcd_set_cursor(1, 6);
//    lcd_display(read_bytes[3]);
//    lcd_char(':');
//    delay(10);
//    
//    lcd_set_cursor(1, 9);
//    lcd_display(read_bytes[4]);
//    lcd_char(':');
//    delay(10);
//    
//    lcd_set_cursor(1, 12);
//    lcd_display(read_bytes[6]);
//    delay(10);
//}

//void main()
//{
//    lcd_init();
//    DS1302_init();
//    UART_init();
//        
//    while(1)
//    {         
//        UART_receivedata();
//        if(messagereceived)
//        {
//            if(receivedata == 'n')
//            {
//                receivemsg[index] = '\0';
//                if(receivemsg[0] == 't')
//                {
//                    lcd_command(0x80);
//                    delay(1000);
//                    
//                    extract_time_set_rtc(&receivemsg[1]);

//                    while(1)
//                    {
//                        DS1302_CE = 1;
//                        DS1302_readbyte();
//                        DS1302_CE = 0;
//                        DS1302_gettime();
//                        DS1302_getdate(); // Display the date along with time
//                    }
//                
//                    index = 0;
//                    memset(receivemsg, 0, sizeof(receivemsg));
//                    messagereceived = 0;
//                }
//            }
//            else
//            {
//                receivemsg[index] = receivedata;
//                index++;
//            }
//        }
//        else
//        {
//            lcd_string("nodata");
//        }
//    }
//}




