#include <reg52.h>  // Include the header file for the 8051 microcontroller

// Define pins for the LCD control lines using sbit (bit-addressable) notation
sbit LCD_RS = P2^6;  // Register Select pin: High for data, Low for command
sbit LCD_RW = P2^5;  // Read/Write pin: High for read, Low for write
sbit LCD_EN = P2^7;  // Enable pin: Used to latch data into the LCD

// Define the data port for the LCD
#define LCD_DataPort P0  // LCD data lines are connected to Port 0 of the microcontroller

/**
 * @brief Delays the execution to allow the LCD to process commands or data
 * @param None
 * @retval None
 */
void LCD_Delay() {
  unsigned char i, j;  // Declare two loop counter variables

  i = 2;  // Initialize the outer loop counter
  j = 239;  // Initialize the inner loop counter

  do {  // Outer loop
    while (--j);  // Inner loop: Decrement j until it reaches 0
  } while (--i);  // Decrement i and repeat the outer loop until i reaches 0
}

/**
 * @brief Sends a command to the LCD
 * @param Command: The command byte to be sent to the LCD
 * @retval None
 */
void LCD_WriteCommand(unsigned char Command) {
  LCD_RS = 0;  // Set RS to 0 to indicate command mode
  LCD_RW = 0;  // Set RW to 0 to indicate write operation
  LCD_DataPort = Command;  // Place the command byte on the data port
  LCD_EN = 1;  // Enable the LCD to latch the command
  LCD_Delay();  // Wait for the LCD to process the command
  LCD_EN = 0;  // Disable the LCD
  LCD_Delay();  // Wait before sending the next command or data
}

/**
 * @brief Sends data to the LCD
 * @param Data: The data byte to be sent to the LCD
 * @retval None
 */
void LCD_WriteData(unsigned char Data) {
  LCD_RS = 1;  // Set RS to 1 to indicate data mode
  LCD_RW = 0;  // Set RW to 0 to indicate write operation
  LCD_DataPort = Data;  // Place the data byte on the data port
  LCD_EN = 1;  // Enable the LCD to latch the data
  LCD_Delay();  // Wait for the LCD to process the data
  LCD_EN = 0;  // Disable the LCD
  LCD_Delay();  // Wait before sending the next command or data
}

/**
 * @brief Sets the cursor position on the LCD
 * @param Line: The line number (1 or 2)
 * @param Column: The column number (1 to 16)
 * @retval None
 */
void LCD_SetCursor(unsigned char Line, unsigned char Column) {
  if (Line == 1) {  // If the first line is selected
    LCD_WriteCommand(0x80 | (Column - 1));  // Set cursor to the specified column on the first line
  } else if (Line == 2) {  // If the second line is selected
    LCD_WriteCommand(0x80 + (Column - 1 + 0x40));  // Set cursor to the specified column on the second line
  }
}

/**
 * @brief Initializes the LCD
 * @param None
 * @retval None
 */
void LCD_Init() {
  LCD_WriteCommand(0x38);  // Function set: 8-bit, 2 line, 5x7 dots
  LCD_WriteCommand(0x0c);  // Display ON, cursor OFF
  LCD_WriteCommand(0x06);  // Entry mode: Increment cursor, no shift
  LCD_WriteCommand(0x01);  // Clear display
}

/**
 * @brief Displays a single character at the specified position
 * @param Line: The line number (1 or 2)
 * @param Column: The column number (1 to 16)
 * @param Char: The character to display
 * @retval None
 */
void LCD_ShowChar(unsigned char Line, unsigned char Column, char Char) {
  LCD_SetCursor(Line, Column);  // Set the cursor to the specified position
  LCD_WriteData(Char);  // Write the character to the LCD
}

/**
 * @brief Displays a string starting from the specified position
 * @param Line: The line number (1 or 2)
 * @param Column: The column number (1 to 16)
 * @param String: Pointer to the string to be displayed
 * @retval None
 */
void LCD_ShowString(unsigned char Line, unsigned char Column, char *String) {
  unsigned char i;  // Loop counter
  LCD_SetCursor(Line, Column);  // Set the cursor to the specified position
  for (i = 0; String[i] != '\0'; i++) {  // Loop through each character in the string
    LCD_WriteData(String[i]);  // Write each character to the LCD
  }
}

/**
 * @brief Calculates the power of a number
 * @param X: The base number
 * @param Y: The exponent
 * @retval The result of X raised to the power Y
 */
int LCD_Pow(int X, int Y) {
  unsigned char i;  // Loop counter
  int Result = 1;  // Initialize result as 1
  for (i = 0; i < Y; i++) {  // Loop Y times
    Result *= X;  // Multiply Result by X each time
  }
  return Result;  // Return the final result
}

/**
 * @brief Displays a number at the specified position
 * @param Line: The line number (1 or 2)
 * @param Column: The column number (1 to 16)
 * @param Number: The number to display
 * @param Length: The number of digits to display
 * @retval None
 */
void LCD_ShowNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length) {
  unsigned char i;  // Loop counter
  LCD_SetCursor(Line, Column);  // Set the cursor to the specified position
  for (i = Length; i > 0; i--) {  // Loop for the specified length
    LCD_WriteData(Number / LCD_Pow(10, i - 1) % 10 + '0');  // Extract and display each digit
  }
}

/**
 * @brief Displays a signed number at the specified position
 * @param Line: The line number (1 or 2)
 * @param Column: The column number (1 to 16)
 * @param Number: The signed number to display
 * @param Length: The number of digits to display
 * @retval None
 */
//		void LCD_ShowSignedNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length) {
//			unsigned char i;  // Loop counter
//			unsigned int Number1;  // Variable to store the absolute value of Number
//			LCD_SetCursor(Line, Column);  // Set the cursor to the specified position
//			if (Number >= 0) {  // If the number is positive
//				LCD_WriteData('+');  // Display a plus sign
//				Number1 = Number;  // Use the original number
//			} else {  // If the number is negative
//				LCD_WriteData('-');  // Display a minus sign
//				Number1 = -Number;  // Use the absolute value of the number
//			}
//			for (i = Length; i > 0; i--) {  // Loop for the specified length
//				LCD_WriteData(Number1 / LCD_Pow(10, i - 1) % 10 + '0');  // Extract and display each digit
//			}
//		}



/**
 * @brief Displays a hexadecimal number at the specified position
 * @param Line: The line number (1 or 2)
 * @param Column: The column number (1 to 16)
 * @param Number: The hexadecimal number to display
 * @param Length: The number of digits to display
 * @retval None
 */
////		void LCD_ShowHexNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length) {
////			unsigned char i, SingleNumber;  // Loop counter and variable to store each digit
////			LCD_SetCursor(Line, Column);  // Set the cursor to the specified position
////			for (i = Length; i > 0; i--) {  // Loop for the specified length
////				SingleNumber = Number / LCD_Pow(16, i - 1) % 16;  // Extract each digit
////				if (SingleNumber < 10) {  // If the digit is less than 10
////					LCD_WriteData(SingleNumber + '0');  // Display it as a numeric character
////				} else {  // If the digit is 10 or greater
////					LCD_WriteData(SingleNumber - 10 + 'A');  // Display it as a letter (A-F)
////				}
////			}
////		}



/**
 * @brief Displays a binary number at the specified position
 * @param Line: The line number (1 or 2)
 * @param Column: The column number (1 to 16)
 * @param Number: The binary number to display
 * @param Length: The number of digits to display
 * @retval None
 */
////		void ShowBinNum(unsigned char Line, unsigned char Column, unsigned int Number, unsigned char Length) {
////			unsigned char i;  // Loop counter
////			LCD_SetCursor(Line, Column);  // Set the cursor to the specified position
////			for (i = Length; i > 0; i--) {  // Loop for the specified length
////				LCD_WriteData((Number / LCD_Pow(2, i - 1) % 2) + '0');  // Extract and display each binary digit
////			}
////		}










/*
 __Summary:__
LCD_Delay(): Introduces a small delay for timing.
LCD_WriteCommand(): Sends commands to the LCD.
LCD_WriteData(): Sends data to be displayed on the LCD.
LCD_SetCursor(): Positions the cursor on the LCD screen.
LCD_Init(): Initializes the LCD with basic settings.
LCD_ShowChar(): Displays a single character on the LCD.
LCD_ShowString(): Displays a string on the LCD.
LCD_Pow(): Calculates the power of a number.
LCD_ShowNum(): Displays a number on the LCD.
LCD_ShowSignedNum(): Displays a signed number (positive or negative).
LCD_ShowHexNum(): Displays a hexadecimal number.
ShowBinNum(): Displays a binary number.
These functions allow you to control the LCD to display various forms of data, from single characters to strings, and from numbers in decimal, hexadecimal, or binary formats.
*/