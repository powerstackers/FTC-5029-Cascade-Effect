/*
*	Robot.h
*	General code and constants that are used by all programs, specific to our robot.
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
*	January 20 2015
*	Version 0.1
*/

// Include guard. This file can only be included one time
#pragma once

// Include files to run various devices
#include "drivers/hitechnic-sensormux.h"

/*
*	FUNCTION PROTOTYPES
*/
void printWelcomeMessage(string programName, float versionNumber);
void checkBatteryLevels();
bool tetrixBatteryGoodState();
bool nxtBatteryGoodState();

/*
*	GLOBAL CONSTANTS
*	Important constant values that are used in many places on the robot
*/

// Minimum acceptable battery levels. Levels below these values will raise an alert.
#define nxtBatteryMinimumLevel		7500
#define tetrixBatteryMinimumLevel	13000

// Motor speeds
#define liftMotorSpeed 	50			// Speed of the vertical lift motor
#define horizMotorSpeed	50			// Speed of the horizontal slide motor
#define tipMotorSpeed 	50			// Speed of the rolling goal tipping motor
#define brushMotorSpeed	65			// Speed of the brush motor

// Servo positions
#define grabberOpenPosition		255	// Rolling goal grabber open servo position
#define grabberClosedPosition	50	// Rolling goal grabber closed servo position
#define flapLeftOpenPosition	0	// Left side flap open servo position
#define flapLeftClosedPosition	1	// Left side flap closed servo position
#define flapRightOpenPosition	0	// Right side flap open servo position
#define flapRightClosedPosition	1	// Right side flap closed servo position
#define trapDoorOpenPosition	20	// Trap door open servo position
#define trapDoorClosedPosition	128	// Trap door closed servo position
#define trapDoorIdlePosition	90	// Idling position for the grabber
#define trapDoorAlignPosition	50	// Trap door precision alignment position

// Motor encoder targets
//		Lift motor
#define liftTargetBase 		0
#define liftTargetLow 		3500
#define liftTargetMed 		7500
#define liftTargetHigh 		11800
#define liftTargetCent 		15700

//		Horizontal lift motor
#define horizTargetBase 	0
#define horizTargetClose	1000
#define horizTargetMed 		2000
#define horizTargetFar 		3000

//		Tip motor
#define tipTargetBase 		0
#define tipTargetLow 		-1000
#define tipTargetMed 		-2000
#define tipTargetHigh 		-3000
#define tipTargetFloor		-5400

/*
*	printWelcomeMessage
*	Prints the name and version of the program, as well as some licensing information, to the debug stream.
*/
void printWelcomeMessage(string programName, float versionNumber)
{
	// Build a string with the progam name and version number in it.
	// We have to do it this way to be able to put more than one dynamic value on a single line
	string nameAndVersion;
	StringFormat(nameAndVersion, "\n\n%s v%1.2f", programName, versionNumber);

	// Print the message
	writeDebugStreamLine(nameAndVersion);
	writeDebugStreamLine("Copyright (C) 2015  Powerstackers");
	writeDebugStreamLine("This program comes with ABSOLUTELY NO WARRANTY.");
	writeDebugStreamLine("This is free software, and you are welcome to redistribute it under certain conditions;");
	writeDebugStreamLine("see LICENST.txt for details.\n");
}

/*
*	tetrixBatteryGoodState
*	Return whether or not the main TETRIX battery is acceptably full.
*/
bool tetrixBatteryGoodState()
{
	return externalBatteryAvg / 1000.0 > tetrixBatteryMinimumLevel;
}

/*
*	nxtBatteryGoodState
*	Return whether or not the NXT battery is acceptably full.
*/
bool nxtBatteryGoodState()
{
	return nAvgBatteryLevel / 1000.0 > nxtBatteryMinimumLevel;
}

/*
*	checkBatteryLevels
*	Check the NXT and TETRIX battery levels
*/
void checkBatteryLevels()
{
	// If battery levels are low, notify the operators
	// A battery level below 13 volts is considered low.
	if(!tetrixBatteryGoodState())
	{
		PlaySound(soundException);
		writeDebugStreamLine("--!! MAIN BATTERY LOW !!--\n\tAvg battery level: %2.2f",
			externalBatteryAvg / 1000.0);

		// A negative reading may indicate that the battery is disconnected
		if(externalBatteryAvg/1000.0 < 0.0)
			writeDebugStreamLine("\tCheck that main battery is connected.");
	}

	// If the NXT battery level is low, print a message. A level below 7.5 volts is considered low.
	if(!nxtBatteryGoodState())
	{
		PlaySound(soundException);
		writeDebugStreamLine("--!! NXT BATTERY LOW !!--\n\tAvg Batt Level: %2.2f",
			nAvgBatteryLevel / 1000.0);
	}

	// Print both battery states, good or bad, to the NXT LCD screen
	nxtDisplayTextLine(5, "MAIN BATT %s", tetrixBatteryGoodState()?"GOOD":"BAD");
	nxtDisplayTextLine(6, "NXT BATT %s", nxtBatteryGoodState()?"GOOD":"BAD");

	// Check that the multiplexer battery is in a good state
	if(HTSMUXreadPowerStatus(SMUX1))
	{	// This code will execute if the battery level is not good
		writeDebugStreamLine("--! MUX BATTERY LOW !--");
		writeDebugStreamLine("\tCheck to see that SMUX battery is turned on");
	}
	else
	{	// If the MUX battery is good, say so
		writeDebugStreamLine("\tMUX battery good");
	}
}
