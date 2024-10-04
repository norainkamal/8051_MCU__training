
////				#include <reg52.h>
////				#include <intrins.h>

////				sbit SCLK = P3^6; // Serial Clock
////				sbit IO = P3^4;   // Data I/O
////				sbit CE = P3^5;   // Chip Enable

////				sbit RS = P2^6;
////				sbit RW = P2^5;
////				sbit EN = P2^7;

////				unsigned char sec;
////					


////				void delay(unsigned int ms) {
////						while(ms--) _nop_();
////				}

////				void LCD_Command(unsigned char command) {
////						RS = 0; 
////						RW = 0; 
////						P0 = command;
////						EN = 1;
////						delay(5);
////						EN = 0;
////				}

////				void LCD_Init() {
////						LCD_Command(0x38);  // 8-bit mode, 2 lines
////						LCD_Command(0x0C);  // Display on, cursor off
////						LCD_Command(0x06);  // Increment cursor
////						LCD_Command(0x01);  // Clear display
////				}

////				void LCD_WriteChar(char c) {
////						RS = 1;
////						RW = 0;
////						P0 = c;
////						EN = 1;
////						delay(5);
////						EN = 0;
////				}

////				void LCD_WriteString(char *str) {
////						while(*str) {
////								LCD_WriteChar(*str++);
////						}
////				}




////				// ---------------------------------------------------------------------------------------------------------------------------------


////				void DS1302_Init() {
////						CE = 0;  // Disable DS1302
////						SCLK = 0;  // Initialize clock line
////				}



////				void DS1302_WriteByte(unsigned char dat) {
////					unsigned char i;
////						for (i = 0; i < 8; i++) {
////								IO = dat & 0x01;
////								SCLK = 1;
////								dat >>= 1;
////								SCLK = 0;
////						}
////				}


////				unsigned char DS1302_ReadByte() {
////						unsigned char dat = 0;
////						unsigned char i;
////					
////						for (i = 0; i < 8; i++) {
////								dat >>= 1;
////								if (IO) dat |= 0x80;
////								SCLK = 1;
////								SCLK = 0;
////						}
////						return dat;
////				}



////				void DS1302_WriteTime(unsigned char address, unsigned char dataa) {
////						CE = 1;  // Enable DS1302
////						DS1302_WriteByte(address);  // Send address
////						DS1302_WriteByte(dataa);  // Send data
////						CE = 0;  // Disable DS1302
////				}



////				unsigned char DS1302_ReadTime(unsigned char address) {
////						unsigned char dataa;
////						CE = 1;  // Enable DS1302
////						DS1302_WriteByte(address | 0x01);  // Send address with read command
////						dataa = DS1302_ReadByte();  // Read data
////						CE = 0;// Disable DS1302
////						return dataa;
////				}


////				void Display_Time(unsigned char hour, unsigned char min, unsigned char sec) {
////						char time[9];
////						time[0] = (hour / 10) + '0';
////						time[1] = (hour % 10) + '0';
////						time[2] = ':';
////						time[3] = (min / 10) + '0';
////						time[4] = (min % 10) + '0';
////						time[5] = ':';
////						time[6] = (sec / 10) + '0';
////						time[7] = (sec % 10) + '0';
////						time[8] = '\0';
////						
////						LCD_Command(0x80);  // Move cursor to the start of the first line
////						LCD_WriteString(time);
////				}


////				////void main() {
////				////		
////				////    DS1302_Init();
////				////    DS1302_WriteTime(0x80, 0x50);  // Write seconds (example)
////				////    sec = DS1302_ReadTime(0x81);  // Read seconds
////				////    // Further processing and display
////				////}




////				void main() {
////						unsigned char sec, min, hour;
////						
////						DS1302_Init();
////						LCD_Init();
////						
////						while(1) {
////								sec = DS1302_ReadTime(0x81);  // Read seconds
////								min = DS1302_ReadTime(0x83);  // Read minutes
////								hour = DS1302_ReadTime(0x85);  // Read hours
////								
////								Display_Time(hour, min, sec);
////								
////								delay(1000);  // Delay for 1 second
////						}
////				}









/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/








//		#include <reg52.h>
//		#include <intrins.h>

//		// Pin Definitions
//		sbit SCLK = P3^6; // Serial Clock
//		sbit IO = P3^4;   // Data I/O
//		sbit CE = P3^5;   // Chip Enable

//		sbit RS = P2^6;    // LCD Register Select
//		sbit RW = P2^5;    // LCD Read/Write
//		sbit EN = P2^7;    // LCD Enable

//		// Function Prototypes
//		unsigned char BCD_to_BIN(unsigned char val);
//		void delay_ms(unsigned int ms);
//		void LCD_Command(unsigned char command);
//		void LCD_Init(void);
//		void LCD_WriteChar(char c);
//		void LCD_WriteString(char *str);
//		void DS1302_Init(void);
//		void DS1302_WriteByte(unsigned char dat);
//		unsigned char DS1302_ReadByte(void);
//		void DS1302_WriteTime(unsigned char address, unsigned char dataa);
//		unsigned char DS1302_ReadTime(unsigned char address);
//		void Display_Time(unsigned char hour, unsigned char min, unsigned char sec);
//		void Display_Date(unsigned char date, unsigned char month, unsigned char year);

//		// Function to convert BCD to Binary
//		unsigned char BCD_to_BIN(unsigned char val) {
//				return ((val >> 4) * 10) + (val & 0x0F);
//		}

//		// Simple Delay Function (Approximate 1 ms per call)
//		void delay_ms(unsigned int ms) {
//				unsigned int i, j;
//				for(i = 0; i < ms; i++)
//						for(j = 0; j < 120; j++); // Adjust inner loop for accurate delay if necessary
//		}

//		// LCD Command Function
//		void LCD_Command(unsigned char command) {
//				RS = 0; 
//				RW = 0; 
//				P0 = command;
//				EN = 1;
//				delay_ms(1);
//				EN = 0;
//		}

//		// Initialize LCD
//		void LCD_Init(void) {
//				LCD_Command(0x38);  // 8-bit mode, 2 lines, 5x7 dots
//				LCD_Command(0x0C);  // Display ON, Cursor OFF
//				LCD_Command(0x06);  // Entry mode: Increment cursor
//				LCD_Command(0x01);  // Clear display
//				delay_ms(2);
//		}

//		// Write Character to LCD
//		void LCD_WriteChar(char c) {
//				RS = 1;
//				RW = 0;
//				P0 = c;
//				EN = 1;
//				delay_ms(1);
//				EN = 0;
//		}

//		// Write String to LCD
//		void LCD_WriteString(char *str) {
//				while(*str) {
//						LCD_WriteChar(*str++);
//				}
//		}

//		// Initialize DS1302
//		void DS1302_Init(void) {
//				CE = 0;  // Disable DS1302
//				SCLK = 0;  // Initialize clock line
//				delay_ms(1);
//				
//				// Optionally, disable write protection by writing to control register (0x8E)
//				DS1302_WriteTime(0x8E, 0x00); // Disable write protection
//		}

//		// Write Byte to DS1302
//		void DS1302_WriteByte(unsigned char dat) {
//				unsigned char i;
//				for (i = 0; i < 8; i++) {
//						IO = dat & 0x01;
//						SCLK = 1;
//						delay_ms(1);
//						SCLK = 0;
//						dat >>= 1;
//				}
//		}

//		// Read Byte from DS1302
//		unsigned char DS1302_ReadByte(void) {
//				unsigned char dat = 0;
//				unsigned char i;
//				
//				IO = 1; // Configure IO as input (open-drain high)
//				for (i = 0; i < 8; i++) {
//						dat >>= 1;
//						if (IO) dat |= 0x80;
//						SCLK = 1;
//						delay_ms(1);
//						SCLK = 0;
//				}
//				return dat;
//		}

//		// Write Time/Date to DS1302
//		void DS1302_WriteTime(unsigned char address, unsigned char dataa) {
//				CE = 1;  // Enable DS1302
//				delay_ms(1);
//				DS1302_WriteByte(address);  // Send address
//				DS1302_WriteByte(dataa);    // Send data
//				CE = 0;  // Disable DS1302
//				delay_ms(1);
//		}

//		// Read Time/Date from DS1302
//		unsigned char DS1302_ReadTime(unsigned char address) {
//				unsigned char dataa;
//				CE = 1;  // Enable DS1302
//				delay_ms(1);
//				DS1302_WriteByte(address | 0x01);  // Send address with read command
//				dataa = DS1302_ReadByte();         // Read data
//				CE = 0;  // Disable DS1302
//				delay_ms(1);
//				return dataa;
//		}

//		// Display Time on LCD
//		void Display_Time(unsigned char hour, unsigned char min, unsigned char sec) {
//				char timeStr[17];
//				unsigned char index = 0;
//				
//				// Format: "Time: hh:mm:ss"
//				timeStr[index++] = 'T';
//				timeStr[index++] = 'i';
//				timeStr[index++] = 'm';
//				timeStr[index++] = 'e';
//				timeStr[index++] = ':';
//				timeStr[index++] = ' ';
//				timeStr[index++] = (hour / 10) + '0';
//				timeStr[index++] = (hour % 10) + '0';
//				timeStr[index++] = ':';
//				timeStr[index++] = (min / 10) + '0';
//				timeStr[index++] = (min % 10) + '0';
//				timeStr[index++] = ':';
//				timeStr[index++] = (sec / 10) + '0';
//				timeStr[index++] = (sec % 10) + '0';
//				timeStr[index++] = ' ';
//				timeStr[index++] = ' ';
//				timeStr[index] = '\0';
//				
//				LCD_Command(0x80);  // Move cursor to the first line
//				LCD_WriteString(timeStr);
//		}

//		// Display Date on LCD
//		void Display_Date(unsigned char date, unsigned char month, unsigned char year) {
//				char dateStr[17];
//				unsigned char index = 0;
//				
//				// Format: "Date: dd/mm/yy"
//				dateStr[index++] = 'D';
//				dateStr[index++] = 'a';
//				dateStr[index++] = 't';
//				dateStr[index++] = 'e';
//				dateStr[index++] = ':';
//				dateStr[index++] = ' ';
//				dateStr[index++] = (date / 10) + '0';
//				dateStr[index++] = (date % 10) + '0';
//				dateStr[index++] = '/';
//				dateStr[index++] = (month / 10) + '0';
//				dateStr[index++] = (month % 10) + '0';
//				dateStr[index++] = '/';
//				dateStr[index++] = (year / 10) + '0';
//				dateStr[index++] = (year % 10) + '0';
//				dateStr[index++] = ' ';
//				dateStr[index++] = ' ';
//				dateStr[index] = '\0';
//				
//				LCD_Command(0xC0);  // Move cursor to the second line
//				LCD_WriteString(dateStr);
//		}

//		void main(void) {
//				unsigned char sec_bcd, min_bcd, hour_bcd;
//				unsigned char date_bcd, month_bcd, year_bcd;
//				unsigned char sec, min, hour;
//				unsigned char date, month, year;
//				
//				DS1302_Init();
//				LCD_Init();
//				
//				while(1) {
//						// Read BCD values from DS1302
//						sec_bcd   = DS1302_ReadTime(0x81);  // Seconds
//						min_bcd   = DS1302_ReadTime(0x83);  // Minutes
//						hour_bcd  = DS1302_ReadTime(0x85);  // Hours
//						date_bcd  = DS1302_ReadTime(0x87);  // Date (Day of Month)
//						month_bcd = DS1302_ReadTime(0x89);  // Month
//						year_bcd  = DS1302_ReadTime(0x8D);  // Year
//						
//						// Convert BCD to Binary
//						sec   = BCD_to_BIN(sec_bcd);
//						min   = BCD_to_BIN(min_bcd);
//						hour  = BCD_to_BIN(hour_bcd);
//						date  = BCD_to_BIN(date_bcd);
//						month = BCD_to_BIN(month_bcd);
//						year  = BCD_to_BIN(year_bcd);
//						
//						// Display Time and Date
//						Display_Time(hour, min, sec);
//						Display_Date(date, month, year);
//						
//						delay_ms(1000);  // Wait for 1 second
//				}
//		}








// _______________________________________________________________________________________________________________________________________________________________________


#include <reg52.h>
#include <intrins.h>

sbit SCLK = P3^6; // Serial Clock
sbit IO = P3^4;   // Data I/O
sbit CE = P3^5;   // Chip Enable

sbit RS = P2^6;
sbit RW = P2^5;
sbit EN = P2^7;

void delay() {
    unsigned int i;
    for(i=0; i<1000; i++) {  // 1ms * 1000 = 1s
        TH0 = 0xFC;          // Load initial value for 1ms delay
        TL0 = 0x66;
        TR0 = 1;             // Start Timer0
        while(!TF0);         // Wait for Timer0 to overflow
        TR0 = 0;             // Stop Timer0
        TF0 = 0;             // Clear Timer0 overflow flag
    }
}

void LCD_Command(unsigned char command) {
    RS = 0; 
    RW = 0; 
    P0 = command;
    EN = 1;
    delay();
    EN = 0;
}

void LCD_Init() {
    LCD_Command(0x38);  // 8-bit mode, 2 lines
    LCD_Command(0x0C);  // Display on, cursor off
    LCD_Command(0x06);  // Increment cursor
    LCD_Command(0x01);  // Clear display
}

void LCD_WriteChar(char c) {
    RS = 1;
    RW = 0;
    P0 = c;
    EN = 1;
    delay();
    EN = 0;
}

void LCD_WriteString(char *str) {
    while(*str) {
        LCD_WriteChar(*str++);
    }
}

void DS1302_Init() {
    CE = 0;  // Disable DS1302
    SCLK = 0;  // Initialize clock line
}

void DS1302_WriteByte(unsigned char dat) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        IO = dat & 0x01;
        SCLK = 1;
        dat >>= 1;
        SCLK = 0;
    }
}

unsigned char DS1302_ReadByte() {
    unsigned char dat = 0;
    unsigned char i;
    
    for (i = 0; i < 8; i++) {
        dat >>= 1;
        if (IO) dat |= 0x80;
        SCLK = 1;
        SCLK = 0;
    }
    return dat;
}

void DS1302_WriteTime(unsigned char address, unsigned char dataa) {
    CE = 1;  // Enable DS1302
    DS1302_WriteByte(address);  // Send address
    DS1302_WriteByte(dataa);  // Send data
    CE = 0;  // Disable DS1302
}

unsigned char DS1302_ReadTime(unsigned char address) {
    unsigned char dataa;
    CE = 1;  // Enable DS1302
    DS1302_WriteByte(address | 0x01);  // Send address with read command
    dataa = DS1302_ReadByte();  // Read data
    CE = 0;  // Disable DS1302
    return dataa;
}

void Display_Date(unsigned char day, unsigned char month, unsigned char year) {
    char date[11];
    date[0] = (day / 10) + '0';
    date[1] = (day % 10) + '0';
    date[2] = '/';
    date[3] = (month / 10) + '0';
    date[4] = (month % 10) + '0';
    date[5] = '/';
    date[6] = '2';
    date[7] = '0';
    date[8] = (year / 10) + '0';
    date[9] = (year % 10) + '0';
    date[10] = '\0';
    
    LCD_Command(0x80);  // Move cursor to the start of the first line
    LCD_WriteString("Date: ");
    LCD_WriteString(date);
}

void Display_Time(unsigned char hour, unsigned char min, unsigned char sec) {
    char time[9];
    time[0] = (hour / 10) + '0';
    time[1] = (hour % 10) + '0';
    time[2] = ':';
    time[3] = (min / 10) + '0';
    time[4] = (min % 10) + '0';
    time[5] = ':';
    time[6] = (sec / 10) + '0';
    time[7] = (sec % 10) + '0';
    time[8] = '\0';
    
    LCD_Command(0xC0);  // Move cursor to the start of the second line
    LCD_WriteString("Time: ");
    LCD_WriteString(time);
}

void main() {
    unsigned char sec, min, hour, day, month, year;
    
    DS1302_Init();
    LCD_Init();
    
    while(1) {
        sec = DS1302_ReadTime(0x81);  // Read seconds
        min = DS1302_ReadTime(0x83);  // Read minutes
        hour = DS1302_ReadTime(0x85);  // Read hours
        day = DS1302_ReadTime(0x87);  // Read day
        month = DS1302_ReadTime(0x89);  // Read month
        year = DS1302_ReadTime(0x8C);  // Read year
        
        Display_Date(day, month, year);  // Display the date
        Display_Time(hour, min, sec);  // Display the time
        
        delay();  // Delay for 1 second
    }
}





















////#include <reg52.h>
////#include <intrins.h>

////sbit SCLK = P3^6; // Serial Clock
////sbit IO = P3^4;   // Data I/O
////sbit CE = P3^5;   // Chip Enable

////sbit RS = P2^6;
////sbit RW = P2^5;
////sbit EN = P2^7;

////void Timer0_Delay1s() {
////    unsigned int i;
////    for(i=0; i<1000; i++) {  // 1ms * 1000 = 1s
////        TH0 = 0xFC;          // Load initial value for 1ms delay
////        TL0 = 0x66;
////        TR0 = 1;             // Start Timer0
////        while(!TF0);         // Wait for Timer0 to overflow
////        TR0 = 0;             // Stop Timer0
////        TF0 = 0;             // Clear Timer0 overflow flag
////    }
////}

////void LCD_Command(unsigned char command) {
////    RS = 0; 
////    RW = 0; 
////    P0 = command;
////    EN = 1;
////    EN = 0;
////    Timer0_Delay1s(); // Replace with appropriate short delay function
////}

////void LCD_Init() {
////    LCD_Command(0x38);  // 8-bit mode, 2 lines
////    LCD_Command(0x0C);  // Display on, cursor off
////    LCD_Command(0x06);  // Increment cursor
////    LCD_Command(0x01);  // Clear display
////}

////void LCD_WriteChar(char c) {
////    RS = 1;
////    RW = 0;
////    P0 = c;
////    EN = 1;
////    EN = 0;
////    Timer0_Delay1s(); // Replace with appropriate short delay function
////}

////void LCD_WriteString(char *str) {
////    while(*str) {
////        LCD_WriteChar(*str++);
////    }
////}

////void DS1302_Init() {
////    CE = 0;  // Disable DS1302
////    SCLK = 0;  // Initialize clock line
////}

////void DS1302_WriteByte(unsigned char dat) {
////    unsigned char i;
////    for (i = 0; i < 8; i++) {
////        IO = dat & 0x01;
////        SCLK = 1;
////        dat >>= 1;
////        SCLK = 0;
////    }
////}

////unsigned char DS1302_ReadByte() {
////    unsigned char dat = 0;
////    unsigned char i;
////    
////    for (i = 0; i < 8; i++) {
////        dat >>= 1;
////        if (IO) dat |= 0x80;
////        SCLK = 1;
////        SCLK = 0;
////    }
////    return dat;
////}

////void DS1302_WriteTime(unsigned char address, unsigned char dataa) {
////    CE = 1;  // Enable DS1302
////    DS1302_WriteByte(address);  // Send address
////    DS1302_WriteByte(dataa);  // Send data
////    CE = 0;  // Disable DS1302
////}

////unsigned char DS1302_ReadTime(unsigned char address) {
////    unsigned char dataa;
////    CE = 1;  // Enable DS1302
////    DS1302_WriteByte(address | 0x01);  // Send address with read command
////    dataa = DS1302_ReadByte();  // Read data
////    CE = 0;  // Disable DS1302
////    return dataa;
////}

////void Display_Date(unsigned char day, unsigned char month, unsigned char year) {
////    char date[11];
////    date[0] = (day / 10) + '0';
////    date[1] = (day % 10) + '0';
////    date[2] = '/';
////    date[3] = (month / 10) + '0';
////    date[4] = (month % 10) + '0';
////    date[5] = '/';
////    date[6] = '2';
////    date[7] = '0';
////    date[8] = (year / 10) + '0';
////    date[9] = (year % 10) + '0';
////    date[10] = '\0';
////    
////    LCD_Command(0x80);  // Move cursor to the start of the first line
////    LCD_WriteString("Date: ");
////    LCD_WriteString(date);
////}

////void Display_Time(unsigned char hour, unsigned char min, unsigned char sec) {
////    char time[9];
////    time[0] = (hour / 10) + '0';
////    time[1] = (hour % 10) + '0';
////    time[2] = ':';
////    time[3] = (min / 10) + '0';
////    time[4] = (min % 10) + '0';
////    time[5] = ':';
////    time[6] = (sec / 10) + '0';
////    time[7] = (sec % 10) + '0';
////    time[8] = '\0';
////    
////    LCD_Command(0xC0);  // Move cursor to the start of the second line
////    LCD_WriteString("Time: ");
////    LCD_WriteString(time);
////}

////void main() {
////    unsigned char sec, min, hour, day, month, year;
////    
////    DS1302_Init();
////    LCD_Init();
////    
////    while(1) {
////        sec = DS1302_ReadTime(0x81);  // Read seconds
////        min = DS1302_ReadTime(0x83);  // Read minutes
////        hour = DS1302_ReadTime(0x85);  // Read hours
////        day = DS1302_ReadTime(0x87);  // Read day
////        month = DS1302_ReadTime(0x89);  // Read month
////        year = DS1302_ReadTime(0x8C);  // Read year
////        
////        Display_Date(day, month, year);  // Display the date
////        Display_Time(hour, min, sec);  // Display the time
////        
////        Timer0_Delay1s();  // Delay for 1 second
////    }
////}
