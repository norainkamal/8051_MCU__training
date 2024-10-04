#include "rtc.h"
#include "LCD.h"

void main(void) {
    unsigned char timeDate[7] = {0x00, 0x25, 0x10, 0x03, 0x15, 0x08, 0x24}; // Example time: 10:25:00 Tuesday, 15th August 2024

    RTC_Init();
    LCD_Init(); // Initialize LCD

    // Write time and date to RTC
    RTC_WriteTimeDate(timeDate);

    // Read time and date from RTC
    RTC_ReadTimeDate(timeDate);

    // Display the time and date on the LCD
    LCD_DisplayTimeDate(timeDate);  // Display time and date

    while(1) {
        // Continuous operation
    }
}
