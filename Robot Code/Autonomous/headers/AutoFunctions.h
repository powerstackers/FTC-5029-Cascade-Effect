#pragma config(StandardModel, "PS CASCADE EFFECT")

/*
*	AutoFunctions.h
*	Functions and subroutines for use during the autonomous period.
*
*	THIS CODE IS PROVIDED AS-IS AND WITHOUT WARRANTY.
*	THIS CODE IS OPEN FOR DISTRIBUTION AND MODIFICATION.
*
*	FTC Team #5029, The Powerstackers
*	powerstackersftc.com
*	github.com/powerstackers
*/

#include "Sensors.h"
#include "CollisionAvoidance.h"
#include "JoystickDriver.c"

/*
*
*	FUNCTION PROTOTYPES
*
*/
void initializeRobot();

// Utility functions
void allMotorsTo(int i);
void driveMotorsTo(int i);
long inchesToTicks(float inches);
float ticksToInches(long ticks);
void goTicks(long ticks, int speed, bool collisionAvoidance);
void turnDegrees(float degrees, int speed);

// Game functions
char findGoalOrientation();
void dropBall(int height);


/*
*
*	GLOBAL VARIABLES
*
*/
// ultrasonicThreshold
// Detecting an ultrasonic sensor value below this threshold will cause the robot to stop
int ultrasonicThreshold = 30;
int ticksPerRevolution = 1120;	// Each time the motors turn once, they turn 1120 ticks

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
	return (long) inches * (ticksPerRevolution/(4*PI));
}

/*
*	ticksToInches
*	Convert a number of ticks to a distance in inches
*/
float ticksToInches(long ticks)
{
	return (float) ticks * ((4*PI)/ticksPerRevolution);
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
	const float initialTurnReading = currentGryoReading();

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
	while(abs(degreesSoFar) < abs(degrees))
	{
		// 10 millisecond interval
		wait10Msec(1);

		float reading = currentGryoReading() - initialTurnReading;

		// Gyro sensor returns an angular speed. Distance=rate*time
		degreesSoFar += reading * 0.01;

	}

	driveMotorsTo(0);

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
	writeDebugStreamLine("--BATTERY LEVELS--\n\textBatt lvl: %2.2f volts\n\tNXT Batt lvl: %2.2f volts", 
		externalBatteryAvg / 1000.0, nAvgBatteryLevel / 1000.0);
	
	// If battery levels are low, notify the operators
	if(externalBatteryAvg < 13000){
		PlaySound(soundException);
		writeDebugStreamLine("--!! MAIN BATTERY LOW !!--\n\tAvg Batt Level: %2.2f", 
			externalBatteryAvg / 1000.0);
		
		// A negative reading may indicate that the battery is disconnected
		if(externalBatteryAvg/1000.0 < 0.0)
			writeDebugStreamLine("\tCheck that main battery is connected.");
		nxtDisplayTextLine(4, "MAIN BATT LOW");
	}
	else
		nxtDisplayTextLine(4, "MAIN BATT GOOD");

	if(nAvgBatteryLevel < 7500){
		PlaySound(soundException);
		writeDebugStreamLine("--!! NXT BATTERY LOW !!--\n\tAvg Batt Level: %2.2f", 
			nAvgBatteryLevel / 1000.0);
		nxtDisplayTextLine(5, "NXT BATT LOW");
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
*	Droping the balls into the tubes
*/
void dropBall(int height)
{

}



