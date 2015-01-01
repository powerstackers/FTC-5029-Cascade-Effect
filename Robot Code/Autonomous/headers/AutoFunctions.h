#pragma config(StandardModel, "PS CASCADE EFFECT")
#pragma once

/*
*	AutoFunctions.h
*	Functions and subroutines for use during the autonomous period.
*	Copyright (C) 2015 Jonathan Thomas
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
#include "JoystickDriver.c"


/*
*
*	FUNCTION PROTOTYPES
*
*/

// Utility functions
void allMotorsTo(int i);
void driveMotorsTo(int i);
long inchesToTicks(float inches);
float ticksToInches(long ticks);
void goTicks(long ticks, int speed/*, bool collisionAvoidance*/);
void turnDegrees(float degrees, int speed);


/*
*	GLOBAL CONSTANTS
*/
// Each time the motors turn once, they turn 1120 ticks
#define ticksPerRevolution 1120


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
void goTicks(long ticks, int speed/*, bool collisionAvoidance*/)
{
	// Target encoder values for the left and right motors
	long targetRight = nMotorEncoder[mDriveRight] + ticks;
	long targetLeft = nMotorEncoder[mDriveLeft] + ticks;

	// Print what we are going to do to the debug stream
	writeDebugStreamLine("-- GOING TICKS --\n\tMoving %d ticks %s at %d speed",
		ticks, ((ticks>0)?"forward":"backward"), speed);

	// Turn on collision avoidance, if it's asked for
	/*if(collisionAvoidance&&!avoidanceActive)
		StartTask(avoidCollision);
		*/

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
	/*if(collisionAvoidance&&avoidanceActive)
		avoidanceActive = false;
		*/
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

	// Take an initial reading of the gyro sensor
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

	// Stop all drive motors
	driveMotorsTo(0);

	writeDebugStreamLine("\tTurning done");
}
