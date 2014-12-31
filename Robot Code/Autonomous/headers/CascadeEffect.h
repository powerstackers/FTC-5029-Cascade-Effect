#include "AutoFunctions.h"
#include "CollisionAvoidance.h"

// Game functions
char findGoalOrientation();
void dropBall(int height);
void kickstand();

void initializeRobot();

/*
*	initializeRobot
*	Move all motors and servos to their starting positions
*/
void initializeRobot()
{
	// Clear the NXT screen
	bDisplayDiagnostics = false;
	eraseDisplay();

	// Measure and print the battery levels
	writeDebugStreamLine("--BATTERY LEVELS--\n\tTETRIX battery level: %2.2f volts\n\tNXT Battery level: %2.2f volts", 
		externalBatteryAvg / 1000.0, nAvgBatteryLevel / 1000.0);
	
	// If battery levels are low, notify the operators
	if(externalBatteryAvg < 13000){
		PlaySound(soundException);
		writeDebugStreamLine("--!! MAIN BATTERY LOW !!--\n\tAvg battery level: %2.2f", 
			externalBatteryAvg / 1000.0);
		
		// A negative reading may indicate that the battery is disconnected
		if(externalBatteryAvg/1000.0 < 0.0)
			writeDebugStreamLine("\tCheck that main battery is connected.");
		nxtDisplayCenteredTextLine(4, "MAIN BATT LOW");
	}
	else
		nxtDisplayTextLine(4, "MAIN BATT GOOD");

	if(nAvgBatteryLevel < 7500){
		PlaySound(soundException);
		writeDebugStreamLine("--!! NXT BATTERY LOW !!--\n\tAvg Batt Level: %2.2f", 
			nAvgBatteryLevel / 1000.0);
		nxtDisplayCenteredTextLine(5, "NXT BATT LOW");
	}
	else
		nxtDisplayTextLine(5, "NXT BATT GOOD");

	// Start getting information from the multiplexer(s)
	StartTask(getSmux);

	// Put all motors and servos into their starting positions
	allMotorsTo(0);
	
	writeDebugStreamLine("-- ROBOT INITIALIZED --");
}

// Macros for the different positions of the center goal
// The number indicates the average reading for the IR sensors when the goal is at that position
#define positionA 80
#define positionB 95
#define positionC 70

// Macros to store the different position names
#define CENTGOAL_POSITION_A 'a'
#define CENTGOAL_POSITION_B 'b'
#define CENTGOAL_POSITION_C 'c'

/*
*	findGoalOrientation
*	Find which way the center goal is facing
*/
char findGoalOrientation()
{
	writeDebugStreamLine("-- FINDING CENTER GOAL ORIENTATION --");
	
	// Make sure the multiplexer is turned on
	if(!gettingSmux)
		StartTask(getSmux);
	// Turn on IR capabilities, and give the sensors time to start up
	gettingIr = true;
	wait10Msec(35);

	// Read the average of the two IR seekers.
	// We may remove one sensor in the future.
	int avg = ((irStrengthLeft + irStrengthRight) / 2);
	
	// Find the difference between the average IR signal and the known values for each position
	int diffA = abs(avg - positionA);
	int diffB = abs(avg - positionB);
	int diffC = abs(avg - positionC);
	char facing;

	// The lowest difference is the position the center goal is facing
	if(diffA < diffB && diffA < diffC)
		facing = CENTGOAL_POSITION_A;
	else if(diffB < diffA && diffB < diffC)
		facing = CENTGOAL_POSITION_B;
	else
		facing = CENTGOAL_POSITION_C;
		
	// Print this information to the debug stream
	writeDebugStreamLine("\tLeft:\t\t%d", irStrengthLeft);
	writeDebugStreamLine("\tRight:\t\t%d", irStrengthRight);
	writeDebugStreamLine("\tAverage:\t%d", avg);
	writeDebugStreamLine("\tThe thing is in position %c", facing);

	return facing;
}

/*
*	dropBall
*	Dropping the balls into the tubes
*/
void dropBall(int height)
{

}

/*
*	kickstand
*	Knock over the kickstand
*/
void kickstand(){

}