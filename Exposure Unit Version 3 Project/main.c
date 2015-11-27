/*
        Aldanis Vigo 
        November 22 2015
 
        Exposure Unit Version 3 Code
 
 */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define RESPONSEBUFFER 50
#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>
#include "main.h"
#include "uart.h"
#include "lcd.h"
#include "adc.h"
#include <stdbool.h>

// Program ID
uint8_t program_author[]   = "  Aldanis Vigo  ";
uint8_t machine_name[]     = " Exposure Unit  ";
uint8_t program_version[]  = "  Version 3.0   ";
uint8_t program_date[]     = "  Nov 22, 2015  ";

int startLCD_Show_Credits( void ){	
    	set_lcd_pins_direction();
	lcd_init();
    	lcd_puts( program_author );
    	_delay_ms(1000);
    	lcd_check_busy_flag();
    	lcd_send_command( LCD_CLEAR );
    	lcd_puts( machine_name );
    	lcd_check_busy_flag();
    	lcd_send_command( LCD_SET_CURSOR_POS | LCD_SECOND_LINE );
    	lcd_puts( program_version );
   	//_delay_ms( 5000 );
}
void display_Selection(long selection){
	lcd_check_busy_flag();
	lcd_send_command(LCD_CLEAR);
	lcd_puts(" Exposure Time: ");
	lcd_check_busy_flag();
	lcd_send_command(LCD_SET_CURSOR_POS | LCD_SECOND_LINE );
	char * zero = "0";
	
	uint8_t mins = selection / 60;
	char minsStr[5];
	itoa(mins,minsStr,10);
	lcd_check_busy_flag();
	lcd_puts("      ");
	if(mins <= 9){
		lcd_check_busy_flag();
		lcd_puts(zero);
	}

	lcd_check_busy_flag();
	lcd_puts(minsStr);
	lcd_check_busy_flag();
	lcd_puts(":");
	
	uint8_t secs = selection % 60;
	char secsStr[5];
	itoa(secs,secsStr,10);
	
	if(secs <= 9){
		lcd_check_busy_flag();
		lcd_puts(zero);
	}
	lcd_check_busy_flag();
	lcd_puts(secsStr);
}

long mapRange(long a1, long a2,long b1, long b2,long s){
	return b1 + (s - a1) * (b2 - b1) / (a2 - a1);
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
#define BUZZER 			PD2
#define BUZZER_DIR		DDRD
#define BUZZER_PORT		PORTD
void buzz(long frequency, long length) {
	long delayValue = 1000000 / frequency / 2;
	long numCycles = frequency * length / 1000;
	for(long i = 0; i < numCycles; i++) {
		BUZZER_PORT |= (1 << BUZZER);
		delayMicro(delayValue);
		BUZZER_PORT &= ~(1 << BUZZER);
		delayMicro(delayValue);	
	}
}
void buzzerSound(uint8_t reps){
	int rep = 0;
	for(rep = 0;rep < reps;rep++){
		buzz(NOTE_C1, 10000);
	}
}
void alarmSound(uint8_t reps){
	int rep = 0;
	for(rep = 0;rep < reps;rep++){
		int size = sizeof(melody) / sizeof(int);
		for(int thisNote = 0; thisNote < size; thisNote++){
			int noteDuration = 1000 / tempo[thisNote];
			buzz(melody[thisNote], noteDuration);
			int pauseBetweenNotes = noteDuration * 1.30;
			while(pauseBetweenNotes-- != 0)
				_delay_us(300);
			
			buzz(0, noteDuration);
		}
	}
}
#define LCD_LED_PIN PC1
#define LCD_LED_DIR DDRC
#define LCD_LED_PORT PORTC
void displayBlink(uint8_t reps){
	for(int i = reps;i > 0;i--){
		LCD_LED_PORT &= ~(1 << LCD_LED_PIN);	//Turn it off for a  second
		_delay_ms(1000);
		LCD_LED_PORT |= (1 << LCD_LED_PIN);	//Turn it on for a second
		_delay_ms(1000);
	}
	PORTC |= (1 << LCD_LED_PIN); //Leave it turned on
}
#define LIGHTS_SIGNAL		PC5
#define LIGHTS_SIGNAL_DIR	DDRC
#define LIGHTS_SIGNAL_PORT 	PORTC

#define START_BUTTON  		PC4
#define START_BUTTON_DIR 	DDRC
#define START_BUTTON_PIN 	PINC

#define STOP_BUTTON 		PC3
#define STOP_BUTTON_DIR 	DDRC
#define STOP_BUTTON_PIN 	PINC

#define TRIP_SWITCH	 	PC2
#define TRIP_SWITCH_DIR 	DDRC
#define TRIP_SWITCH_PIN		PINC
int main(void){
	//startLCD_Show_Credits();
    	//lcd_check_busy_flag();
    	//lcd_send_command( LCD_CLEAR );
    	//You can multiplex these pins by enabling and disabling the LCD display.
    	//lcd_puts( "Lx = 15uH " );
    	//lcd_check_busy_flag();
    	//lcd_send_command( LCD_SET_CURSOR_POS | LCD_SECOND_LINE );
    	//lcd_puts( "Cx = 150uF" );
	//lcd_check_busy_flag();
	//_delay_ms(2000);
	//lcd_send_command( LCD_CLEAR );
	//lcd_puts("Hello Jenn!");
	uart_init();
	stdout = &uart_tx;
	stdin = &uart_rx;
	init_ADC();

	//Set Pin Directions
	//Lights Pin as output
	LIGHTS_SIGNAL_DIR |= (1 << LIGHTS_SIGNAL);
	
	//Start Button Pin as input
	START_BUTTON_DIR &= ~(1 << START_BUTTON);
	
	//Stop Button Pin as Input
	STOP_BUTTON_DIR &= ~(1 << STOP_BUTTON);

	//Buzzer Pin as Output
	BUZZER_DIR |= (1 << BUZZER);

	//Trip Switch Pin as Input
	TRIP_SWITCH_DIR &= ~(1 << TRIP_SWITCH_PIN);

	//LCD Led Pin as output
	LCD_LED_DIR |= (1 << LCD_LED_PIN);
	LCD_LED_PORT |= (1 << LCD_LED_PIN); //Turn it on

	static long reading;
	static bool btnStop = false; 
	static bool tripSwitch = false;
	startLCD_Show_Credits();
	displayBlink(3);
	while (1){
		printf("Raw Reading: %d\t",read_ADC(0,&reading));
		long mapped_reading = mapRange(0,1023,0,1800,reading);
		printf("Mapped Reading: %d\t",mapped_reading);
		display_Selection(mapped_reading);
		if(START_BUTTON_PIN & (1 << START_BUTTON) && mapped_reading > 0){
			lcd_check_busy_flag();
			lcd_send_command(LCD_CLEAR);
			lcd_puts("    STARTING    ");
			displayBlink(2);
			long count = 0;
			_delay_ms(1000);
			//Turn on lights
			LIGHTS_SIGNAL_PORT |= (1 << LIGHTS_SIGNAL);
			bool lightsOn = true;
			for(count = mapped_reading;count >= 0;count--){
				//Check for stop button
				if(STOP_BUTTON_PIN & (1 << STOP_BUTTON)){
					lcd_check_busy_flag();
					lcd_send_command(LCD_CLEAR);
					lcd_puts("    STOPPING    ");
					btnStop = true;
					break;
				}
				//Pause if trip switch is high
				if(!(TRIP_SWITCH_PIN & (1 << TRIP_SWITCH))){
					if(!lightsOn){
						LIGHTS_SIGNAL_PORT |= (1 << LIGHTS_SIGNAL);
					}
					//If the the switch is still closed
					printf("UV Lights on for: %d more secs...\n",count);
					lcd_check_busy_flag();
					lcd_send_command(LCD_CLEAR);
					lcd_puts("    Exposing    ");
					lcd_check_busy_flag();
					lcd_send_command(LCD_SET_CURSOR_POS | LCD_SECOND_LINE);
				        char * zero = "0";	
					uint8_t mins = count / 60;
					uint8_t secs = count % 60;
					char minsStr[5];
					char secsStr[5];
					itoa(mins,minsStr,10);
					itoa(secs,secsStr,10);
					lcd_check_busy_flag();
					lcd_puts("      ");
					if(mins <= 9){
						lcd_check_busy_flag();
						lcd_puts(zero);
					}
					lcd_check_busy_flag();
					lcd_puts(minsStr);
					lcd_check_busy_flag();
					lcd_puts(":");
					if(secs <= 9){
						lcd_check_busy_flag();
						lcd_puts(zero);
					}
					lcd_check_busy_flag();
					lcd_puts(secsStr);  	
					_delay_ms(1000);
				}
				else{	
					lightsOn = false;
					LIGHTS_SIGNAL_PORT &= ~(1 << LIGHTS_SIGNAL);
					//Tell user the lid is open
					printf("LID OPEN, CLOSE IT TO CONTINUE\n");
					lcd_check_busy_flag();
					lcd_send_command(LCD_CLEAR);
					lcd_puts("    LID OPEN     ");
					lcd_check_busy_flag();
					lcd_send_command(LCD_SET_CURSOR_POS | LCD_SECOND_LINE);
					lcd_puts(" PLEASE CLOSE IT ");
					//Increment Count to make up for this one
					count++;
				}
				 	
			}
			//Turn off lights
			LIGHTS_SIGNAL_PORT &= ~(1 << LIGHTS_SIGNAL);
			if(btnStop != true){
				lcd_check_busy_flag();
				lcd_send_command(LCD_CLEAR);
				lcd_puts("    FINISHED    ");
				alarmSound(2);
			}
			else{
				buzzerSound(13);
				btnStop = false; //reset the stop flag
			}
			displayBlink(2);
		}	
	}
    	return 0;
}
