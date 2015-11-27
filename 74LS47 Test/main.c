//Defines
#ifndef BAUD
#define BAUD 9600
#endif
#define F_CPU 16000000UL
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <util/setbaud.h>
#include <stdbool.h>

//Prototypes
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

//Globals
FILE uart_tx = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_rx = FDEV_SETUP_STREAM(NULL,uart_getchar, _FDEV_SETUP_READ);

void init_UART(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
}

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

void init_ADC(){
	ADMUX=(1<<REFS0);                         // For Aref=AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Rrescalar div factor =128
}

uint16_t read_ADC(uint8_t ch,long * storein)
{
	//Select ADC Channel ch must be 0-7
   	ch=ch&0b00000111;
   	ADMUX|=ch;
	//Start Single conversion
   	ADCSRA|=(1<<ADSC);
   	//Wait for conversion to complete
   	while(!(ADCSRA & (1<<ADIF)));
   	//Clear ADIF by writing one to it
   	//Note you may be wondering why we have write one to clear it
   	//This is standard way of clearing bits in io as said in datasheets.
   	//The code writes '1' but it result in setting bit to '0' !!!
	ADCSRA|=(1<<ADIF);
	*storein = ADC;
	return(ADC);
}


void firstDisplay(uint8_t digit){
	//digit = (digit << 4);
	//digit = ~digit;
	//clear the display first
	int cur_bit = 0;
	for(cur_bit = 0;cur_bit < 4;cur_bit++){
		if(CHECK_BIT(digit,cur_bit)){
			switch(cur_bit){
				case 3:
					PORTD |= (1 << PD5);
					break;				
				case 2:
					PORTD |= (1 << PD6);
					break;
				case 1:
					PORTD |= (1 << PD7);
					break;
				case 0:
					PORTB |= (1 << PB0);
					break;
				default:
					break;
			}
		}
		else{
			switch(cur_bit){
				case 3:
					PORTD &= ~(1 << PD5);
					break;
				case 2:
					PORTD &= ~(1 << PD6);
					break;
				case 1:
					PORTD &= ~(1 << PD7);
					break;
				case 0:
					PORTB &= ~(1 << PB0);
					break;
				default:
					break;
			}
		}
	}
}
void secondDisplay(uint8_t digit){
        //digit = (digit << 4);
        //digit = ~digit;
        //clear the display first
        int cur_bit = 0;
        for(cur_bit = 0;cur_bit < 4;cur_bit++){
                if(CHECK_BIT(digit,cur_bit)){
                        switch(cur_bit){
                                case 3:
                                        PORTC |= (1 << PC1);
                                        break;
                                case 2:
                                        PORTC |= (1 << PC2);
                                        break;
                                case 1:
                                        PORTC |= (1 << PC3);
                                        break;
                                case 0:
                                        PORTC |= (1 << PC4);
                                        break;
                                default:
                                        break;
                        }
                }
                else{
                        switch(cur_bit){
                                case 3:
                                        PORTC &= ~(1 << PC1);
                                        break;
                                case 2:
                                        PORTC &= ~(1 << PC2);
                                        break;
                                case 1:
                                        PORTC &= ~(1 << PC3);
                                        break;
                                case 0:
                                        PORTC &= ~(1 << PC4);
                                        break;
                                default:
                                        break;
                        }
                }
        }
}

void display_Selection(uint8_t selection){
	if(selection < 10){
		firstDisplay(0);
		secondDisplay(selection);
	}
	else{
		uint8_t first_digit = selection / 10;
		uint8_t second_digit = selection % 10;
		firstDisplay(first_digit);
		secondDisplay(second_digit);
	}
}

long mapRange(long a1,long a2,long b1,long b2,long s)
{
	return b1 + (s-a1)*(b2-b1)/(a2-a1);
}


void displayBlink(uint8_t reps){
	int rep = 0;
	for(rep = 0; rep < reps; rep++){
		firstDisplay(0b1111);
		secondDisplay(0b1111);
		_delay_ms(500);
		firstDisplay(8);
		secondDisplay(8);
		_delay_ms(500);
	}
}

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};
void delayMicro(uint8_t us){
	while(us-- != 0){
		_delay_us(1);
	}
}
void buzz(long frequency, long length) {
 	long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  	//// 1 second's worth of microseconds, divided by the frequency, then split in half since
  	//// there are two phases to each cycle
  	long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  	//// multiply frequency, which is really cycles per second, by the number of seconds to
  	//// get the total number of cycles to produce
  	for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    		PORTD |= (1 << PD2); // write the buzzer pin high to push out the diaphram
    		delayMicro(delayValue); // wait for the calculated delay value
    		PORTD &= ~(1 << PD2); // write the buzzer pin low to pull back the diaphram
    		delayMicro(delayValue); // wait again or the calculated delay value
  
	}
}
void alarmSound(uint8_t reps){
	int rep = 0;
	for(rep = 0;rep < reps;rep++){
		int size = sizeof(melody) / sizeof(int);
    		for (int thisNote = 0; thisNote < size; thisNote++) {
 			// to calculate the note duration, take one second
      			// divided by the note type.
      			//e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
     		 	int noteDuration = 1000 / tempo[thisNote];
 
	      		buzz(melody[thisNote], noteDuration);
	 
	      		// to distinguish the notes, set a minimum time between them.
	      		// the note's duration + 30% seems to work well:
	      		int pauseBetweenNotes = noteDuration * 1.30;
	      		while(pauseBetweenNotes-- != 0)
				_delay_us(300);
			
	 
	     	 	// stop the tone playing:
	      		buzz(0, noteDuration);
	    	}
	}		
}
int main (void){
	//Start UART
	init_UART();
	stdout = &uart_tx;
	stdin = &uart_rx;
	
	//Start ADC
	init_ADC();

	//Set pins for display as output
	//Digit 1
	DDRD |= (1 << PD5); //A
	DDRD |= (1 << PD6); //B
	DDRD |= (1 << PD7); //C
	DDRB |= (1 << PB0); //D
	
	//Digit 2
	DDRC |= (1 << PC1); //A
	DDRC |= (1 << PC2); //B
	DDRC |= (1 << PC3); //C
	DDRC |= (1 << PC4); //D

	//Lights Pin Output
	DDRC |= (1 << PC5); 
 
	//Start/Stop Button
	DDRB &= ~(1 << PB1); //Start button input	
	DDRB &= ~(1 << PB2); //Stop button input

	//Buzzer Pin
	DDRD |= (1 << PD2); //Output

   	int first_digit = 9;
	int second_digit = 10;    
    	static long reading;
	static bool btnStart;

	//Blink display to let me know everything is good!
	displayBlink(5);
	while(1) {
		printf("Raw Reading: %d\t",read_ADC(0,&reading));
		long mapped_reading = mapRange(0,1023,0,99,reading);
		printf("Mapped Reading: %d\n",mapped_reading);
		display_Selection(mapped_reading);
		if(PINB & (1 << PB1) && mapped_reading > 0 ){ //If the Start button gets pressed then
			displayBlink(2);
			int count = 0;
			_delay_ms(1000);
			PORTC |= (1 << PC5);
			for(count = mapped_reading;count >= 0;count--){
				//Count Backwards from that number	
				//Check for stop button quit if set
				if(PINB & (1 << PB2))
					break;

				uint8_t fdigitm = count / 10;
				uint8_t sdigitm = count % 10;
				firstDisplay(fdigitm);
				secondDisplay(sdigitm);
				printf("UV Lights on for: %d more secs...\n",count);
				_delay_ms(1000);
			}	
			PORTC &= ~(1 << PC5);
			//displayBlink(5);
			alarmSound(2);
			displayBlink(2);
		}
	}
}

