#ifndef _ADC_H
#define _ADC_H

#include <avr/io.h>

void init_ADC();
uint16_t read_ADC(uint8_t ch,long * storein);
	
#endif /* _ADC_H */
