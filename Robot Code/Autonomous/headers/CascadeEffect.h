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
*	January 2 2015
*	Version 0.3
*/

#include "AutoFunctions.h"
#include "CollisionAvoidance.h"

// Game functions
char findGoalOrientation();
void dropBall(int height);
void kickstand();

// Manipulator movement functions
void moveMotorTo(short affectedMotor, long position);

// General functions
void initializeRobot();

/*
*	initializeRobot
*	Move all motors and servos to their starting positions
*/
void initializeRobot()
{
	// Turn of the diagnostic display from JoystickDriver.c, and clear the NXT screen
	bDisplayDiagnostics = false;
	eraseDisplay();

	// Measure and print the battery levels
	writeDebugStreamLine("--BATTERY LEVELS--\n\tTETRIX battery level: %2.2f volts", externalBatteryAvg / 1000.0);
	writeDebugStreamLine("\tNXT Battery level: %2.2f volts", nAvgBatteryLevel / 1000.0);

	// If battery levels are low, notify the operators
	// A battery level below 13 volts is considered low.
	if(externalBatteryAvg < 13000)
	{
		PlaySound(soundException);
		writeDebugStreamLine("--!! MAIN BATTERY LOW !!--\n\tAvg battery level: %2.2f",
			externalBatteryAvg / 1000.0);

		// A negative reading may indicate that the battery is disconnected
		if(externalBatteryAvg/1000.0 < 0.0)
			writeDebugStreamLine("\tCheck that main battery is connected.");
		nxtDisplayTextLine(5, "MAIN BATT LOW");
	}
	else
		// If the battery level is acceptable, print a Battery Good message
		nxtDisplayTextLine(5, "MAIN BATT GOOD");

	// If the NXT battery level is low, print a message. A level below 7.5 volts is considered low.
	if(nAvgBatteryLevel < 7500)
	{
		PlaySound(soundException);
		writeDebugStreamLine("--!! NXT BATTERY LOW !!--\n\tAvg Batt Level: %2.2f",
			nAvgBatteryLevel / 1000.0);
		nxtDisplayTextLine(6, "NXT BATT LOW");
	}
	else
		// If the battery level is acceptable, print a Battery Good message
		nxtDisplayTextLine(6, "NXT BATT GOOD");

	if(HTSMUXreadPowerStatus(SMUX1))
	{
			writeDebugStreamLine("--! MUX BATTERY LOW !--");
			writeDebugStreamLine("\tCheck to see that SMUX battery is turned on");
	}

	// Put all motors and servos into their starting positions
	allMotorsTo(0);

	// When initialization is done, notify the drivers
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
	writeDebugStreamLine("\tdiifA:", diffA);
	writeDebugStreamLine("\tdiffB:", diffB);
	writeDebugStreamLine("\tdiffC:", diffC);
	writeDebugStreamLine("\tThe thing is in position %c", facing);

	// Return the direction that the center goal is facing
	return facing;
}

/*
*	dropBall
*	Dropping the balls into the tubes
*/
void dropBall(long height)
{
	// put the grabber down,
	moveMotorTo(mTip, height);
//then move forward a little,
goTicks(inchesToTicks(6), 30);
//then t-rex hand have to go down.
servo[rGrabber]=grabberClosedPosition;
//lift hpper,
moveMotorTo(mLift, position);
//then trap door has to drop,
servo[rTrapDoor]=trapDoorOpenPosition;
//then wait,
wait10Msec (300);
//put the trap door back up
servo[rTrapDoor]=trapDoorClosedPosition;
// put the lift down.
moveMotorTo(mTip, position);
//
}

/*
*	kickstand
*	Knock over the kickstand
*/
void kickstand()
{

}

void moveMotorTo(short affectedMotor, long position)
{
	if(nMotorEncoder[affectedMotor]=posititon)
	{
		return;
	}
	else
	{
		if(position>nMotorEncoder[affectedMotor])
		{
			motor[affectedMotor]= tipMotorSpeed;
			while(position>nMotorEncoder[affectedMotor]){}
		} else {
			motor[affectedMotor]= -tipMotorSpeed;
			while(postition<nMotorEncoder[affectedMotor]){}
		}
		motor[affectedMotor]=0;
	}
}
