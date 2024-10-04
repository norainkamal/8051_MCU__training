#include<reg51.h>
#include<main.h>
unsigned char Unite1,Ten1,Hunderd1;
unsigned char j=0;
unsigned char z;
unsigned char serial_values[5]={0};
	
void display_num_serial(unsigned int z)
{
	Hunderd1=(z/100) + 0x30; //hundred
	Ten1=(z%100/10) + 0x30; //ten
	Unite1=(z%10) + 0x30; //unit
	serial_values[0]= Hunderd1;
	serial_values[1]= Ten1;
	serial_values[2]= Unite1;
	serial_values[3]='\n';
 }
void serial_data(void)
	{
	 for(z=0;z<6;z++)
	 {
	 SBUF=serial_values[z];
		 while(TI==0);
		  TI=0;
	    delay1(100);
	 }


//	while(arr[i]!='\0')
//{
//  SBUF=arr[i];
//	while (TI==0);
//	TI=0;	
//	i++;
//}
//delay1(100);
//i=0;
 }
