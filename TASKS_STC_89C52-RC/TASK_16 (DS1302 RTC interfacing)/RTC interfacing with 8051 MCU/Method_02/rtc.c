#include "rtc.h"

void RTC_Init(void) {
    CE = 0;   // Disable RTC
    SCLK = 0; // Set clock low
}

void RTC_WriteTimeDate(unsigned char timeDate[7]) {
    unsigned char i;
    CE = 1;  // Enable RTC
    Send_Byte(0xBE);  // Command for burst write

    for(i = 0; i < 7; i++) {
        Send_Byte(timeDate[i]);
    }

    CE = 0;  // Disable RTC
}

void RTC_ReadTimeDate(unsigned char timeDate[7]) {
    unsigned char i;
    CE = 1;  // Enable RTC
    Send_Byte(0xBF);  // Command for burst read

    for(i = 0; i < 7; i++) {
        timeDate[i] = Receive_Byte();
    }

    CE = 0;  // Disable RTC
}

void Send_Byte(unsigned char byte) {
    unsigned char i;
    for(i = 0; i < 8; i++) {
        IO = byte & 0x01;  // Send LSB first
        SCLK = 1;
        byte >>= 1;  // Shift right for next bit
        SCLK = 0;
    }
}

unsigned char Receive_Byte(void) {
    unsigned char i, receivedData = 0;

    for(i = 0; i < 8; i++) {
        receivedData >>= 1;  // Shift to prepare for next bit
        if(IO) receivedData |= 0x80;  // Read data bit
        SCLK = 1;
        SCLK = 0;
    }

    return receivedData;
}
