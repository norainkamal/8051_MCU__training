#include <reg52.h>  // Include the 8051 microcontroller header file to use its special function registers and features
#include "LCD1602.h"

// Define pin connections for the DS1302 RTC (Real-Time Clock)
sbit DS1302_SCLK = P3^6; // Define Serial Clock pin (SCLK) connected to Port 3, bit 6
sbit DS1302_IO = P3^4;   // Define Input/Output data pin (IO) connected to Port 3, bit 4
sbit DS1302_CE = P3^5;   // Define Chip Enable pin (CE) connected to Port 3, bit 5

// Define addresses for various DS1302 registers
#define DS1302_SECOND 0x80 // Register address for seconds
#define DS1302_MINUTE 0x82 // Register address for minutes
#define DS1302_HOUR   0x84 // Register address for hours
#define DS1302_DATE   0x86 // Register address for date
#define DS1302_MONTH  0x88 // Register address for month
#define DS1302_DAY    0x8A // Register address for day of the week
#define DS1302_YEAR   0x8C // Register address for year
#define DS1302_WP     0x8E // Register address for write protection (WP)

// Array to store time and date values
unsigned int DS1302_Time[] = {23, 12, 31, 23, 59, 50, 6}; // {Year, Month, Date, Hour, Minute, Second, Day}

// Initialize DS1302 RTC
void DS1302_Init() 
{
    DS1302_CE = 0;   // Set Chip Enable (CE) pin low to disable communication with DS1302
    DS1302_SCLK = 0; // Set Serial Clock (SCLK) pin low (default state for clock signal)
    DS1302_IO = 0;   // Set IO pin low (default state for data transmission)
}

// Write a byte of data to the DS1302
void DS1302_WriteByte(unsigned char Command, unsigned char Data) 
{
    unsigned int i;

    DS1302_CE = 1; // Enable communication with the DS1302 by setting CE high

    // Send the Command byte to DS1302
    for (i = 0; i < 8; i++) 
    {
        DS1302_IO = Command & (0x01 << i); // Set IO pin to the bit of Command byte (0 or 1)
        DS1302_SCLK = 1; // Pulse the Serial Clock signal to DS1302 (rising edge)
        DS1302_SCLK = 0; // Return Serial Clock signal to low (falling edge)
    }

    // Send the Data byte to DS1302
    for (i = 0; i < 8; i++) 
    {
        DS1302_IO = Data & (0x01 << i); // Set IO pin to the bit of Data byte (0 or 1)
        DS1302_SCLK = 1; // Pulse the Serial Clock signal to DS1302 (rising edge)
        DS1302_SCLK = 0; // Return Serial Clock signal to low (falling edge)
    }

    DS1302_CE = 0; // Disable communication with the DS1302 by setting CE low
}

// Read a byte of data from the DS1302
unsigned char DS1302_ReadByte(unsigned char Command) 
{
    unsigned char i, Data = 0x00; // Initialize Data to 0
    Command |= 0x01; // Set the read bit (0x01) in Command to read data from DS1302

    DS1302_CE = 1; // Enable communication with the DS1302 by setting CE high

    // Send the Command byte to DS1302
    for (i = 0; i < 8; i++) 
    {
        DS1302_IO = Command & (0x01 << i); // Set IO pin to the bit of Command byte (0 or 1)
        DS1302_SCLK = 0; // Pulse the Serial Clock signal to DS1302 (falling edge)
        DS1302_SCLK = 1; // Return Serial Clock signal to high (rising edge)
    }

    DS1302_IO = 1; // Set IO pin to input mode to read data from DS1302

    // Read the Data byte from DS1302
    for (i = 0; i < 8; i++) 
    {
        DS1302_SCLK = 1; // Pulse the Serial Clock signal to DS1302 (rising edge)
        DS1302_SCLK = 0; // Return Serial Clock signal to low (falling edge)

        if (DS1302_IO) // Check if the data bit is high
        {
            Data |= (0x01 << i); // Set the corresponding bit in Data
        }
    }

    DS1302_CE = 0; // Disable communication with the DS1302 by setting CE low
    DS1302_IO = 0; // Set IO pin back to low (prepare for next operation)

    return Data; // Return the read data
}

// Set the current time and date on the DS1302
void DS1302_SetTime()
{
    // Disable write protection to allow setting time
    DS1302_WriteByte(DS1302_WP, 0x00);

    // Convert time and date values from decimal to BCD (Binary-Coded Decimal) and write to DS1302
    DS1302_WriteByte(DS1302_YEAR, DS1302_Time[0] / 10 * 16 + DS1302_Time[0] % 10); // Set year
    DS1302_WriteByte(DS1302_MONTH, DS1302_Time[1] / 10 * 16 + DS1302_Time[1] % 10); // Set month
    DS1302_WriteByte(DS1302_DATE, DS1302_Time[2] / 10 * 16 + DS1302_Time[2] % 10); // Set date
    DS1302_WriteByte(DS1302_HOUR, DS1302_Time[3] / 10 * 16 + DS1302_Time[3] % 10); // Set hour
    DS1302_WriteByte(DS1302_MINUTE, DS1302_Time[4] / 10 * 16 + DS1302_Time[4] % 10); // Set minute
    DS1302_WriteByte(DS1302_SECOND, DS1302_Time[5] / 10 * 16 + DS1302_Time[5] % 10); // Set second
    DS1302_WriteByte(DS1302_DAY, DS1302_Time[6] / 10 * 16 + DS1302_Time[6] % 10); // Set day of the week

    // Re-enable write protection to prevent further changes
    DS1302_WriteByte(DS1302_WP, 0x80);
}

// Read the current time and date from the DS1302
void DS1302_ReadTime()
{
    unsigned int Temp;

    // Read and convert each time and date component from BCD (Binary-Coded Decimal) format to decimal
    Temp = DS1302_ReadByte(DS1302_YEAR);  // Read year
    DS1302_Time[0] = Temp / 16 * 10 + Temp % 16; // Convert from BCD to decimal

    Temp = DS1302_ReadByte(DS1302_MONTH); // Read month
    DS1302_Time[1] = Temp / 16 * 10 + Temp % 16; // Convert from BCD to decimal

    Temp = DS1302_ReadByte(DS1302_DATE); // Read date
    DS1302_Time[2] = Temp / 16 * 10 + Temp % 16; // Convert from BCD to decimal

    Temp = DS1302_ReadByte(DS1302_HOUR); // Read hour
    DS1302_Time[3] = Temp / 16 * 10 + Temp % 16; // Convert from BCD to decimal

    Temp = DS1302_ReadByte(DS1302_MINUTE); // Read minute
    DS1302_Time[4] = Temp / 16 * 10 + Temp % 16; // Convert from BCD to decimal

    Temp = DS1302_ReadByte(DS1302_SECOND); // Read second
    DS1302_Time[5] = Temp / 16 * 10 + Temp % 16; // Convert from BCD to decimal

    Temp = DS1302_ReadByte(DS1302_DAY); // Read day of the week
    DS1302_Time[6] = Temp / 16 * 10 + Temp % 16; // Convert from BCD to decimal
}


//	void DS1302_ReadTime() {
//			unsigned char hour;
//			unsigned char is_pm = 0; // Declare and initialize is_pm

//			// Read the time from DS1302
//			DS1302_Time[0] = DS1302_ReadByte(0x81); // Read second
//			DS1302_Time[1] = DS1302_ReadByte(0x83); // Read minute
//			hour = DS1302_ReadByte(0x85); // Read hour

//			// Determine AM/PM and convert to 12-hour format if needed
//			if (hour & 0x80) { // If the 12/24-hour mode bit is set
//					is_pm = (hour & 0x20) ? 1 : 0; // PM if bit 5 is set
//					hour &= 0x1F; // Clear the mode bits
//					if (hour == 0) hour = 12; // Convert 0 hours to 12 (midnight)
//					if (hour > 12) hour -= 12; // Convert to 12-hour format
//			}

//			DS1302_Time[2] = hour; // Store hour

//			// Display time on LCD
//			LCD_ShowNum(2, 7, DS1302_Time[2], 2);  // Display hour
//			LCD_ShowChar(2, 9, ':');
//			LCD_ShowNum(2, 10, DS1302_Time[1], 2); // Display minute
//			LCD_ShowChar(2, 12, ':');
//			LCD_ShowNum(2, 13, DS1302_Time[0], 2); // Display second

//			// Display AM/PM on LCD
//			if (is_pm) {
//					LCD_ShowString(2, 15, "PM");
//			} else {
//					LCD_ShowString(2, 15, "AM");
//			}
//	}
