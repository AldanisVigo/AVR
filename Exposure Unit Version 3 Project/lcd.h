/*
 
 -------------                 ------------
 | ATmega168 |                 |   LCD    |
 |           |                 |          |
 |        PD7|---------------->|D7        |
 |        PD6|---------------->|D6        |
 |        PD5|---------------->|D5        |
 |        PD4|---------------->|D4        |
 |           |                 |D3        |
 |           |                 |D2        |
 |           |                 |D1        |
 |           |                 |D0        |
 |           |                 |          |
 |        PB1|---------------->|E         |
 |        PB2|---------------->|RW        |
 |        PB0|---------------->|RS        |
 -------------                 ------------
 */
//Set these for your LCD
#define LCD_D7_PORT     	PORTD
#define LCD_D7_BIT      	PD7
#define LCD_D7_DIRECTION  	DDRD
#define LCD_D7_PIN      	PIND
#define LCD_D6_PORT     	PORTD
#define LCD_D6_BIT     		PD6
#define LCD_D6_DIRECTION      	DDRD
#define LCD_D5_PORT     	PORTD
#define LCD_D5_BIT      	PD5
#define LCD_D5_DIRECTION      	DDRD
#define LCD_D4_PORT     	PORTD
#define LCD_D4_BIT      	PD4
#define LCD_D4_DIRECTION      	DDRD
#define LCD_E_PORT      	PORTB
#define LCD_E_BIT       	PB1
#define LCD_E_DIRECTION       	DDRB
#define LCD_RS_PORT     	PORTB
#define LCD_RS_BIT      	PB0
#define LCD_RS_DIRECTION      	DDRB
#define LCD_RW_PORT     	PORTB
#define LCD_RW_BIT      	PB2
#define LCD_RW_DIRECTION      	DDRB

#define LCD_FIRST_LINE     	0x00
#define LCD_SECOND_LINE     	0x40
//For 20x4 Displays
//#define   LCD_THIRDLINE   	0x14                  // start of line 3 (20x4)
//#define   LCD_FOURTHLINE    	0x54                  // start of line 4 (20x4)

//For 16x4 Displays
//#define   LCD_THIRDLINE   	0x10                  // start of line 3 (16x4)
//#define   LCD_FOURTHLINE    	0x50                  // start of line 4 (16x4)


// LCD instructions
#define LCD_CLEAR           	0b00000001 // replace all characters with ASCII 'space'
#define LCD_CURSOR_HOME         0b00000010 // return cursor to first position on first line
#define LCD_ENTRY_MODE      	0b00000110 // shift cursor from left to right on read/write
#define LCD_DISPLAY_OFF      	0b00001000 // turn display off
#define LCD_DISPLAY_ON      	0b00001100 // display on, cursor off, don't blink character
#define LCD_RESET   		0b00110000 // reset the LCD
#define LCD_FOUR_BIT_MODE 	0b00101000 // 4-bit data, 2-line display, 5 x 7 font
#define LCD_SET_CURSOR_POS     	0b10000000 // set cursor position

#include <avr/io.h>
#include <util/delay.h>

// Function Prototypes
void lcd_write( uint8_t );
void lcd_send_command( uint8_t );
void lcd_putc( uint8_t );
void lcd_puts( uint8_t *);
void lcd_init( void );
void lcd_check_busy_flag( void );
void set_lcd_pins_direction();
