#include<reg51.h>
#include "delay.h"
#ifndef RTC_SCLK

#define RTC_SCLK = P3^6
#define RTC_IO   = P3^4
#define RTC_RST  = P3^5

#endif


void write_ds1302_byte(char cmd) {
   char i;

   for(i=0;i<=7;++i) {
      output_bit(RTC_IO, shift_right(&cmd,1,0) );
      output_high(RTC_SCLK);
      output_low(RTC_SCLK);
   }
}

void write_ds1302(char cmd, char data) {

   output_high(RTC_RST);
   write_ds1302_byte(cmd);
   write_ds1302_byte(data);
   output_low(RTC_RST);
}

char read_ds1302(char cmd) {
   char i,data;

   output_high(RTC_RST);
   write_ds1302_byte(cmd);

   for(i=0;i<=7;++i) {
      shift_right(&data,1,input(RTC_IO));
      output_high(RTC_SCLK);
      delay_us(2);
      output_low(RTC_SCLK);
      delay_us(2);
   }
   output_low(RTC_RST);
   return(data);
}

void rtc_init() {
   char x;
   output_low(RTC_RST);
   delay_us(2);
   output_low(RTC_SCLK);
   write_ds1302(0x8e,0);
   write_ds1302(0x90,0xa6);
   x=read_ds1302(0x81);
   if((x & 0x80)!=0)
     write_ds1302(0x80,0);
}

void rtc_set_datetime(char day, char mth, char year, char hr, char min) {
   write_ds1302(0x86,day);
   write_ds1302(0x88,mth);
   write_ds1302(0x8c,year);
   write_ds1302(0x8a,dow);
   write_ds1302(0x84,hr);
   write_ds1302(0x82,min);
   write_ds1302(0x80,0);
}

void rtc_get_date(char* day, char* mth, char* year) {
   day = read_ds1302(0x87);
   mth = read_ds1302(0x89);
   year = read_ds1302(0x8d);
}


void rtc_get_time(char* hr, char* min, char* sec) {
   hr = read_ds1302(0x85);
   min = read_ds1302(0x83);
   sec = read_ds1302(0x81);
}

void rtc_write_nvr(char address, char data) {
   write_ds1302(address|0xc0,data);
}

char rtc_read_nvr(char address) {
    return(read_ds1302(address|0xc1));
}