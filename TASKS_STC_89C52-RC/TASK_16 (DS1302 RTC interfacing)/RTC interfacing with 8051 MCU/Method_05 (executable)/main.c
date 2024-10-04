#include <reg51.h>             // Include the header file for 8051 microcontroller
#include "LCD.h"               // Include the custom header file for LCD functions

// Define pins connected to the DS1302 RTC
sbit DS1302_IO =  P3^4;        // Data I/O pin of DS1302 connected to P3.4
sbit DS1302_CE =  P3^5;        // Chip Enable pin of DS1302 connected to P3.5
sbit DS1302_CLK = P3^6;        // Clock pin of DS1302 connected to P3.6

// Define constants for DS1302 commands
#define DS1302_READ_BURST      0xBF   // Command to read in burst mode
#define DS1302_WRITE_BURST     0xBE   // Command to write in burst mode
#define DS1302_SECOND          0x80   // Command to write seconds
#define DS1302_SECOND_READ     0x81   // Command to read seconds
#define DS1302_MINUTE          0x82   // Command to write minutes
#define DS1302_MINUTE_READ     0x83   // Command to read minutes
#define DS1302_HOUR            0x84   // Command to write hours
#define DS1302_HOUR_READ       0x85   // Command to read hours
#define DS1302_DATE            0x86   // Command to write date
#define DS1302_MONTH           0x88   // Command to write month
#define DS1302_DAY             0x8A   // Command to write day of the week
#define DS1302_YEAR            0x8C   // Command to write year
#define DS1302_CONTROL         0x8E   // Command to access control register
#define DS1302_CHARGER         0x90   // Command to access trickle charger register
#define DS1302_RAMBURST_READ   0xFF   // Command to read RAM in burst mode
#define DS1302_RAMBURST_WRITE  0xFE   // Command to write RAM in burst mode
#define CLK_DELAY              10     // Define a constant for clock delay

// Function prototypes
void write_byte(unsigned int byte);         // Prototype for function to write a byte to DS1302
unsigned char read_byte(unsigned char cmd); // Prototype for function to read a byte from DS1302
void DS1302_settime(unsigned char hour, unsigned char minute, unsigned char second); // Prototype to set time on DS1302
void DS1302_setdate(unsigned char day, unsigned char month, unsigned char year);     // Prototype to set date on DS1302
void DS1302_gettime(void); // Prototype to get time from DS1302
void DS1302_getdate(void); // Prototype to get date from DS1302
void send_bit(unsigned int bitt); // Prototype for function to send a single bit to DS1302
void DS1302_init();     // Prototype for DS1302 initialization function
void DS1302_readbyte(); // Prototype for function to read bytes in burst mode

unsigned int read_bytes[12]; // Array to store bytes read from DS1302

// Function to generate a clock pulse
void SCLK_Pulse()
{
    DS1302_CLK = 1;   // Set clock pin high
    DS1302_CLK = 0;   // Set clock pin low to complete the pulse
}

// Function to send a single bit to DS1302
void send_bit(unsigned int bitt)
{
    if (bitt != 0)
    {
        DS1302_IO = 1;  // If the bit is 1, set the IO pin high
    }
    else
    {
        DS1302_IO = 0;  // If the bit is 0, set the IO pin low
    }
}

// Function to write a byte to DS1302
void write_byte(unsigned int byte)
{
    unsigned int i, Bit;
    for (i = 0; i < 8; i++)  // Loop through each bit in the byte
    {
        Bit = byte & (1 << i); // Extract the current bit to send
        send_bit(Bit);         // Send the bit to DS1302
        SCLK_Pulse();          // Pulse the clock to latch the bit
    }
}

// Function to read a byte from DS1302
unsigned char read_byte(unsigned char cmd)
{
    unsigned int j, dat = 0;
    write_byte(cmd);  // Send the command to DS1302 to prepare it for reading
    for (j = 0; j < 8; j++)  // Loop through each bit in the byte
    {
        dat >>= 1;  // Shift the previously read bits to the right
        if (DS1302_IO)  // If the IO pin is high
        {
            dat |= 0x80;  // Set the most significant bit of dat to 1
        }
        else
        {
            dat &= 0x7F;  // Ensure the most significant bit of dat is 0
        }
        SCLK_Pulse();  // Pulse the clock to shift in the next bit
    }
    return dat;  // Return the byte read from DS1302
}

// Function to write a byte to a specific DS1302 register
void DS1302_writebyte(unsigned char cmd, unsigned char value)
{
    DS1302_CE = 1;        // Enable the DS1302 by setting CE high
    write_byte(cmd);      // Send the register address to DS1302
    write_byte(value);    // Send the value to be written to DS1302
    DS1302_CE = 0;        // Disable the DS1302 by setting CE low
}

// Function to read multiple bytes from DS1302 in burst mode
void DS1302_readbyte()
{
    unsigned int dat = 0, i, j;
    DS1302_CE = 1;  // Enable the DS1302 by setting CE high
    
    write_byte(DS1302_READ_BURST);  // Send the command to start burst read
    for (i = 0; i < 8; i++)  // Read 8 bytes in burst mode
    {
        dat = 0;
        for (j = 0; j < 8; j++)  // Loop through each bit in the byte
        {
            dat >>= 1;  // Shift previously read bits to the right
            if (DS1302_IO)  // If the IO pin is high
            {
                dat |= 0x80;  // Set the most significant bit of dat to 1
            }
            else
            {
                dat &= 0x7F;  // Ensure the most significant bit of dat is 0
            }
            SCLK_Pulse();  // Pulse the clock to shift in the next bit
        }
        read_bytes[i] = dat;  // Store the byte in the read_bytes array
    }    
    DS1302_CE = 0;  // Disable the DS1302 by setting CE low
}

// Function to initialize the DS1302 RTC
void DS1302_init()
{
    unsigned char r;
    DS1302_CE = 0;       // Disable the DS1302 initially
    DS1302_CLK = 0;      // Set the clock pin low initially
    DS1302_writebyte(DS1302_CONTROL, 0x00);  // Disable write protection
    DS1302_writebyte(DS1302_CHARGER, 0xAB);  // Configure the trickle charger
    
    r = read_byte(DS1302_SECOND_READ);  // Read the current seconds value
    if ((r & DS1302_SECOND_READ) != 0)  // Check if the oscillator is halted
    {
        DS1302_writebyte(DS1302_SECOND, 0);  // If halted, reset seconds to 0
    }
}

// Function to set the time on the DS1302 RTC
void DS1302_settime(unsigned char hour, unsigned char minute, unsigned char second)
{
    DS1302_writebyte(DS1302_SECOND, second);  // Write the seconds value
    DS1302_writebyte(DS1302_MINUTE, minute);  // Write the minutes value
    DS1302_writebyte(DS1302_HOUR, hour);      // Write the hours value
}

// Function to display a number on the LCD in BCD format
void lcd_display(unsigned char number)
{
    lcd_char(((number >> 4) & 0x0F) + 0x30); // Display the tens digit
    lcd_char((number & 0x0F) + 0x30);        // Display the units digit
}

// Function to get the time from the DS1302 RTC and display it on the LCD
void DS1302_gettime(void)
{
    lcd_set_cursor(0, 0);         // Set cursor to the beginning of the first line
    lcd_string("TIME:");          // Display the label "TIME:"
    
    lcd_set_cursor(0, 6);         // Set cursor to display the hour
    lcd_display(read_bytes[2]);   // Display the hour value
    lcd_char(':');                // Display the separator ":"
    delay(10);                    // Delay for stability
    
    lcd_set_cursor(0, 9);         // Set cursor to display the minute
    lcd_display(read_bytes[1]);   // Display the minute value
    lcd_char(':');                // Display the separator ":"
    delay(10);                    // Delay for stability
    
    lcd_set_cursor(0, 12);        // Set cursor to display the second
    lcd_display(read_bytes[0]);   // Display the second value
    delay(10);                    // Delay for stability
}

// Function to set the date on the DS1302 RTC
void DS1302_setdate(unsigned char day, unsigned char month, unsigned char year)
{
    DS1302_writebyte(DS1302_DATE, day);    // Write the day value
    DS1302_writebyte(DS1302_MONTH, month); // Write the month value
    DS1302_writebyte(DS1302_YEAR, year);   // Write the year value
}

// Function to get the date from the DS1302 RTC and display it on the LCD
void DS1302_getdate(void)
{
    lcd_set_cursor(1, 0);         // Set cursor to the beginning of the second line
    lcd_string("DATE:");          // Display the label "DATE:"
    
    lcd_set_cursor(1, 6);         // Set cursor to display the day
    lcd_display(read_bytes[3]);   // Display the day value
    lcd_char(':');                // Display the separator ":"
    delay(10);                    // Delay for stability
    
    lcd_set_cursor(1, 9);         // Set cursor to display the month
    lcd_display(read_bytes[4]);   // Display the month value
    lcd_char(':');                // Display the separator ":"
    delay(10);                    // Delay for stability
    
    lcd_set_cursor(1, 12);        // Set cursor to display the year
    lcd_display(read_bytes[6]);   // Display the year value
    delay(10);                    // Delay for stability
}

// Main function
void main()
{
    lcd_init();                    // Initialize the LCD
    DS1302_init();                 // Initialize the DS1302 RTC
    DS1302_settime(0x23, 0x59, 0x59);  // Set the initial time to 23:59:50
    DS1302_setdate(0x31, 0x12, 0x23);  // Set the initial date to 31/12/23
    
    while (1)  // Infinite loop
    {
        DS1302_CE = 1;             // Enable the DS1302 by setting CE high
        DS1302_readbyte();         // Read the current time and date values
        DS1302_CE = 0;             // Disable the DS1302 by setting CE low
        
        DS1302_gettime();          // Display the time on the LCD
        DS1302_getdate();          // Display the date on the LCD
    }
}
