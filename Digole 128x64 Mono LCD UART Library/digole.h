//
//  digole.h
//  
//
//  Created by Aldanis Vigo on 11/26/15.
//
//

#ifndef digole_h
#define digole_h

#include <stdio.h>
#include "uart.h"
#include <avr/io.h>
#include "digole.h"
//
//  digole.c
//
//
//  Created by Aldanis Vigo on 11/26/15.
//
//
void lcdInit();
void lcdWrite(unsigned int data);
void gotoPos(uint8_t X, uint8_t Y);
void setPrintPosition(unsigned int x, unsigned int y, uint8_t graphicsmode);
void writeStrAtPos(unsigned int x, unsigned int y, const char* str);
void writeStr(char* str);
void toggleCursor(uint8_t val);
void clearScreen(void);
void lcdBlink(uint8_t reps);
void drawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
void drawBitmap(short int x,short int y,short int width,short int height,const uint8_t * bitmap);
#endif /* digole_h */
