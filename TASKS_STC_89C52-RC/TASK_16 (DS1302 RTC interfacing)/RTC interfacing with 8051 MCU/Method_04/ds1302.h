// the #ifndef prevents the file from being included more than once
#ifndef __DS1307_H__   
#define __DS1307_H__
 
void write_ds1302_byte(char);

void write_ds1302(char, char);

char read_ds1302(char);
void rtc_init();
void rtc_set_datetime(char , char , char , char, char );
void rtc_get_date(char* , char* , char* );
void rtc_get_time(char* , char* , char* );
void rtc_write_nvr(char , char );
char rtc_read_nvr(char );


#endif