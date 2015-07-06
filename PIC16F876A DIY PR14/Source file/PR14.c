//==========================================================================
//	Author				: Cytron Technologies		
//	Project				: Alarm System: Motion Detector
//	Project description	: This project will use PIC16F876A (microcontroller)
//                                to control the alarm system (buzzer & LED) with
//                                a PIR Sensor (Motion Detector).Compatible with
//                                -MPLAB IDE with HITECH C compiler
//                                -MPLABX with HITECH C compiler v9.83/v9.82/v9.8 and XC8 compiler.

//==========================================================================
//	include
//==========================================================================

#if defined(__XC8)
  #include <xc.h>
 #pragma config CONFIG = 0x3F32
//FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
//WDTE = ON        // Watchdog Timer Enable bit (WDT enabled)
//PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
//BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
//LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
//CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
//WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
//CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#else
//#include <htc.h>                  //include the PIC microchip header file
#include <pic.h>

//	configuration
//==========================================================================
__CONFIG (0x3F32);

#endif
//	define
//==========================================================================
#define	ON_ALARM        RA0  //SW1
#define	SENSOR		RB2
#define	LED1		RC0
#define	LED2		RC1
#define	LED3		RC2
#define BUZZER 		RC4




//	main function
//==========================================================================
void main(void)
{
	unsigned char status;
	unsigned long temp1=0;          //Acts as delay
        unsigned long temp2=0;          //Acts as delay

	ADCON1 = 0x06;			//Congifure Port A as digital I/O

	TRISA = 0b11111111;		//Configure Port A as Input
	TRISB = 0b11111111;		//Configure Port B as Input
	TRISC = 0b00000000;		//Configure Port C as Output
	
	status=0;
	LED1=0;
        LED2=0;
        LED3=0;
	BUZZER=0;

        
	while(1)								//Infinity Loop
	{	
		//scan input
		if((ON_ALARM==0)&&(status!=4))		//Turn on the alarm
		{
			while(ON_ALARM==0) continue;
			status=1;
			temp1=0;
		}
                if((SENSOR==0)&&(status==4))		//Turn on the alarm
		{
			while(ON_ALARM==0) continue;
			status=2;
			LED1=0;
                        BUZZER=0;
		}
		
		if((SENSOR==1)&&(status==3))		//Motion detected
			status=4;
	
		//processing output
		switch(status)
		{
			//Permitted period mode (15 seconds)
			case 1:							
				LED1=1;
				temp1++;
				if(temp1<1000)
                                    BUZZER=1;	//Sound 'Beep'
                                else if(temp1>1040000)
				{
					temp1=0;		
					status=2; 
				}
				else BUZZER=0;
				break;

			//Scanning mode
			case 2:							
				temp2++;
				if(temp2<2500)
                                {
                                    LED2=1;
                                    LED3=1;                //LED blink
                                }
				else if(temp2>2500 && temp2<3500 )//LED blink
                                {
                       
                                    LED2=0;
                                    LED3=0;
                                }
                                else if(temp2>3500 && temp2<4500 )//LED blink
                                {
                       
                                    LED2=1;
                                    LED3=1; 
                                }
                                else if(temp2>6500)//LED blink
                                {

                                    temp2=0;
                                    status=3;
                                }
				
                                    
				break;

                        case 3:
                            LED1=0;
                            LED2=0;
                            LED3=0;
                            break;
			//Detected mode
			case 4:
				temp2++;
				if(temp2<2500)
                                {
                                    LED1=1;
                                    BUZZER=1;                //LED blink
                                }
				else if(temp2>2500 && temp2<3500 )//LED blink
                                {

                                    LED1=0;
                                    //BUZZER=1;
                                }
                                else if(temp2>3500)//LED blink
                                {
                                    LED1=1;
                                }
				
				break;
		}
	}
}
