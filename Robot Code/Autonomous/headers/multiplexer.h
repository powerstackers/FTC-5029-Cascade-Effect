// Include files to handle the multiplexer and all the different sensors
#include "hitechnic-sensormux.h"
#include "hitechnic-irseeker-v2.h"
#include "lego-ultrasound.h"

// Constants
const bool LEFT = true;
const bool RIGHT = false;

// Macros to store the sensor addresses
#define irLeft  	msensor_S2_1		// IR Sensor, NXT 2 MUX 1
#define irRight  	msensor_S2_2 		// IR Sensor, NXT 2 MUX 2
#define ultraBack	msensor_S2_3		// Ultrasonic, NXT 2 MUX 3
#define ultraFront	msensor_S2_4		// Ultrasonic, NXT 2 MUX 4

// Variables to store the sensor values
int irStrengthLeft;
int irDirectionLeft;

int irStrengthRight;
int irDirectionRight;

int ultraStrengthBack;
int ultraStrengthFront;


// Flag to turn on or off the IR seekers
bool gettingIr = false;
int dummy;

/*
*	getIRDirection
*	Gets the overall direction of the chosen IR seeker
*/
int getIRDirection(tMUXSensor sensor)
{
	return HTIRS2readACDir(sensor);
}

/*
*	getIRStrength
*	Gets the overall strength of the chosen IR seeker
*/
int getIRStrength(tMUXSensor sensor)
{
	int strength;
	int acS1, acS2, acS3, acS4, acS5 = 0;

	HTIRS2readAllACStrength(sensor, acS1, acS2, acS3, acS4, acS5 );

	strength = (acS1 > acS2) ? acS1 : acS2;
	strength = (strength > acS3) ? strength : acS3;
	strength = (strength > acS4) ? strength : acS4;
	strength = (strength > acS5) ? strength : acS5;

	return 	strength;
}

/*
*	getIREnhanced
*	Get both the direction and the strength of the chosen IR seeker
*/
void getIREnhanced(tMUXSensor sensor, bool LEFT_OR_RIGHT)
{
	if(LEFT_OR_RIGHT == LEFT)
		HTIRS2readEnhanced(sensor, irDirectionLeft, irStrengthLeft);
	else if(LEFT_OR_RIGHT == RIGHT)
		HTIRS2readEnhanced(sensor, irDirectionRight, irStrengthRight);
}

/*
*	getIREnhanced
*	Get both the direction and the strength of the chosen IR seeker
*/
void getIREnhanced(tSensors sensor, bool LEFT_OR_RIGHT)
{
	if(LEFT_OR_RIGHT == LEFT)
		HTIRS2readEnhanced(sensor, irDirectionLeft, irStrengthLeft);
	else if(LEFT_OR_RIGHT == RIGHT)
		HTIRS2readEnhanced(sensor, irDirectionRight, irStrengthRight);
}

/*
*	getSmux
*	Update all the sensors attached to a multiplexer
*/
task getSmux()
{
	// Print a ready message
	writeDebugStreamLine("Multiplexer setup ready");

	// Loop forever
	while (true){
		// Only update the IR seeker variables when the IR seekers are turned on
		// This keeps the debug stream clear
		if(gettingIr){
			if(!HTIRS2readEnhanced(irLeft, dummy, irStrengthLeft)){
				writeDebugStreamLine("Something's wrong with the IR");
			}

		}

		// Store the values of the ultrasonic sensors
		ultraStrengthBack = USreadDist(ultraBack);
		ultraStrengthFront = USreadDist(ultraFront);
	}
}
