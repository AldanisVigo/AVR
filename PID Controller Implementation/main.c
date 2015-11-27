//Proportional Integral Derrivative Controller
#include <avr/io.h>
#include <stdbool.h>

//

unsigned long PreviousTime;
float Input, Output , Target , IntegralTerm , ErrorSum , PreviousInput , ProportionalGain , IntegralGain , DerivativeGain;
float outputMinimum, outputMaximum;
uint8_t SampleInterval = 1000; //1000ms = 1s
bool Automatic = false;

unsigned long currentTime(){
	//Implement a way to get current time
	return unsigned long time;
}
void Calculate()
{
	if(!Automatic) return;
	unsigned long now = currentTime();
	uint8_t TimeDifference = (now - PreviousTime);
	if(TimeDifference >= SampleInterval){
		float Error = Target - Input;
		//ErrorSum = ErrorSum + Error;
		IntegralTerm = IntegralTerm + (IntegralGain * error);
		if(IntegralTerm > outputMaximum){
			IntegralTerm = outputMaximum;
			
		}
		else if(IntegralTerm < outputMinimum){
			IntegralTerm = outputMinimum;
		}
		
		float InputDerivative = (Input - PreviousInput);
		Output = ProportionalGain * Error + IntegralTerm - DerivativeGain * InputDerivative;
		if(Output > outputMaximum){
			Output = outputMaximum;
		}
		else if(Output < ouputMinimum){
			Output = outputMinimum;
		}
		PreviousInput = Input;
		PreviousTime = now;
	}
}

void Tune(float Kp,float Ki,float Kd){
	ProportionalGain = Kp;
	IntegralGain = Ki * ((float)SampleInterval/1000);
	DerivativeGain = Kd / ((float)SampleInterval/1000);
}

void SetSampleInterval(uint8_t dTime){
	if(dTime > 0){
		float SampleTimeRatio = (float)dTime / (float)SampleInterval;
		IntegralGain = IntegralGain * SampleTimeRatio;
		DerivativeGain = IntegralGain / SampleTimeRatio;
		SampleInterval = (unsigned long)dTime;
	}
}

