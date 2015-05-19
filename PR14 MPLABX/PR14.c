//==========================================================================
//	Author				: Cytron Technologies		
//	Project				: Alarm System: Motion Detector
//	Project description	: This project will use PIC16F876A (microcontroller)
//				 to control the alarm system (buzzer & LED) with 
//				 a PIR Sensor (Motion Detector). 
//==========================================================================
//	include
//==========================================================================
#include <pic.h> 

//	configuration
//==========================================================================
__CONFIG (0x3F32);

//	define
//==========================================================================
#define	alarm_set	RA0
#define	sensor		RB2
#define	led			RC0
#define buzzer 		RC4


//	main function
//==========================================================================
void main(void)
{
	unsigned char status;
	unsigned long temp1,temp2;

	ADCON1 = 0x06;							//Congifure Port A as digital I/O

	TRISA = 0b11111111;						//Configure Port A as Input
	TRISB = 0b11111111;						//Configure Port B as Input
	TRISC = 0b00000000;						//Configure Port C as Output
	
	status=0;
	led=0;
	buzzer=0;

	while(1)								//Infinity Loop
	{	
		//scan input
		if((alarm_set==0)&&(status!=3))		//Alarm set
		{
			while(alarm_set==0) continue;
			status=1;
			temp1=0;
		}
		
		if((sensor==1)&&(status==2))		//Motion detected
			status=3;
	
		//processing output
		switch(status)
		{
			//Permitted period mode (15 seconds)
			case 1:							
				led=1;
				temp1+=1;
				if(temp1<10000) buzzer=1;	//Sound 'Beep'
				else if(temp1>1000000)
				{
					temp1=0;		
					status=2; 
				}
				else buzzer=0;
				break;

			//Scanning mode
			case 2:							
				temp2+=1;
				if(temp2<2500) led=1;		//LED blink
				else if(temp2<200000) led=0;
				else temp2=0;
				break;

			//Detected mode
			case 3:							
				temp2+=1;
				if(temp2<40000) 			//LED blink & buzzer activated
				{
					led=1;
					buzzer=1;
				}
				else if(temp2<60000) 
				{
					led=0;
					buzzer=0;
				}
				else temp2=0;
				break;
		}
	}
}
