/*
*	CascadeEffect.h
*	Game-specific functions and routines for the 2014-15 FTC game.
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
*	March 1 2015
*	Version 2.1
*/

#pragma once

#include "AutoFunctions.h"
//#include "CollisionAvoidance.h"
#include "Menu.h"
#include "../../Robot.h"

/*
*	FUNCTION PROTOTYPES
*/
char findGoalOrientation();
void dropBall(long height);
void kickstand();
void grabTube();
void irAlign();

// Macros for the different positions of the center goal
// The number indicates the average reading for the IR sensors when the goal is at that position
#define positionA 80
#define positionB 35
#define positionC 17

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

	// Read the average of the IR seeker
	int avg = getIRStrength();

	// Find the difference between the average IR signal and the known values for each position
	int diffA = abs(avg - positionA);
	int diffB = abs(avg - positionB);
	int diffC = abs(avg - positionC);

	// The lowest difference is the position the center goal is facing
	char facing;	// Create a variable to store the decision
	if(diffA < diffB && diffA < diffC)		// If the difference in position a is lowest, the tower must be in position a
		facing = CENTGOAL_POSITION_A;
	else if(diffB < diffA && diffB < diffC)	// If the difference in position b is lowest, the tower must be in position b
		facing = CENTGOAL_POSITION_B;
	else									// If the tower isn't in position a or b, default to position c
		facing = CENTGOAL_POSITION_C;

	// Print the IR reading, the differences, and the answer to the debug stream
	writeDebugStreamLine("\tIR:\t%d", avg);
	writeDebugStreamLine("\tdiifA:\t%d", diffA);
	writeDebugStreamLine("\tdiffB:\t%d", diffB);
	writeDebugStreamLine("\tdiffC:\t%d", diffC);
	writeDebugStreamLine("\tThe thing is in position %c", facing);

	// // Print out that this function is running
	// writeDebugStreamLine("-- CALCULATING CENTER GOAL ORIENTATION --");
	//
	// // Calculate the percent difference between the current reading and the reading taken
	// // before the match started
	// float percentDiff = 1.0 - (float) (getIRStrength(infraRed)/prematchIRreading);
	//
	// // Print out the percent difference
	// writeDebugStreamLine("\tPercent diff since initialization: %2.8f", percentDiff);
	//
	// char facing;
	//
	// // If the percent difference between the inital reading and current reading is very small,
	// // assume that the center structure has not moved
	// if(percentDiff < 0.1)
	// {
	// 	facing = centerStartPos;
	// }

	// Return the direction that the center goal is facing
	return facing;
}

/*
*	dropBall
*	Dropping the balls into the tubes
*/
void dropBall(long height)
{
	//lift hpper,
	nMotorEncoder[mLift] = 0;
	moveMotorTo(mLift, height, liftMotorSpeed);
	//then trap door has to drop,
	servo[rTrapDoor]=trapDoorOpenPosition;
	//then wait,
	wait10Msec (300);
	//put the trap door back up
	servo[rTrapDoor]=trapDoorClosedPosition;
	// put the lift down.
	moveMotorTo(mLift, liftTargetBase, liftMotorSpeed);
}

void grabTube()
{
	// put the grabber down,
	turnDegrees(18, 50);
	//moveMotorTo(mTip, nMotorEncoder[mTip]+tipTargetFloor, tipMotorSpeed);
	// Open the grabber
	moveMotorTo(mGrab, grabOpenPosition, 100);
	wait10Msec(50);
	//then move forward a little,
	goTicks(inchesToTicks(-15), 50);
	//then t-rex hand have to go down.
	moveMotorTo(mGrab, grabClosedPosition, 100);
	//goTicks(inchesToTicks(5), 50);
}

/*
*	kickstand
*	Knock over the kickstand
*/
void kickstand()
{

}

/*
*	irAlign
*	Use the secondary infrared sensor to align the robot with the center structure
*/
void irAlign()
{
	// Store the previously recorded IR value
	int prevIRvalue;

	// Set the drive motors to -50
	driveMotorsTo(-50);

	// For as long as the current IR reading is less than the previously recorded reading
	// The idea is that we keep moving forward until the IR reading starts to decrease
	do
	{
		prevIRvalue = getIRStrength(infraRedSide);
		wait10Msec(10);
	}
	while(getIRStrength(infraRedSide) > prevIRvalue);

	// Stop the drive motors
	driveMotorsTo(0);

	// Turn 90 degrees to face the center structure
	turnDegrees(90, 50);
}
