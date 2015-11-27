//Defines
#ifndef BAUD
#define BAUD 9600
#endif
//Includes
#include <avr/io.h>
#include <stdio.h>
#include <util/setbaud.h>

//Prototypes
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

//Globals
FILE uart_tx = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_rx = FDEV_SETUP_STREAM(NULL,uart_getchar, _FDEV_SETUP_READ);

/*
 * Initializes the serial port at the baud rate indicated by the 
 * BAUD definition above. The serial port parameters are as follows:
 * - 8 bit data
 * - Transmitter Enabled
 * - Receiver Enabled
 *
 * @param (void)
 * @return (void)
 */
void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
}
/*
 * Sends a character through the transmitter interface.
 *
 * @param (char) c, (FILE *) stream
 * @return (int) 1
 */
int uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

int uart_getchar(FILE *stream) {
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}
