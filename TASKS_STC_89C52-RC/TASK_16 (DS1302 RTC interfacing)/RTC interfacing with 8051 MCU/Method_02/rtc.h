#ifndef RTC_H
#define RTC_H

#include <reg51.h>

// Pin Definitions
sbit CE = P3^5;    // Chip Enable (connected to P35 in schematic)
sbit SCLK = P3^6;  // Serial Clock (connected to P36 in schematic)
sbit IO = P3^4;    // Input/Output Data (connected to P34 in schematic)

// Function Prototypes
void RTC_Init(void);
void RTC_WriteTimeDate(unsigned char timeDate[7]);
void RTC_ReadTimeDate(unsigned char timeDate[7]);
void Send_Byte(unsigned char byte);
unsigned char Receive_Byte(void);

#endif
