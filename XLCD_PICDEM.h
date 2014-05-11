#ifndef __XLCD_PICDEM_H
#define __XLCD_PICDEM_H

#include "xlcd.h"
#include <stdio.h>
#include <delays.h>

void DelayFor18TCY(void);

void DelayPORXLCD(void);

void DelayXLCD(void);

void Gotoxy(unsigned char x, unsigned char y);

int _user_putc( char c);

void InitLCD(void);


#endif