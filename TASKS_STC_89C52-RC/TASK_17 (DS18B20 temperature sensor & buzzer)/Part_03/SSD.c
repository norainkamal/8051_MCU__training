#include<reg51.h>
#include  "main.h"


unsigned char index=0;
unsigned int select[]  =  {0x0C,0x08,0x04,0x00};
unsigned int message[] = {0x3f,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};
unsigned char values[4];

void display_num_7seg(unsigned char num)
{
 int i=3;
 while(num!=0)
 {
  values[i]=num%10;
	 num=num/10;
	 i--;
 }
 
 }
void values_on_ssd(void)
 {
  switch(values[index])
	{
		case 0:
		   {
			 P2=select[index];
			 P0=message[0];		 
			 }
	     break;
		case 1:
		   {
			 P2=select[index];
			 P0=message[1];		 
			 }
	     break;
		case 2:
		   {
			 P2=select[index];
			 P0=message[2];		 
			 }
	     break;
		case 3:
		   {
			  P2=select[index];
			 P0=message[3];		 
			 }
	     break;
		case 4:
		   {
			 P2=select[index];
			 P0=message[4];		 
			 }
	     break;
		case 5:
		   {
			  P2=select[index];
			 P0=message[5];		 
			 }
	     break;
		case 6:
		   {
			  P2=select[index];
			 P0=message[6];		 
			 }
	     break;
		case 7:
		   {
			 P2=select[index];
			 P0=message[7];		 
			 }
	     break;
		case 8:
		   {
			 P2=select[index];
			 P0=message[8];		 
			 }
	     break;
		case 9:
		   {
			 P2=select[index];
			 P0=message[9];		 
			 }
	     break;
	default:
		break;
	}
 index+=1;
	if(index > 3)
					{
						index=0;
					}
  
}