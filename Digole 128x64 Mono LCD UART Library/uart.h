#ifndef _UART_H
#define _UART_H

int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);
unsigned char receiveWord(const uint16_t length,unsigned char * response);
void uart_init(void);

/* http://www.ermicro.com/blog/?p=325 */

extern FILE uart_tx;
extern FILE uart_rx;

#endif /* _UART_H_ */
