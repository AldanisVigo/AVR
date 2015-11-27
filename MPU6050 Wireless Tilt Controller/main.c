#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "mpu6050.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

int16_t axg, ayg, azg, gxds, gyds, gzds, roll, pitch;
void pwm_init(){
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11); //Non Inverted PWM
	//Set mode to 14(FAST PWM)
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS12); //Prescaler 256
	//PWMOut = FLCK / PRE (1 + TOP)
	//PRE * PWMOut * (1 + TOP) = FLCK
	//(1 + TOP) = FLCK/(PRE * PWMOut)
	//TOP = (FLCK/(PRE * PWMOut)) - 1
        //TOP = (16MHz/(256 * 50)) - 1
	//TOP = 1250
	ICR1 = 1250; //50Hz Duty Cycle 
	DDRB |= (1 << PB2); //Set OC1B as output
}
long mapRange(long a1,long a2,long b1, long b2, long s){
	//long percision = (s - a1) / (a1 - a2);
	return b1 + (s - a1) * (b2 - b1) / (a2 - a1);
	//return (b1 + percision * (b2 - b1));
}
long ServoRotateTo(long degree){
	//A = X1 = -45 , Y1 = leftPWMLimit
	//C = X2 = 45  , Y2 = rightPWMLimit
	//B X = deg , Y = OCR1B
	//mAC = mAB

	//Y2 - LPWMlmt    OCR1B - LPWMlmt
	//------- =  ----------
	//X2 - X1    deg - X1

	//ICR1 - 0      OCR1B - 0
	//---------  = ----------
	//45 - (-45)    deg - (-45)
	
	//1250 - 0	OCR1B - 0
	//--------   =  ---------
	//45 + 45       deg + 45
	
	//1250		OCR1B
	//----       =  --------
	// 90           deg + 45
	
	//13.88888889 = OCR1B
	//		--------
	//		deg + 45
	
	//X2 = ServoRightLimit
	//X1 = ServoLeftLimit
	//Y2 = ICR1
	//Y1 = 0
	double LPwmLmt = 35;
	double RPwmLmt = 90;
	double rightLimit = 45;
	double leftLimit = -45.0;
	double stepSize = (rightLimit - leftLimit) / RPwmLmt;
	double stepSizeCompensation = .34;
	stepSize  += stepSizeCompensation;
	if(degree < leftLimit){
		degree = leftLimit;
	}
	if(degree > rightLimit)
	{
		degree = rightLimit;
	}
	double val = stepSize * (degree - leftLimit);
	if(val < 35){
		val = 30;
	}
	if(val > 90){
		val = 95;
	}
	OCR1B = val;
	return val;
}
int main(void){
	uart_init();
	//pwm_init();
	stdin = &uart_rx;
	stdout = &uart_tx;
	mpu6050_init();
	pwm_init();
	//sei();
	if(mpu6050_testConnection()){
		printf("The MPU6050 initialized!\n");
		while(1){
			//Read the raw values from the MPU6050
			mpu6050_getRawData(&axg,&ayg,&azg,&gxds,&gyds,&gzds);
			//Calculate Roll and Pitch angles using Trig
			int8_t Roll = atan2(ayg,azg) * 180 / M_PI;
			int8_t Pitch = atan2(-axg,azg) * 180 / M_PI;
			//Display Results
			long converted = ServoRotateTo(Roll);	
			printf("X-Raw:%d X-Deg:%d˚ X-Converted:%ld\n",ayg,Roll,converted);
			//_delay_ms(2000);
		}
	}
}
