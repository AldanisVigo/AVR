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

// Program ID
uint8_t program_author[]   = "  Aldanis Vigo  ";
uint8_t machine_name[]     = " Exposure Unit  ";
uint8_t program_version[]  = "  Version 1.0   ";
uint8_t program_date[]     = "  Oct 31, 2015  ";

int main( void ){	
    	set_lcd_pins_direction();
	lcd_init();
    	lcd_puts( program_author );
    	_delay_ms(2000);
    	lcd_check_busy_flag();
    	lcd_send_command( LCD_CLEAR );
    	lcd_puts( machine_name );
    	lcd_check_busy_flag();
    	lcd_send_command( LCD_SET_CURSOR_POS | LCD_SECOND_LINE );
    	lcd_puts( program_version );
   	_delay_ms( 5000 );
    	lcd_check_busy_flag();
    	lcd_send_command( LCD_CLEAR );
    	//You can multiplex these pins by enabling and disabling the LCD display.
    	lcd_puts( "Lx = 15uH " );
    	lcd_check_busy_flag();
    	lcd_send_command( LCD_SET_CURSOR_POS | LCD_SECOND_LINE );
    	lcd_puts( "Cx = 150uF" );
    	lcd_check_busy_flag();
	_delay_ms(2000);
	lcd_send_command( LCD_CLEAR );
	lcd_puts("Hello Jenn!");
	while( 1 );
    	return 0;
}
