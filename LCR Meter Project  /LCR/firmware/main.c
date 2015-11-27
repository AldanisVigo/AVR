/* Name: main.c
 * Author: Aldanis Vigo
 * Copyright: Aldanis Vigo 2015
 * Descriptions
 */

#include <avr/io.h>
#include <math.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <string.h>
// Some macros that make the code more readable
#define set_low(port,pin) port &= ~(1<<pin)
#define set_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

//#define BAUDRATE 9600           //UART desired baud rate
//#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1) //scale it

//Declaration of our functions
//void USART_init(void);                  //UART Initialization Function
//unsigned char USART_receive(void);      //Retrieve data from UART
//void USART_send( unsigned char data);   //Send char through UART
//void USART_putstring(char* StringPtr);  //Send string through UART
//
/////*
//    Initialize the UART
//*/
//void USART_init(void){
//    
//    UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
//    UBRR0L = (uint8_t)(BAUD_PRESCALLER);
//    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
//    UCSR0C = (3<<UCSZ00);
//}
///*
//    Retrieve the contents of the UART RX Buffer
//*/
//unsigned char USART_receive(void){
//    
//    while(!(UCSR0A & (1<<RXC0)));
//    return UDR0;
//    
//}
///*
//    Send a single character through the UART
//*/
//void USART_send( unsigned char data){
//    
//    while(!(UCSR0A & (1<<UDRE0)));
//    UDR0 = data;
//    
//}
///*
//    Send a string through the UART
//*/
//void USART_putstring(char* StringPtr){
//    
//    while(*StringPtr != 0x00){
//        USART_send(*StringPtr);
//        StringPtr++;}
//    
//}

/*
    Initialize the Analog to Digital Converter
 */
void initADC(void)
{
    ADMUX = (1<<REFS0);
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
}
/*
    Read value from analog to digital converter channel
 */
int readADC(int ch)
{
    ch &= 0b00000111;
    ADMUX = (ADMUX & 0xF8)|ch;
    ADCSRA |= (1<<ADSC);
    while(ADCSRA & (1<<ADSC));
    return (ADC);
}
/*
    Initialize seial port with the desired baud rate.
 */
static void initSerial(unsigned long int baud)
{
    unsigned long int BAUD_PRESCALE = (((F_CPU / (baud * 16UL))) - 1);
    UBRR0H = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
    UBRR0L = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
    //    //Set the baud rate
    //    UBRR0H = (unsigned char) (baud >> 8);
    //    UBRR0L = (unsigned char) baud;
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); //8 bit character size asynchronous mode    UBRR0H = (unsigned char) (baud >> 8);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);   //Enable the transmitter and receiver
    //UCSR0B |= (1 << RXCIE0); // Enable the USART Recieve Complete interrupt (USART_RXC)
    //sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
    return;
}
unsigned char serial_Read(void)
{
    //    while ((UCSR0A & _BV(RXC0)) == 0)
    //    {;;}
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
    
}
void serial_Write(char DataOut)
{
    //    while ((UCSR0A & _BV(UDRE0)) == 0)
    //    {;;}
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = DataOut;
}/*
    Read a string from the serial port
*/
char * serial_Read_Word(){
    int index = 0;
    char * byte = "";
    unsigned char currentChar = serial_Read();
    while(currentChar != '\0'){ //As long as the currentCharacter is not a newline character
        byte[index] = currentChar; //set the character on the currentIdex to the currentChar
        index++; //increment to the next index
        currentChar = serial_Read(); //read the next character
    }
    return byte;
}
void serial_Write_Word(char * word){
    int count = 0;
    for(count = 0;count < strlen(word);count++){
        serial_Write(word[count]);
    }
}
void delaymilliseconds(int millis){
    int counter = 0;
    for(counter = 0;counter < millis;counter++){
        _delay_ms(1);
    }
}
/*
    Compare two character arrays.
    returns 0 when true, -1 when false
 */
int compare_string(unsigned char *first,unsigned char *second) {
    while (*first == *second) {
        if (*first == '\0' || *second == '\0')
            break;
        
        first++;
        second++;
    }
    
    if (*first == '\0' && *second == '\0')
        return 0;
    else
        return -1;
}

/*
 pulseWidth - Variable to count the width of incoming positive edge pulses          from the output of the Comparator into PB0(ICP1, pin 14).
 Period     - Variable to calculate the period with pulseWidth * 2
 TF         - Variable to calculate the frequency given the Period
 Inductance - Variable to store the value of the Inductance with                      1/(4pi^2 * TF^2 * KnownCapacitor)
*/
volatile uint16_t pulseWidth,Period,TF,Inductance;
const volatile uint16_t KnownCapacitor = .000006;

/*
 
 Function to initialize the timer in Input Capture mode with a prescaler of
 256.
 
 Calculations:
 16MHz/256 = 62.5KHz <---Counter Frequency
 1/62.5KHz = 16uS    <---Time per counter tick
 
*/
void initPulseCountTimer(){
    TCCR1A = 0x0F; //Normal Mode
    
    
    /* TCCR1B - Timer/Counter1 control register B Description
     ======================================================================
     • Bit 7 – ICNC1: Input capture noise canceler
                Setting this bit (to one) activates the Input Capture Noise Canceler. When the noise canceler is
                activated, the input from the Input Capture pin (ICP1) is filtered. The filter function requires four
                successive equal valued samples of the ICP1 pin for changing its output. The Input Capture is
                therefore delayed by four Oscillator cycles when the noise canceler is enabled.
     • Bit 6 – ICES1: Input capture edge select
                This bit selects which edge on the Input Capture pin (ICP1) that is used to trigger a capture
                event. When the ICES1 bit is written to zero, a falling (negative) edge is used as trigger, and
                when the ICES1 bit is written to one, a rising (positive) edge will trigger the capture.
                When a capture is triggered according to the ICES1 setting, the counter value is copied into the
                Input Capture Register (ICR1). The event will also set the Input Capture Flag (ICF1), and this
                can be used to cause an Input Capture Interrupt, if this interrupt is enabled.
                When the ICR1 is used as TOP value (see description of the WGM13:0 bits located in the
                TCCR1A and the TCCR1B Register), the ICP1 is disconnected and consequently the Input Capture
                function is disabled.
     • Bit 5 – Reserved bit
                This bit is reserved for future use. For ensuring compatibility with future devices, this bit must be
                written to zero when TCCR1B is written.
     • Bit 4:3 – WGM13:2: Waveform generation mode
                See TCCR1A Register description.
     • Bit 2:0 – CS12:0: Clock select
                The three Clock Select bits select the clock source to be used by the Timer/Counter, see Figure
                16-10 on page 127 and Figure 16-11 on page 128.
     
     Table 16-5. Clock select bit description.
     CS12 CS11 CS10 Description
     0    0    0    No clock source (timer/counter stopped)
     0    0    1    clkI/O/1 (no prescaling)
     0    1    0    clkI/O/8 (from prescaler)
     0    1    1    clkI/O/64 (from prescaler)
     1    0    0    clkI/O/256 (from prescaler)
     1    0    1    clkI/O/1024 (from prescaler)
     1    1    0    External clock source on T1 pin. Clock on falling edge.
     1    1    1    External clock source on T1 pin. Clock on rising edge.
     
                If external pin modes are used for the Timer/Counter1, transitions on the T1 pin will clock the
                counter even if the pin is configured as an output. This feature allows software control of the
                counting.
    */
    TCCR1B = 0x44; //            BIN:01000100 - DEC:682
    /*                           ||XXX|||
                                 ||   |||
                                 ||   |||--{Divide Clock by 256 prescaler}
                                 ||
                                 ||--------{Rising Edge Trigerred}
                                 |---------{Disable Digital Noise Canceler}
    */
    
    
    TCCR1C = 0x00; //Normal Mode
    
    
    
    /*TIMSK1 -  Timer/Counter1 interrupt mask register Description
     ======================================================================
     • Bit 7, 6 – Res: Reserved bits
                These bits are unused bits in the Atmel ATmega48/88/168, and will always read as zero.
     • Bit 5 – ICIE1: Timer/Counter1, input capture interrupt enable
                When this bit is written to one, and the I-flag in the Status Register is set (interrupts globally
                enabled), the Timer/Counter1 Input Capture interrupt is enabled. The corresponding Interrupt
                Vector (see “Interrupts” on page 56) is executed when the ICF1 Flag, located in TIFR1, is set.
     • Bit 4, 3 – Res: Reserved bits
                These bits are unused bits in the ATmega48/88/168, and will always read as zero.
     • Bit 2 – OCIE1B: Timer/Counter1, output compare B match interrupt enable
                When this bit is written to one, and the I-flag in the Status Register is set (interrupts globally
                enabled), the Timer/Counter1 Output Compare B Match interrupt is enabled. The corresponding
                Interrupt Vector (see “Interrupts” on page 56) is executed when the OCF1B Flag, located in
                TIFR1, is set.
     • Bit 1 – OCIE1A: Timer/Counter1, output compare A match interrupt    enable
                When this bit is written to one, and the I-flag in the Status Register is set (interrupts globally
                enabled), the Timer/Counter1 Output Compare A Match interrupt is enabled. The corresponding
                Interrupt Vector (see “Interrupts” on page 56) is executed when the OCF1A Flag, located in
                TIFR1, is set.
     • Bit 0 – TOIE1: Timer/Counter1, overflow interrupt enable
                When this bit is written to one, and the I-flag in the Status Register is set (interrupts globally
                enabled), the Timer/Counter1 Overflow interrupt is enabled. The corresponding Interrupt Vector
                (See “Interrupts” on page 56) is executed when the TOV1 Flag, located in TIFR1, is set.
     */
    TIMSK1 = 33;//            BIN:00100001 - DEC: 33
     /*                           XX|XXXX|
                                    |    |---Enable Overflow Interrupt
                                    |--------Enable Input Capture Interrupt
     */
    
    
    //Set the Timer/Counter1 Register to 0, so we can start counting from 0
    TCNT1 = 0x00;
}

/*
    Timer 1 Capture Interrupt Vector Function
    This function will execute every time a positive edge pulse is seen in the ICP1(pin 14) pin.
 
*/
ISR (TIMER1_CAPT_vect){
    pulseWidth = ICR1; //Save the duration of the pulse
    TCNT1 = 0x00; //Reset the timer to 0 for the next pulse
}
/*
    Main Entry Point
 */
int main(void)
{
    //Enable Global Interrupts
    sei();

    //Initialize the UART
    initSerial(9600);
    //    //Prepare Input Pin PB0(pin 14)
    DDRB &= ~(1 << PB0); // configure PB0 as an input
    //PORTB |= (1 << PB0); // enable the pull-up on PB0
    //Initialize the timer
    initPulseCountTimer();
//    

    
    //Prepare PC5 for I'm alive LED
    DDRC |= (1 << PC5);
//    int blinkcount = 10;
//    for(blinkcount = 10;blinkcount > 0;blinkcount--){
//        //LED ON
//        PORTC |= (1<<PC5); //Set the pin high
//        delaymilliseconds(1000);
//        //LED OFF
//        PORTC &= ~(1<<PC5); //Set the pin low
//        delaymilliseconds(1000);
//    }
    
    //Program Loop
    while(1){
        //LED ON
        PORTC |= (1<<PC5); //Set the pin high
        delaymilliseconds(1000);
        //LED OFF
        PORTC &= ~(1<<PC5); //Set the pin low
        delaymilliseconds(1000);
        //If the pulseWidth is greater than 0
//        if(pulseWidth > 0){
//            Period = 2 * pulseWidth;
//            TF = 1 / Period;
//            //Calculate the value of L using
//            //Inductance = 1 / ((4pi^2)(TF^2)(KnownCapacitor))
//            Inductance = 1 / (pow(M_PI,2) * pow(TF,2) * KnownCapacitor);
//            //Tell UART the value of Inductance
//            //serial_Write_Word((char*)Inductance);
//            delaymilliseconds(1000);
//        }
//        else{
//            serial_Write_Word("Connect a Coil!\n");
//            //Tell UART there is no inductor connected
//            delaymilliseconds(1000);
//        }
    }
    return 0;   /* never reached */
}
