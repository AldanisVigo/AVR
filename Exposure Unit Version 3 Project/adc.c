#include <avr/io.h>
#include "adc.h"

void init_ADC(){
	ADMUX = ( 1 << REFS0);
	ADCSRA = ( 1 << ADEN ) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //128 PreScale
}

uint16_t read_ADC(uint8_t ch, long * storein){
	ch = ch & 0b00000111;
	ADMUX |= ch;
	ADCSRA |= (1 << ADSC);
	while(!(ADCSRA & (1 << ADIF)));
	
	ADCSRA |= (1 << ADIF);
	*storein = ADC;
	return(ADC);	
}
