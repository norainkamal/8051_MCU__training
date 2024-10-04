#ifndef __DS1302_H__  // Check if __DS1302_H__ is not defined
#define __DS1302_H__  // Define __DS1302_H__ to prevent multiple inclusions of this header file

// Declare an external array to store time and date values
extern unsigned int DS1302_Time[];

// Function prototypes for DS1302 operations
// Function to initialize the DS1302 RTC
void DS1302_Init();

// Function to write a byte of data to the DS1302
// Parameters:
//   Command: The command byte to be sent
//   Data: The data byte to be sent
void DS1302_WriteByte(unsigned char Command, unsigned char Data);

// Function to read a byte of data from the DS1302
// Parameters:
//   Command: The command byte specifying which data to read
// Returns: The data byte read from DS1302
unsigned char DS1302_ReadByte(unsigned char Command);

// Function to set the current time and date on the DS1302
void DS1302_SetTime();

// Function to read the current time and date from the DS1302
void DS1302_ReadTime();

#endif  // End of the header guard __DS1302_H__




//#ifndef __LCD1602_H__
//#define __LCD1602_H__
//extern unsigned int DS1302_Time[];

//void LCD_Init();
//void LCD_ShowChar(unsigned char row, unsigned char col, char ch);
//void LCD_ShowString(unsigned char row, unsigned char col, char *str);
//void LCD_ShowNum(unsigned char row, unsigned char col, unsigned int num, unsigned char len);
//void LCD_ShowSignedNum(unsigned char row, unsigned char col, int num, unsigned char len);
//void LCD_ShowHexNum(unsigned char row, unsigned char col, unsigned int num, unsigned char len);
//void LCD_ShowBinNum(unsigned char row, unsigned char col, unsigned int num, unsigned char len);

//#endif











/*  

//  __Explanation of Header File:__ 
#ifndef __DS1302_H__: Checks if the macro __DS1302_H__ is not defined. This is used to prevent multiple inclusions of the header file in a single compilation, which could cause redefinition errors.

#define __DS1302_H__: Defines the macro __DS1302_H__. If the header file is included again, this macro will already be defined, preventing the content of the file from being processed again.

extern unsigned int DS1302_Time[];: Declares an external array DS1302_Time that will be defined in another source file. This array is used to store the time and date values.

void DS1302_Init();: Declares the DS1302_Init function, which initializes the DS1302 RTC. This function will be defined elsewhere and does not return any value.

void DS1302_WriteByte(unsigned char Command, unsigned char Data);: Declares the DS1302_WriteByte function, which writes a byte of data to the DS1302. It takes two parameters: Command (the command byte) and Data (the data byte).

unsigned char DS1302_ReadByte(unsigned char Command);: Declares the DS1302_ReadByte function, which reads a byte of data from the DS1302. It takes one parameter: Command (the command byte), and returns the data byte read from DS1302.

void DS1302_SetTime();: Declares the DS1302_SetTime function, which sets the current time and date on the DS1302. This function does not return any value.

void DS1302_ReadTime();: Declares the DS1302_ReadTime function, which reads the current time and date from the DS1302. This function does not return any value.

#endif: Ends the conditional block started by #ifndef __DS1302_H__. This ensures that the content between #ifndef and #endif is only included once.

*/