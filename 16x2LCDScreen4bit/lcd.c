#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

void set_lcd_pins_direction(){
	LCD_D7_DIRECTION |= ( 1 << LCD_D7_BIT );
	LCD_D6_DIRECTION |= ( 1 << LCD_D6_BIT );
  	LCD_D5_DIRECTION |= ( 1 << LCD_D5_BIT );
    	LCD_D4_DIRECTION |= ( 1 << LCD_D4_BIT );
    	LCD_E_DIRECTION  |= ( 1 << LCD_E_BIT  );
   	LCD_RS_DIRECTION |= ( 1 << LCD_RS_BIT );
    	LCD_RW_DIRECTION |= ( 1 << LCD_RW_BIT );
}

void lcd_init( void ){
   	 _delay_ms( 100 );
    	LCD_RS_PORT &= ~( 1 << LCD_RS_BIT );
    	LCD_E_PORT &= ~( 1 << LCD_E_BIT );
    	LCD_RW_PORT &= ~( 1 << LCD_RW_BIT );
    	lcd_write(LCD_RESET);
    	_delay_ms( 10 );
    	lcd_write(LCD_RESET);
    	_delay_us( 200 );
    	lcd_write( LCD_RESET );
    	_delay_us( 80 );
    	lcd_write( LCD_FOUR_BIT_MODE );
    	lcd_check_busy_flag();
    	lcd_send_command( LCD_FOUR_BIT_MODE );
    	lcd_check_busy_flag();
    	lcd_send_command( LCD_DISPLAY_OFF );
    	lcd_check_busy_flag();
    	lcd_send_command( LCD_CLEAR );
    	lcd_check_busy_flag();
    	lcd_send_command( LCD_ENTRY_MODE );
    	lcd_check_busy_flag();
    	lcd_send_command( LCD_DISPLAY_ON );
}

void lcd_puts( uint8_t str[] ){
    	volatile int i = 0;
    	while ( str[i] != 0 ){
       		lcd_check_busy_flag();
        	lcd_putc( str[i] );
        	i++;
    	}
}

void lcd_putc( uint8_t dchar ){
    	LCD_RW_PORT &= ~( 1 << LCD_RW_BIT );;
    	LCD_RS_PORT |= ( 1 << LCD_RS_BIT );
    	LCD_E_PORT &= ~( 1 << LCD_E_BIT );
    	lcd_write( dchar );
    	lcd_write( dchar << 4 );
}

void lcd_send_command( uint8_t command ){
    	LCD_RW_PORT &= ~( 1 << LCD_RW_BIT );
    	LCD_RS_PORT &= ~( 1 << LCD_RS_BIT );
    	LCD_E_PORT &= ~( 1 << LCD_E_BIT );
    	lcd_write( command );
    	lcd_write( command << 4 );
}

void lcd_write(uint8_t data){
    	LCD_D7_PORT &= ~( 1<< LCD_D7_BIT );
    	
	if (data & 1 << 7 ){
        	LCD_D7_PORT |= ( 1 << LCD_D7_BIT );
        }
    	
	LCD_D6_PORT &= ~( 1 << LCD_D6_BIT );
        
 	if (data & 1 << 6 ){
        	LCD_D6_PORT |= ( 1 << LCD_D6_BIT );
	}

    	LCD_D5_PORT &= ~( 1 << LCD_D5_BIT );

    	if ( data & 1 << 5 ){
        	LCD_D5_PORT |= ( 1 << LCD_D5_BIT );
	}

    	LCD_D4_PORT &= ~( 1 << LCD_D4_BIT );
    	
	if ( data & 1 << 4 ){
        	LCD_D4_PORT |= ( 1 << LCD_D4_BIT );
    	}

    	LCD_E_PORT |= ( 1 << LCD_E_BIT );
    	_delay_us( 1 );
    	LCD_E_PORT &= ~( 1 << LCD_E_BIT );
    	_delay_us( 1 );
}

void lcd_check_busy_flag( void ){
    uint8_t flag_duplicate;
    LCD_D7_DIRECTION &= ~( 1 << LCD_D7_BIT );
    LCD_RS_PORT &= ~( 1 << LCD_RS_BIT);
    LCD_RW_PORT |= ( 1 << LCD_RW_BIT);
    
    do{
        flag_duplicate = 0;
        LCD_E_PORT |= ( 1 << LCD_E_BIT);
        _delay_us( 1 );
        flag_duplicate |= ( LCD_D7_PIN & ( 1 << LCD_D7_BIT ) );
        LCD_E_PORT &= ~( 1 << LCD_E_BIT );
        _delay_us( 1 );
        LCD_E_PORT |= ( 1 << LCD_E_BIT );
        _delay_us( 1 );
        LCD_E_PORT &= ~( 1 << LCD_E_BIT );
        _delay_us( 1 );
        
    } while ( flag_duplicate );
    
    LCD_RW_PORT &= ~( 1 << LCD_RW_BIT );
    LCD_D7_DIRECTION |= ( 1 << LCD_D7_BIT );
}

