#pragma config(StandardModel, "PS CASCADE EFFECT")

/*
*	CollisionAvoidance.h
*	Functions and tasks to avoid on-field collisions.
*
*	THIS CODE IS PROVIDED AS-IS AND WITHOUT WARRANTY.
*	THIS CODE IS OPEN FOR DISTRIBUTION AND MODIFICATION.
*
*	FTC Team #5029, The Powerstackers
*	powerstackersftc.com
*	github.com/powerstackers
*/

#include "Sensors.h"
#include "AutoFunctions.h"

/*
*	GLOBAL CONSTANTS
*/
// Detecting an ultrasonic sensor value below this threshold will cause the robot to stop
#define ultrasonicThreshold 30

/*
*	FUNCTION PROTOTYPES
*/
task avoidCollision();
float detour();

/*
*	detour
*	Use the gyro sensor and encoders to clear an obstruction.
*/
float detour(){
	/*
	*	If the sonar sensors detect an obstruction, the robot will try to find a safe path around
	*	whatever it is that's in its way. The robot will turn 90 degrees, and use its side sensor
	*	to determine when it is past the obstruction. Then, it will turn 90 degrees back the other
	*	way (now facing in the original heading) and use its side sensors again to move past the 
	*	obstruction. It will then turn 90 degrees again, and then once more, and it will then be
	*	back on the original path, past the obstruction.
		This function will be called while a goTicks function is running. To keep from ruining the count
	*	used by that function, we have to cut out the encoder ticks travelled during the evasive manouver.
	*	We recorded the original encoder value from before we started the detour, and we recorded the
	*	amount of ticks it took to run parallel to the original path, past the obstruction. At the end, we
	*	will add those two numbers together, and the original goTicks function will still have an accurate
	*	count.
	*/
	
	// Store the motor encoder value before the manouver
	long startingEncoderValue = nMotorEncoder[mDriveLeft];
	
	// Turn 90 degrees counter-clockwise, now driving perpendicular to original path
	turnDegrees(90.0, 30);
	
	// Store the encoder value before moving clear of the obstruction
	long encoderBeforeTurn1 = nMotorEncoder[mDriveLeft];
	
	// While the back sensor detects an obstruction, keep moving
	while(ultraStrengthBack<ultrasonicThreshold)
		driveMotorsTo(25);
	driveMotorsTo(0);
	
	// Determine the number of ticks it took to travel past the obstruction
	long ticksTraveledPerpendicular = nMotorEncoder[mDriveLeft] - encoderBeforeTurn1;
	
	// Turn 90 degrees, now parallel to the original path, but moved over past the obstruction
	turnDegrees(-90.0, 30);
	
	long encoderBeforeParallel = nMotorEncoder[mDriveLeft];
	
	// Moving past the obstruction. While the sensor detects an object, keep going.
	while(ultraStrengthBack<ultrasonicThreshold)
		driveMotorsTo(25);
	driveMotorsTo(0);

	long ticksParallel = nMotorEncoder[mDriveLeft] - encoderBeforeParallel;
	
	turnDegrees(-90.0, 30);
	
	// Moving perpendicular to the original path, back to the original heading
	long encoderBeforeTurn2 = nMotorEncoder[mDriveLeft];
	while(nMotorEncoder[mDriveLeft]<encoderBeforeTurn2+ticksTraveledPerpendicular)
		driveMotorsTo(25);
	driveMotorsTo(0);
	
	// Turn back to the original heading
	turnDegrees(90.0, 30);
	
	// Set the encoder value to the original encoder value added to the distance travelled parallel to the path.
	// This cuts out all the ticks travelled perpendicular to the path, and puts the goTicks function back on track.
	nMotorEncoder[mDriveLeft] = startingEncoderValue + ticksParallel;
}

/*
*	avoidCollision
*	Actively watch the ultrasonic sensors to prevent a collision
*/
bool avoidanceActive = false;
task avoidCollision()
{
	avoidanceActive = true;
	writeDebugStreamLine("--COLLISION AVOIDANCE ACTIVATED--")
	// Loop until the switch is thrown
	while(avoidanceActive)
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
			//while(ultraStrengthBack < ultrasonicThreshold || ultraStrengthFront < ultrasonicThreshold){}
			
			// Go around the obstruction
			detour();
			
			// Start up all other processes again
			releaseCPU();

			// Write messages to the nxt and the debug stream
			writeDebugStreamLine("--!! OBSTRUCTION CLEARED !!--");
			nxtDisplayCenteredBigTextLine(2, "RUNNING");
			nxtDisplayCenteredTextLine(6, "--OBSTRUCTION--");
			nxtDisplayCenteredTextLine(7, "--CLEARED--");
		}
	}
	
	writeDebugStreamLine("--COLLISION AVOIDANCE DEACTIVATED--");
}