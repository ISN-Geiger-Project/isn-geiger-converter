/*

Routines pour utuliser la bibliothèque XLCD
sur la carte PICDEM2+

*/

#include "XLCD_PICDEM.h"



void DelayFor18TCY(void)
{
 Delay10TCYx(10);					// Augmenté à 100TCy en raison de la rapidité de l'horloge (32MHz)
}


void DelayPORXLCD(void)
{
  // Temporisation de 15 ms.
  Delay1KTCYx(120);
}

void DelayXLCD(void)
{
 // Temporisation 5 ms.
 Delay1KTCYx(40);
}


void Gotoxy(unsigned char x, unsigned char y) 
{
    if (y<2 && x<16)
    {
      if (y) SetDDRamAddr(0x40+x);
      else SetDDRamAddr(x);
    }
}


int _user_putc( char c)
{
 while(BusyXLCD());
 WriteDataXLCD(c);
}


void InitLCD(void)
{
  OpenXLCD(FOUR_BIT & LINES_5X7);
  while(BusyXLCD());
  WriteCmdXLCD( SHIFT_DISP_LEFT );        
  while(BusyXLCD());
  WriteCmdXLCD( BLINK_OFF & CURSOR_OFF ); 
  while(BusyXLCD());
 
  stdout = _H_USER;
}
