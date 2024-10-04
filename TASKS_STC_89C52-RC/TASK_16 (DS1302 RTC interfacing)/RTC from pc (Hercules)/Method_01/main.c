//#include <reg52.h>
//#include <intrins.h>
//#include <stdio.h>

//sbit SCLK = P3^6;  // Serial Clock
//sbit IO = P3^4;    // Data I/O
//sbit CE = P3^5;    // Chip Enable

//sbit RS = P2^6;
//sbit RW = P2^5;
//sbit EN = P2^7;

//unsigned char sec, min, hour, day, month, year;

//// Delay Function
//void delay(unsigned int ms) {
//    while(ms--) _nop_();
//}

//// LCD Functions
//void LCD_Command(unsigned char command) {
//    RS = 0; 
//    RW = 0; 
//    P0 = command;
//    EN = 1;
//    delay(5);
//    EN = 0;
//}

//void LCD_Init() {
//    LCD_Command(0x38);  // 8-bit mode, 2 lines
//    LCD_Command(0x0C);  // Display on, cursor off
//    LCD_Command(0x06);  // Increment cursor
//    LCD_Command(0x01);  // Clear display
//}

//void LCD_WriteChar(char c) {
//    RS = 1;
//    RW = 0;
//    P0 = c;
//    EN = 1;
//    delay(5);
//    EN = 0;
//}

//void LCD_WriteString(char *str) {
//    while(*str) {
//        LCD_WriteChar(*str++);
//    }
//}

//// DS1302 Functions
//void DS1302_Init() {
//    CE = 0;  // Disable DS1302
//    SCLK = 0;  // Initialize clock line
//}

//void DS1302_WriteByte(unsigned char dat) {
//    unsigned char i;
//    for (i = 0; i < 8; i++) {
//        IO = dat & 0x01;
//        SCLK = 1;
//        dat >>= 1;
//        SCLK = 0;
//    }
//}

//unsigned char DS1302_ReadByte() {
//    unsigned char dat = 0;
//    unsigned char i;
//    for (i = 0; i < 8; i++) {
//        dat >>= 1;
//        if (IO) dat |= 0x80;
//        SCLK = 1;
//        SCLK = 0;
//    }
//    return dat;
//}

//void DS1302_WriteTime(unsigned char address, unsigned char dataa) {
//    CE = 1;  // Enable DS1302
//    DS1302_WriteByte(address);  // Send address
//    DS1302_WriteByte(dataa);  // Send data
//    CE = 0;  // Disable DS1302
//}

//unsigned char DS1302_ReadTime(unsigned char address) {
//    unsigned char dataa;
//    CE = 1;  // Enable DS1302
//    DS1302_WriteByte(address | 0x01);  // Send address with read command
//    dataa = DS1302_ReadByte();  // Read data
//    CE = 0;  // Disable DS1302
//    return dataa;
//}

//void Display_Date_Time() {
//    char buffer[17];
//    sprintf(buffer, "Time: %02x:%02x:%02x", hour, min, sec);
//    LCD_Command(0x80);  // Move to first line
//    LCD_WriteString(buffer);

//    sprintf(buffer, "Date: %02x/%02x/20%02x", day, month, year);
//    LCD_Command(0xC0);  // Move to second line
//    LCD_WriteString(buffer);
//}

//void Set_DS1302_Time(unsigned char h, unsigned char m, unsigned char s, unsigned char d, unsigned char mo, unsigned char y) {
//    DS1302_WriteTime(0x80, s);
//    DS1302_WriteTime(0x82, m);
//    DS1302_WriteTime(0x84, h);
//    DS1302_WriteTime(0x86, d);
//    DS1302_WriteTime(0x88, mo);
//    DS1302_WriteTime(0x8C, y);
//}

//void UART_Init() {
//    TMOD = 0x20;  // Timer 1, mode 2
//    TH1 = 0xFD;   // 9600 baud rate
//    SCON = 0x50;  // 8-bit, 1 stop bit, REN enabled
//    TR1 = 1;      // Start Timer 1
//}

//void UART_SendChar(char c) {
//    SBUF = c;
//    while(TI == 0);
//    TI = 0;
//}

//char UART_ReceiveChar() {
//    while(RI == 0);
//    RI = 0;
//    return SBUF;
//}

//void UART_SendString(char *str) {
//    while(*str) {
//        UART_SendChar(*str++);
//    }
//}

//void Set_Time_From_PC() {
//    char time_data[7];
//    unsigned char i;

//    UART_SendString("Enter time in HHMMSSDDMMYY format: ");
//    for(i = 0; i < 12; i++) {
//        time_data[i] = UART_ReceiveChar();
//        UART_SendChar(time_data[i]);  // Echo back the entered character
//    }

//    // Convert and set time
//    Set_DS1302_Time((time_data[0]-'0')*10 + (time_data[1]-'0'), 
//                    (time_data[2]-'0')*10 + (time_data[3]-'0'), 
//                    (time_data[4]-'0')*10 + (time_data[5]-'0'), 
//                    (time_data[6]-'0')*10 + (time_data[7]-'0'), 
//                    (time_data[8]-'0')*10 + (time_data[9]-'0'), 
//                    (time_data[10]-'0')*10 + (time_data[11]-'0'));
//}

//void main() {
//    DS1302_Init();
//    LCD_Init();
//    UART_Init();

//    Set_Time_From_PC();  // Set time from PC via serial port

//    while(1) {
//        sec = DS1302_ReadTime(0x81);  // Read seconds
//        min = DS1302_ReadTime(0x83);  // Read minutes
//        hour = DS1302_ReadTime(0x85);  // Read hours
//        day = DS1302_ReadTime(0x87);  // Read day
//        month = DS1302_ReadTime(0x89);  // Read month
//        year = DS1302_ReadTime(0x8D);  // Read year

//        Display_Date_Time();  // Display date and time on LCD

//        delay(1000);  // Delay for 1 second
//    }
//}






#include <reg52.h>
#include <intrins.h>
#include <stdio.h>

sbit SCLK = P3^6;  // Serial Clock
sbit IO = P3^4;    // Data I/O
sbit CE = P3^5;    // Chip Enable

sbit RS = P2^6;
sbit RW = P2^5;
sbit EN = P2^7;

unsigned char sec, min, hour, day, month, year;

// Delay Function
void delay(unsigned int ms) {
    while(ms--) _nop_();
}

// LCD Functions
void LCD_Command(unsigned char command) {
    RS = 0; 
    RW = 0; 
    P0 = command;
    EN = 1;
    delay(10);  // Increased delay
    EN = 0;
}

void LCD_Init() {
    LCD_Command(0x38);  // 8-bit mode, 2 lines
    LCD_Command(0x0C);  // Display on, cursor off
    LCD_Command(0x06);  // Increment cursor
    LCD_Command(0x01);  // Clear display
    delay(20);  // Wait for LCD to clear
}

void LCD_WriteChar(char c) {
    RS = 1;
    RW = 0;
    P0 = c;
    EN = 1;
    delay(10);  // Increased delay
    EN = 0;
}

void LCD_WriteString(char *str) {
    while(*str) {
        LCD_WriteChar(*str++);
    }
}

// DS1302 Functions
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

void Display_Date_Time() {
    char buffer[17];
    sprintf(buffer, "Time: %02d:%02d:%02d", hour, min, sec);  // Changed %02x to %02d
    LCD_Command(0x80);  // Move to first line
    LCD_WriteString(buffer);

    sprintf(buffer, "Date: %02d/%02d/20%02d", day, month, year);  // Changed %02x to %02d
    LCD_Command(0xC0);  // Move to second line
    LCD_WriteString(buffer);
}

void Set_DS1302_Time(unsigned char h, unsigned char m, unsigned char s, unsigned char d, unsigned char mo, unsigned char y) {
    DS1302_WriteTime(0x80, s);
    DS1302_WriteTime(0x82, m);
    DS1302_WriteTime(0x84, h);
    DS1302_WriteTime(0x86, d);
    DS1302_WriteTime(0x88, mo);
    DS1302_WriteTime(0x8C, y);
}

void UART_Init() {
    TMOD = 0x20;  // Timer 1, mode 2
    TH1 = 0xFD;   // 9600 baud rate
    SCON = 0x50;  // 8-bit, 1 stop bit, REN enabled
    TR1 = 1;      // Start Timer 1
}

void UART_SendChar(char c) {
    SBUF = c;
    while(TI == 0);
    TI = 0;
}

char UART_ReceiveChar() {
    while(RI == 0);
    RI = 0;
    return SBUF;
}

void UART_SendString(char *str) {
    while(*str) {
        UART_SendChar(*str++);
    }
}

void Set_Time_From_PC() {
    char time_data[12];
    unsigned char i;

    UART_SendString("Enter time in HHMMSSDDMMYY format: ");
    for(i = 0; i < 12; i++) {
        time_data[i] = UART_ReceiveChar();
        UART_SendChar(time_data[i]);  // Echo back the entered character
    }

    // Convert and set time
    Set_DS1302_Time((time_data[0]-'0')*10 + (time_data[1]-'0'), 
                    (time_data[2]-'0')*10 + (time_data[3]-'0'), 
                    (time_data[4]-'0')*10 + (time_data[5]-'0'), 
                    (time_data[6]-'0')*10 + (time_data[7]-'0'), 
                    (time_data[8]-'0')*10 + (time_data[9]-'0'), 
                    (time_data[10]-'0')*10 + (time_data[11]-'0'));
}

void main() {
    DS1302_Init();
    LCD_Init();
    UART_Init();

    Set_Time_From_PC();  // Set time from PC via serial port

    while(1) {
        sec = DS1302_ReadTime(0x81);  // Read seconds
        min = DS1302_ReadTime(0x83);  // Read minutes
        hour = DS1302_ReadTime(0x85);  // Read hours
        day = DS1302_ReadTime(0x87);  // Read day
        month = DS1302_ReadTime(0x89);  // Read month
        year = DS1302_ReadTime(0x8D);  // Read year

        Display_Date_Time();  // Display date and time on LCD

        delay(1000);  // Delay for 1 second
    }
}
