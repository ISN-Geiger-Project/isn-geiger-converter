/*
_______________________________________________________________________________
   	
    Lycée St-Eloi

	Informatique et Sciences du Numérique

	Compteur Geiger Connecté

_______________________________________________________________________________

   Geiger.c : Programme interface Compteur / RS232
_______________________________________________________________________________


24/01/14	:	Rev 1.0

*/

#include "Geiger.h"


#pragma config OSC  = XT
#pragma config PWRT = ON
#pragma config WDT  = OFF
#pragma config BOR  = ON
#pragma config BORV = 42
#pragma config LVP  = OFF


//_____________________________________________________________________________
//
// Variables globales pour routine d'IT
//_____________________________________________________________________________
int Ctr; 
int FlPulse, FlTimer;


//_____________________________________________________________________________
// Configure l'USART
//
// 9600 bauds, 1 bit de stop, pas de parité.
//_____________________________________________________________________________
void Configure_USART(void)
{
  OpenUSART( 	USART_TX_INT_OFF 	& USART_RX_INT_OFF 	& USART_ASYNCH_MODE &
				USART_EIGHT_BIT 	& USART_SINGLE_RX  	& USART_BRGH_HIGH , 
  				25 );
}


//_____________________________________________________________________________
//
// Routine d'interruption 
//_____________________________________________________________________________
#pragma interrupt ISR_HIGH
void ISR_HIGH()
{
  if(INTCON3bits.INT1IF == 1)
    {
      INTCON3bits.INT1IF = 0;
      Ctr++;
      FlPulse=1;
    }

  if(INTCONbits.TMR0IF == 1)
    {
      WriteTimer0( TMR01S );
      INTCONbits.TMR0IF = 0;
      FlTimer=1;
    }
}

//_____________________________________________________________________________
//
// Initialisation des vecteurs d'IT
//_____________________________________________________________________________
#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void)
{
 _asm
 goto ISR_HIGH
 _endasm
}



//_____________________________________________________________________________
//
//               P R O G R A M M E        P R I N C I P A L 
//_____________________________________________________________________________
void main(void)
{
 
int Integration = 0;

 Ctr = 0;
 FlPulse = 0;
 FlTimer = 0;

 InitLCD();													// Initialise l'afficheur LCD
 Configure_USART();											// Configure l'USART

 OpenRB1INT( PORTB_CHANGE_INT_OFF |							// RB0 interromp
             FALLING_EDGE_INT     |
             PORTB_PULLUPS_OFF);

 OpenTimer0( 	TIMER_INT_ON &
				T0_16BIT &
				T0_SOURCE_INT &
				T0_PS_1_256 );

 WriteTimer0( TMR01S );

 stdout = _H_USER;

 RCONbits.IPEN	=1;											// Gestion des priorité d'IT

 INTCON3bits.INT1IP = 1;									// INT1 en priorité haute
 INTCON3bits.INT1IE = 1;									// Autorise INT1 à interrompre

 INTCONbits.TMR0IE = 1;										// Autorise TMRO à interrompre
 INTCON2bits.TMR0IP = 1; 									// en priorité haute

 INTCONbits.GIEH = 1;										// Autorise les it hautes
 INTCONbits.GIEL = 1;										// Autorises les it basses


 while(1)
  {
	if(FlPulse == 1)
     {
	   stdout = _H_USER;
       Gotoxy(0,0);					
       printf("N=%d  ",Ctr);
       FlPulse = 0;
     }

    if(FlTimer == 1)
     {
 		stdout = _H_USART;
		Integration = Ctr;
        Ctr = 0;
		printf("N = %d \r\n", Integration );

		stdout = _H_USER;
        Gotoxy(0,0);
        printf("T=0   ");
        Gotoxy(8,0);
	    printf("T=%d  ",Integration);

        FlTimer = 0;
	 }
  }

}
