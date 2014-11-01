// Include files to handle the multiplexer and all the different sensors
#include "hitechnic-sensormux.h"
#include "hitechnic-irseeker-v2.h"

#define LEFT true
#define RIGHT false

// Create variables to store the sensor addresses
const tMUXSensor irLeft = msensor_S2_1;			// IR Sensor, NXT 2 MUX 1

// Create variables to store the sensor values
int irStrengthLeft;
int irDirectionLeft;

int irStrengthRight;
int irDirectionRight;

// Flag to turn on or off the IR seekers
bool gettingIr = false;
int dummy;

int getIRDirection(tMUXSensor sensor)
{
	return HTIRS2readACDir(sensor);
}

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

int getIREnhanced(tMUXSensor sensor, bool LEFT_OR_RIGHT)
{
	if(LEFT_OR_RIGHT == LEFT)
		HTIRS2readEnhanced(sensor, irDirectionLeft, irStrengthLeft);
	else if(LEFT_OR_RIGHT == RIGHT)
		HTIRS2readEnhanced(sensor, irDirectionRight, irStrengthRight);
}

/////////////////////////////////////////////////////////////////////////
//
//	Update the sensor values
//
////////////////////////////////////////////////////////////////////////
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
	}
}
