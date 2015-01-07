#pragma config(StandardModel, "PS CASCADE EFFECT")

/*
*	CollisionAvoidance.h
*	Functions and tasks to avoid on-field collisions.
*	Copyright (C) 2015 Powerstackers
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	FTC Team #5029, The Powerstackers
*	powerstackersftc.com
*	github.com/powerstackers
*	January 2 2015
*	Version 0.2
*/

// Include files to handle the sensors, and for basic utility functions
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
void detour();

/*
*	detour
*	Use the gyro sensor and encoders to clear an obstruction.
*/
void detour(){
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

	/*
	*	FIRST MOTION
	*/
	// Store the encoder value before moving perpendicular clear of the obstruction
	long encoderBeforeTurn1 = nMotorEncoder[mDriveLeft];

	// While the back sensor detects an obstruction, keep moving
	while(getUltraStrength(ultraBack)<ultrasonicThreshold)
		driveMotorsTo(25);
	driveMotorsTo(0);

	// Determine the number of ticks it took to travel past the obstruction
	long ticksTraveledPerpendicular = nMotorEncoder[mDriveLeft] - encoderBeforeTurn1;

	// Turn 90 degrees, now parallel to the original path, but moved over past the obstruction
	turnDegrees(-90.0, 30);

	/*
	*	SECOND MOTION
	*/
	// Store the encoder value before moving parallel to the obstacle. This value will be added to the starting value
	// to get the robot's navigation back on track.
	long encoderBeforeParallel = nMotorEncoder[mDriveLeft];

	// Moving past the obstruction. While the sensor detects an object, keep going.
	while(getUltraStrength(ultraBack)<ultrasonicThreshold)
		driveMotorsTo(25);
	driveMotorsTo(0);

	// Calculate the number of encoder ticks it took to move parallel past the obstacle
	long ticksParallel = nMotorEncoder[mDriveLeft] - encoderBeforeParallel;

	// Turn back 90 degrees, now facing the wall and the obstruction
	turnDegrees(-90.0, 30);

	/*
	*	THIRD MOTION
	*/
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
bool avoidanceActive = false;	// Simple on/off switch for the task
task avoidCollision()
{
	// Set the switch to the ON position
	avoidanceActive = true;
	
	// Tell the drivers that the collision avoidance system is activated
	writeDebugStreamLine("-- COLLISION AVOIDANCE ACTIVATED --");
	
	// Loop until the switch is thrown
	while(avoidanceActive)
	{
		// If the ultrasonic sensors detect an obstruction
		if(getUltraStrength(ultraFront))
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

	// Notify the drivers when the collision avoiance system is deactivated
	writeDebugStreamLine("-- COLLISION AVOIDANCE DEACTIVATED --");
}
