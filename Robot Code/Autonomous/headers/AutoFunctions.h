#pragma config(StandardModel, "PS CASCADE EFFECT")

/*
*	AutoFunctions.h
*	Functions and subroutines for use during the autonomous period.
*
*	THIS CODE IS PROVIDED AS-IS AND WITHOUT WARRANTY.
*	THIS CODE IS OPEN FOR DISTRIBUTION AND MODIFICATION.
*
*	FTC Team #5029, The Powerstackers
*	powerstackersftc.weebly.com
*	github.com/powerstackers
*/



#include "Sensors.h"
#include "JoystickDriver.c"

/*
*
*	FUNCTION PROTOTYPES
*
*/
void initializeRobot();

// Utility functions
task avoidCollision();
void allMotorsTo(int i);
void driveMotorsTo(int i);
long inchesToTicks(float inches);
float ticksToInches(long ticks);
void goTicks(long ticks, int speed, bool collisionAvoidance);
void turnDegrees(float degrees, int speed);

int findGoalOrientation();
void dropBall(int height);


/*
*
*	GLOBAL VARIABLES
*
*/
// ultrasonicThreshold
// Detecting an ultrasonic sensor value below this threshold will cause the robot to stop
int ultrasonicThreshold = 30;

/*
*	allMotorsTo
*	Set all the motors to the input value
*/
void allMotorsTo(int i)
{
		motor[mDriveLeft] 	= i;
		motor[mDriveRight] 	= i;
		motor[mBrush] 		= i;
		motor[mLift] 		= i;
		writeDebugStreamLine("\tSet all motors to %d", i);
}

/*
*	driveMotorsTo
*	Sets all drive motors to the input value
*/
void driveMotorsTo(int i)
{
		motor[mDriveLeft] = i;
		motor[mDriveRight] = i;
		writeDebugStreamLine("\tSet drive motors to %d", i);
}

/*
*	inchesToTicks
*	Convert a distance in inches to a number of ticks
*/
long inchesToTicks(float inches)
{
	return (long) inches * 1.0;
}

/*
*	ticksToInches
*	Convert a number of ticks to a distance in inches
*/
float ticksToInches(long ticks)
{
	return (float) ticks * 1;
}

/*
*	goTicks
*	Move the robot a discance in ticks
*/
void goTicks(long ticks, int speed, bool collisionAvoidance)
{
	// Target encoder values for the left and right motors
	long targetRight = nMotorEncoder[mDriveRight] + ticks;
	long targetLeft = nMotorEncoder[mDriveLeft] + ticks;

	// Print what we are going to do to the debug stream
	writeDebugStreamLine("-- GOING TICKS --\n\tMoving %d ticks %s at %d speed",
		ticks, ((ticks>0)?"forward":"backward"), speed);

	// Turn on collision avoidance, if it's asked for
	if(collisionAvoidance)
	{
		StartTask(avoidCollision);
		writeDebugStreamLine("\t-- COLLISION AVOIDANCE ACTIVATED --");
	}

	// If we are going forwards or backwards
	if(ticks > 0)
	{
		// Set the drive motors to the given speed
		driveMotorsTo(speed);
		// Wait until both motors have reached the target
		while(nMotorEncoder[mDriveLeft] < targetLeft && nMotorEncoder[mDriveRight] < targetRight){}
		// Stop the drive motors here
		driveMotorsTo(0);
	}
	else
	{
		// Set the drive motors to the speed (in reverse)
		driveMotorsTo(-1 * speed);
		// Wait until both motors have reached the target
		while(nMotorEncoder[mDriveLeft] > targetLeft && nMotorEncoder[mDriveRight] > targetRight){}
		// Turn off the drive motors here
		driveMotorsTo(0);
	}
	writeDebugStreamLine("\tMoving done");

	// Turn off the collision avoidance system
	if(collisionAvoidance)
	{
		StopTask(avoidCollision);
		writeDebugStreamLine("\t-- COLLISION AVOIDANCE DEACTIVATED --");
	}
}

/*
*	turnDegrees
*	Turn the robot a certain number of degrees
*/
void turnDegrees(float degrees, int speed)
{
	writeDebugStreamLine("-- TURNING --\n\tTurning %d degrees at %d speed",
		degrees, speed);

	// Store the number of degrees turned so far
	float degreesSoFar = 0;

	// Take an initial reading of the accelerometer sensor
	const int initialTurnReading = accelX;

	// Decide whether to turn clockwise or counterclockwise
	if(degrees > 0)
	{
		motor[mDriveLeft] = -1 * speed;
		motor[mDriveRight] = speed;
	}
	else
	{
		motor[mDriveLeft] = speed;
		motor[mDriveRight] = -1 * speed;
	}

	// For as long as the current degree measure doesn't equal the target




	writeDebugStreamLine("\tTurning done");

}

/*
*	initializeRobot
*	Move all motors and servos to their starting positions
*/
void initializeRobot()
{
	// Clear the nxt screen
	bDisplayDiagnostics = false;
	eraseDisplay();

	// Measure and print the battery levels
	writeDebugStreamLine("extBatt lvl: %2.2f volts\nNXT Batt lvl: %2.2f", externalBatteryAvg / 1000.0, nAvgBatteryLevel / 1000.0);
	if(externalBatteryAvg < 13000){
		PlaySound(soundException);
		writeDebugStreamLine("--!! MAIN BATTERY LOW !!--\n\t Avg Batt Level: %2.2f", externalBatteryAvg / 1000.0);
		nxtDisplayTextLine(4, "MAIN BATT LOW");
	}
	else
		nxtDisplayTextLine(4, "MAIN BATT GOOD");

	if(nAvgBatteryLevel < 7500){
		PlaySound(soundException);
		writeDebugStreamLine("--!! NXT BATTERY LOW !!--\n\tAvg Batt Level: %2.2f", nAvgBatteryLevel / 1000.0);
		nxtDisplayTextLine(5, "NXT BATT LOW");
	}
	else
		nxtDisplayTextLine(5, "NXT BATT GOOD");

	// Start getting information from the multiplexer(s)
	StartTask(getSmux);

	writeDebugStreamLine("-- ROBOT INITIALIZED --");
	allMotorsTo(0);
}

/*
*	findGoalOrientation
*	Find which way the center goal is facing
*/
int findGoalOrientation()
{

}

/*
*	dropBall
*	droping the balls into the tubes
*/
void dropBall(int height)
{

}


/*
*	avoidCollision
*	Actively watch the ultrasonic sensors to prevent a collision
*/
task avoidCollision()
{
	// Loop forever
	while(true)
	{
		// If the ultrasonic sensors detect an obstruction
		if(ultraStrengthBack < ultrasonicThreshold || ultraStrengthFront < ultrasonicThreshold)
		{
			// Stop all motors
			allMotorsTo(0);
			// Stop all processes except this one (temporarily stops all other functions)
			hogCPU();

			// Print a STOP message to the NXT screen and debug stream
			writeDebugStreamLine("--!! OBSTRUCTION DETECTED !!--");
			nxtDisplayCenteredBigTextLine(2, "SUSPENDED");
			nxtDisplayCenteredTextLine(6, "--!!COLLISION!!--");
			nxtDisplayCenteredTextLine(7, "--!!DETECTED!!--");

			// Wait until the ultrasonic sensors are clear
			while(ultraStrengthBack < ultrasonicThreshold || ultraStrengthFront < ultrasonicThreshold){}
			// Start up all other processes again
			releaseCPU();

			// Write messages to the nxt and the debug stream
			writeDebugStreamLine("--!! OBSTRUCTION CLEARED !!--");
			nxtDisplayCenteredBigTextLine(2, "RUNNING");
			nxtDisplayCenteredTextLine(6, "--OBSTRUCTION--");
			nxtDisplayCenteredTextLine(7, "--CLEARED--");
		}
	}
}
