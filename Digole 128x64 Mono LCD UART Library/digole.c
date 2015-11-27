//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//										//
//  digole.c									//
//  										//
//  AVR UART Library for Digole LCD Screens made using Atmge168-20PU		//
//										//
//  |----------------------|     |--------------------|				//
//  |     ATMEGA168        |	 |     DIGOLE LCD     |				//
//  |			   |	 |		      |				//
//  |			   |	 |		      |				//
//  |                  TX--|-----|--DATA	      |				//
//  |                 VCC--|-----|--VCC		      |				//
//  |		      GND--|-----|--GND  	      |				//
//  |			   |     |		      |				//
//  |			   |	 |		      |				//
//  |----------------------|	 |--------------------|				//	
//										//
//  Created by Aldanis Vigo on 11/26/15.					//
//										//	
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <avr/io.h>
#ifndef _AVR_/_DELAY_H
#include <avr/delay.h>
#endif
#include <avr/pgmspace.h>
#include "digole.h"
#include "uart.h"

void lcdInit(){
	stdin = &uart_rx;
   	stdout = &uart_tx;
}
void lcdWrite(unsigned int data){
    	if(data < 255){
        	printf("%c",data);
    	}
    	else{
        	printf("%c",(unsigned int)255);
        	printf("%c",(unsigned int)(255 - data));
    	}
}
void gotoPos(uint8_t X, uint8_t Y){
    	printf("STCR");
    	lcdWrite((unsigned int)X);
    	lcdWrite((unsigned int)Y);
    	printf("\x80\xC0\x94\xD4");
}
void setPrintPosition(unsigned int x, unsigned int y, uint8_t graphicsmode){
    	if(graphicsmode == 0){
    		printf("TP");
    	    	lcdWrite(x);
   	    	lcdWrite(y);
   	}
   	else{
   	     	printf("GP");
   	    	lcdWrite(x);
   	     	lcdWrite(y);
   	}
}
void writeStrAtPos(unsigned int x, unsigned int y, const char* str){
	setPrintPosition(x,y,0);
    	printf("TT");
    	puts(str);
    	printf("%c",(uint8_t)0);
}
void writeStr(char* str){
    	printf("TT");
    	puts(str);
}
void toggleCursor(uint8_t val){
    	printf("CS");
    	lcdWrite(val);
}
void lcdBlink(uint8_t reps){
    	for(int rep = 0;rep < reps;rep++){
    		printf("BL0");
    	    	_delay_ms(500);
    	    	printf("BL1");
    	    	_delay_ms(500);
    	}
}
void clearScreen(void){
    	printf("CL");
}
void drawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2){
    	printf("LN");
    	lcdWrite(x1);
    	lcdWrite(y1);
    	lcdWrite(x2);
    	lcdWrite(y2);
}
void drawBitmap(short x,short y,short width,short height,const uint8_t *bitmap){
	uint8_t i = 0;
        if ((width & 7) != 0)
        	i = 1;
    
        printf("DIM");
        lcdWrite(x); //x;
        lcdWrite(y);
        lcdWrite(width);
        lcdWrite(height);
        for (int j = 0; j < height * ((width >> 3) + i); j++){
        	printf("%c",pgm_read_byte_near(bitmap + j));
        }
}
