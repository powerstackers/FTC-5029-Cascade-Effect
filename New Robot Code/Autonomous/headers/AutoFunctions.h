#pragma config(StandardModel, "PS CASCADE EFFECT")
#pragma once

/*
*	AutoFunctions.h
*	Functions and subroutines for use during the autonomous period.
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
*	December 31 2014
*	Version 0.1
*/

#include "Sensors.h"
#include "../../drivers/JoystickDriver.c"
#include "GyroPath.h"


/*
*
*	FUNCTION PROTOTYPES
*
*/

// Function prototypes
void 	allMotorsTo(int i);
void 	driveMotorsTo(int i);
long 	inchesToTicks(float inches);
float 	ticksToInches(long ticks);
void	goTicks(long ticks, int speed);
void 	turnDegrees(float degrees, int speed);
void	wallAlign(bool forwardBackward);


/*
*	GLOBAL CONSTANTS
*/
#define ticksPerRevolution 	1120	// Number of encoder ticks per motor rotation
#define wheelDiameter 		4		// Diameter of your wheels in inches
#define driveGearMultiplier 2.0		// Drive gear multiplier.
									// EXAMPLE: If your drive train is geared 2:1 (1 motor rotation = 2 wheel rotations), set this to 2

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
	// Given a distance in inches, calculate the equivalent distance in motor encoder ticks.
	// We calculate this by taking the number of wheel rotations (inches/(PI*wheelDiameter)) multiplied
	// by the inverse of the gear ratio, to get the number of motor rotations. Multiply one more time
	// by the number of motor encoder ticks per one motor revolution.

	// Optional debug information
	/*writeDebugStreamLine("Drive gear multiplier: %f", 1/driveGearMultiplier);
	writeDebugStreamLine("Ticks per revolution: %d", ticksPerRevolution);
	writeDebugStreamLine("Inches to travel: %f", (inches/(PI*wheelDiameter)));*/
	return (1/driveGearMultiplier)*ticksPerRevolution*(inches/(PI*wheelDiameter));
}

/*
*	ticksToInches
*	Convert a number of ticks to a distance in inches
*/
float ticksToInches(long ticks)
{
	// Given a number of encoder ticks, calculate the equivalent distance in inches.
	// We calculate this by taking the number of ticks traveled, divided by the number of ticks per revolution,
	// and then multiplied by the gear ratio multiplier to get the number of wheel rotations. Multiply one more
	// time by the circumference of the wheels (PI*wheelDiameter).
	return (ticks/ticksPerRevolution)*driveGearMultiplier*(PI*wheelDiameter);
}

/*
*	goTicks
*	Move the robot a discance in ticks
*/
void goTicks(long ticks, int speed/*, bool collisionAvoidance*/)
{
	// Target encoder values for the left and right motors
	long targetRight = nMotorEncoder[mDriveRight] + ticks;
	long targetLeft = nMotorEncoder[mDriveLeft] + ticks;

	// Print what we are going to do to the debug stream
	writeDebugStreamLine("-- GOING TICKS --\n\tMoving %d ticks (%3.2f inches) %s at %d speed",
		ticks, ticksToInches(ticks), ((ticks>0)?"forward":"backward"), speed);

	// If we are going forwards or backwards
	// A positive number of ticks to travel indicates we are moving fowards.
	// A negative value indicates we are moving backwards.
	if(ticks > 0)
	{
		// Set the drive motors to the given speed
		driveMotorsTo(speed);
		// While this function runs, keep the robot on a constant heading
		//StartTask(stablizePath);
		// Wait until both motors have reached the target
		while(nMotorEncoder[mDriveLeft] < targetLeft && nMotorEncoder[mDriveRight] < targetRight)
		{
			//writeDebugStreamLine("Curr: %d\tTarg: %d", nMotorEncoder[mDriveLeft], targetLeft);
		}
		// Now that we've reached our destination, turn off the stablization
		StopTask(stablizePath);
		// Stop the drive motors here
		driveMotorsTo(0);
	}
	else if(ticks < 0)
	{
		// Set the drive motors to the speed (in reverse)
		driveMotorsTo(-1 * speed);
		// While this function runs, keep the robot on a constant heading
		//StartTask(stablizePath);
		// Wait until both motors have reached the target
		while(nMotorEncoder[mDriveLeft] > targetLeft && nMotorEncoder[mDriveRight] > targetRight)
		{
			//writeDebugStreamLine("Curr: %d\tTarg: %d", nMotorEncoder[mDriveLeft], targetLeft);
		}
		// Now that we've reached our destination, turn off the stablization
		StopTask(stablizePath);
		// Turn off the drive motors here
		driveMotorsTo(0);
	}

	// Write to the debug stream that we are done
	writeDebugStreamLine("-- MOVING DONE --");
}

/*
*	turnDegrees
*	Turn the robot a certain number of degrees
*/
void turnDegrees(float degrees, int speed)
{
	// Notify the drivers of what we are about to do
	string dummy;
	StringFormat(dummy, "-- TURNING --\n\tTurning %d degrees at %d speed",
		degrees, speed);
	writeDebugStreamLine(dummy);


	// Store the number of degrees turned so far, i.e., the difference of
	// the current position and the starting position.
	float degreesSoFar = 0;

	// Take an initial reading of the gyro sensor. This compensates for any initial spin the gyro may have.
	const float initialTurnReading = currentGryoReading();

	// Decide whether to turn clockwise or counterclockwise.
	// A positive degree target inmplies turning counterclockwise. A negative target implies clockwise.
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

	writeDebugStreamLine("\tSet left motor to %d", motor[mDriveLeft]);
	writeDebugStreamLine("\tSet right motor to %d", motor[mDriveRight]);

	// For as long as the current degree measure doesn't equal the target. This will work in the clockwise and
	// counterclockwise directions, since we are comparing the absolute values.
	while(abs(degreesSoFar) < abs(degrees))
	{
		// 10 millisecond interval
		wait10Msec(1);

		// Calculate the gyro's angular velocity reading.
		// The reading is given as the current sensor value, minus any initial spin that the gyro may have had.
		float reading = currentGryoReading() - initialTurnReading;

		// Gyro sensor returns an angular speed. To calculate the distance, we multiply the rate by the time interval (.01 seconds).
		degreesSoFar += reading * 0.01;

	}

	// Stop all drive motors
	driveMotorsTo(0);

	// Notify the drivers that we are done.
	writeDebugStreamLine("\tTurning done\n\tTotal degrees turned: %f", degreesSoFar);
}


#define ALIGN_FORWARD 	true
#define ALIGN_BACKWARD 	false
/*
*	wallAlign
*	Use a wall or other rigid surface to align the robot.
*/
void wallAlign(bool forwardBackward)
{
	writeDebugStreamLine("-- ALIGNING ROBOT --\n\tMoving %s", forwardBackward? "forward":"backward");

	// If we are going to move forward into the wall
	if(forwardBackward==ALIGN_FORWARD)
	{
		driveMotorsTo(50);
	}
	// If we are going to move backward into the wall
	else
	{
		driveMotorsTo(-50);
	}

	// Store whether the left and right sides are finished aligning
	bool rightDone = false;
	bool leftDone = false;

	// Store the previously read value of the left and right encoders, and initialize them to the current positions
	long leftPrevValue = nMotorEncoder[mDriveLeft];
	long rightPrevValue = nMotorEncoder[mDriveRight];

	// This is our threshold. A change less than this will indicate that the motor has met resistance
	const unsigned long stopThreshold = 300;

	while(!rightDone&&!leftDone)
	{
		// Wait for 1 second to give the motors time to move, if they're going to
		wait10Msec(100);

		// If the left motor hasn't changed an acceptable amount in the last second, then it has met resistance
		if(abs(leftPrevValue - nMotorEncoder[mDriveLeft]) < stopThreshold)
		{
			// Turn the motor off, and indicate that this side is aligned
			motor[mDriveLeft] = 0;
			leftDone = true;
			writeDebugStreamLine("\tLeft side aligned. Diff: %d", abs(leftPrevValue - nMotorEncoder[mDriveLeft]));
			PlaySound(soundBeepBeep);
			if(!rightDone)
				motor[mDriveRight] = 100;
		}
		// If the right motor hasn't changed an acceptable amount in the last second, then it has met resistance
		if(abs(rightPrevValue - nMotorEncoder[mDriveRight]) < stopThreshold)
		{
			// Turn the motor off, and indicate that this side is aligned
			motor[mDriveRight] = 0;
			rightDone = true;
			writeDebugStreamLine("\tRight side aligned. Diff: %d", abs(rightPrevValue - nMotorEncoder[mDriveRight]));
			PlaySound(soundBeepBeep);
			if(!leftDone)
				motor[mDriveLeft] = 100;
		}

		leftPrevValue = nMotorEncoder[mDriveLeft];
		rightPrevValue = nMotorEncoder[mDriveRight];

	}

	writeDebugStreamLine("-- ALIGNMENT DONE --");
}
