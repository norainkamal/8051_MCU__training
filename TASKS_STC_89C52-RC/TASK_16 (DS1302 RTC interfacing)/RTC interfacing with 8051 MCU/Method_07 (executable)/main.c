/*  https://blog.csdn.net/m0_74068921/article/details/131745259  */

/* Include header files for special function register definitions, DS1302 RTC, and LCD1602 */
#include <reg52.h>      // Include register definitions for the 8051 microcontroller
#include "DS1302.h"    // Include header for DS1302 RTC functions
#include "LCD1602.h"   // Include header for LCD1602 display functions

void main() {
    /* Initialize the LCD and DS1302 RTC modules */
    LCD_Init();        // Set up the LCD with default settings
    DS1302_Init();     // Initialize the DS1302 RTC to prepare it for communication

    /* Set up the LCD with static text labels */
    LCD_ShowString(1, 1, "Date: "); // Display "Date: " on the first row starting from column 1
    LCD_ShowString(2, 1, "Time: "); // Display "Time: " on the second row starting from column 1

    DS1302_SetTime();  // Set the time on the DS1302 RTC

    while (1) {        // Infinite loop to continuously update the display
        DS1302_ReadTime(); // Read the current time and date from the DS1302 RTC

        /* Display Date on the LCD */
        LCD_ShowNum(1, 7, DS1302_Time[2], 2); // Show the day on the first row, starting at column 7
        LCD_ShowChar(1, 9, '/');              // Display '/' separator after the day
        LCD_ShowNum(1, 10, DS1302_Time[1], 2); // Show the month on the first row, starting at column 10
        LCD_ShowChar(1, 12, '/');             // Display '/' separator after the month
        LCD_ShowNum(1, 13, DS1302_Time[0], 2); // Show the year on the first row, starting at column 13

        /* Display Time on the LCD */
        LCD_ShowNum(2, 7, DS1302_Time[3], 2); // Show the hour on the second row, starting at column 7
        LCD_ShowChar(2, 9, ':');              // Display ':' separator after the hour
        LCD_ShowNum(2, 10, DS1302_Time[4], 2); // Show the minute on the second row, starting at column 10
        LCD_ShowChar(2, 12, ':');             // Display ':' separator after the minute
        LCD_ShowNum(2, 13, DS1302_Time[5], 2); // Show the second on the second row, starting at column 13
    }
}











// Uncommented code example for another way to display time and date

// #include <reg52.H>
// #include "Ds1302.h"
// #include "LCD1602.h"

// void main() {
//     LCD_Init();       // Initialize the LCD
//     DS1302_Init();    // Initialize the DS1302 RTC
//     LCD_ShowString(1, 1, "  /  /  ");  // Set initial date format on LCD
//     LCD_ShowString(2, 1, "  :  :  ");  // Set initial time format on LCD
//     DS1302_SetTime(); // Set the RTC time

//     while (1) {       // Infinite loop to continuously update the display
//         DS1302_ReadTime(); // Read current date and time from the RTC
//         LCD_ShowNum(1, 1, DS1302_Time[0], 2); // Show year
//         LCD_ShowNum(1, 4, DS1302_Time[1], 2); // Show month
//         LCD_ShowNum(1, 7, DS1302_Time[2], 2); // Show day
//         LCD_ShowNum(2, 1, DS1302_Time[3], 2); // Show hour
//         LCD_ShowNum(2, 4, DS1302_Time[4], 2); // Show minute
//         LCD_ShowNum(2, 7, DS1302_Time[5], 2); // Show second
//     }
// }
