#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>
#include "main.h"
#include "uart.h"

unsigned char receiveWord(const uint16_t length,unsigned char * response){
    uint16_t index = 0;
    if(length == 0) return 0;
    
    for(index = 0;index < length;index++){
        response[index] = getchar();
        switch (response[index]){
            case '\r':
            case '\n':
                response[index] = '\0';
                return index + 1;
            default:
                putchar(response[index]);
                break;
        }
    }
    response[index] = '\0';
    return index + 1;
}

#define RESPONSEBUFFER 50
int main(void) {
    uart_init();
    stdout = &uart_tx;
    stdin  = &uart_rx;
    DDRB = 0; //Make output pin
    char uart_response[RESPONSEBUFFER];
    while (1) {
        //To write to UART use printf
	uart_putchar('c',&uart_tx);
        printf("Please enter your last name:");
        receiveWord(RESPONSEBUFFER,&uart_response);
        printf("\n\n\nYou responded:\n %s\n\n\n",uart_response);
    }
    return 0;
}
