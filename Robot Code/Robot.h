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
*	March 6 2015
*	Version 2.5
*/

// Include guard. This file can only be included one time
#pragma once

// Include files to run various devices
#include "drivers/hitechnic-sensormux.h"
#include "Autonomous/headers/Sensors.h"

/*
*	FUNCTION PROTOTYPES
*/
void printWelcomeMessage(string programName, float versionNumber);
void checkBatteryLevels();
bool tetrixBatteryGoodState();
bool nxtBatteryGoodState();
bool muxBatteryGoodState();
void initializeRobot();
void moveMotorTo(short affectedMotor, long position, short speed);

/*
*	GLOBAL CONSTANTS
*	Important constant values that are used in many places on the robot
*/

// Minimum acceptable battery levels. Levels below these values will raise an alert.
#define nxtBatteryMinimumLevel		7500
#define tetrixBatteryMinimumLevel	13000

// Motor speeds
#define liftMotorSpeed 	100			// Speed of the vertical lift motor
#define liftMotorSpeedDown	-5		// Speed of the vertical lift motor when moving down
#define brushMotorSpeed	-100			// Speed of the brush motor
#define grabMotorSpeed	100			// Speed of the grab motor
#define preferredTurnSpeed	50		// Most desirable turning speed

// Servo positions
#define flapLeftOpenPosition	1	// Left side flap open servo position
#define flapLeftClosedPosition	256	// Left side flap closed servo position

#define flapRightOpenPosition	230	// Right side flap open servo position
#define flapRightClosedPosition	1	// Right side flap closed servo position

#define grabOpenPosition		-180// Grabber open position
#define grabClosedPosition		-15	// Grabber closed position

#define trapDoorOpenPosition	196	// Trap door open servo position
#define trapDoorClosedPosition	82	// Trap door closed servo position
#define trapDoorIdlePosition	85	// Idling position for the grabber
#define trapDoorAlignPosition	143	// Trap door precision alignment position

#define trapDoorChangeRate		30 	// Trap door servo change rate
									// Given in positions per update (20 ms, 50 updates per second)
#define ballDoorOpenPosition	85	// Ball door open servo position
#define ballDoorClosedPosition	160	// Ball door closed servo position

// Motor encoder targets
//		Lift motor
#define liftTargetBase 		0
#define liftTargetLow 		1400
#define liftTargetMed 		2330
#define liftTargetHigh 		3320
#define liftTargetCent 		4400

// Reading on the IR sensor before the center structure is put in its position
int prematchIRreading;

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
*	muxBatteryGoodState
*	Return whether or not the multiplexer battery is acceptably full
*/
bool muxBatteryGoodState()
{
	return HTSMUXreadPowerStatus(SMUX1);
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
	else
	{	// If the main battery is low, say so
		writeDebugStreamLine("\tMain battery good (%2.2f volts)", externalBatteryAvg / 1000.0);
	}

	// If the NXT battery level is low, print a message. A level below 7.5 volts is considered low.
	if(!nxtBatteryGoodState())
	{
		PlaySound(soundException);
		writeDebugStreamLine("--!! NXT BATTERY LOW !!--\n\tAvg Batt Level: %2.2f",
			nAvgBatteryLevel / 1000.0);
	}
	else
	{	// If the nxt battery is good, say so
		writeDebugStreamLine("\tNXT battery good (%2.2f volts)", nAvgBatteryLevel / 1000.0);
	}


	// Check that the multiplexer battery is in a good state
	if(muxBatteryGoodState())
	{	// This code will execute if the battery level is not good
		writeDebugStreamLine("--! MUX BATTERY LOW !--");
		writeDebugStreamLine("\tCheck to see that SMUX battery is turned on");
	}
	else
	{	// If the MUX battery is good, say so
		writeDebugStreamLine("\tMUX battery good");
	}


	// Print both battery states, good or bad, to the NXT LCD screen
	nxtDisplayTextLine(5, "MAIN BATT %s", tetrixBatteryGoodState()?"GOOD":"BAD");
	nxtDisplayTextLine(6, "NXT BATT %s", nxtBatteryGoodState()?"GOOD":"BAD");
	nxtDisplayTextLine(7, "MUX BATT %s", muxBatteryGoodState()?"GOOD":"BAD");
}

void initializeRobot()
{
	// Turn of the diagnostic display from JoystickDriver.c, and clear the NXT screen
	bDisplayDiagnostics = false;
	eraseDisplay();

	// Print out a message sayint that the initialization has started
	writeDebugStreamLine("-- INITIALIZING --");

	// Make sure that the batteries are at acceptable levels
	checkBatteryLevels();

	// Print what we're doing to the nxt screen
	nxtDisplayTextLine(0, "2015 Powerstackers");
	nxtDisplayCenteredBigTextLine(1, "INIT");
	nxtDisplayCenteredBigTextLine(3, "ROBOT");

	/*
	*	MOTOR INITIALIZATION
	*	This section ensures all motors are turned off, and that the grabber is initialized
	*/
	// DC motors should be set to OFF
	motor[mDriveLeft] 	= 0;
	motor[mDriveRight] 	= 0;
	motor[mLift]		= 0;
	motor[mBrush] 		= 0;

	// All encoder positions that we use start at zero
	nMotorEncoder[mLift] 	= 0;

	// Move the grab motor down until it hits the cookie cutter, then set its encoder position to 0
	// This zeroes the grabber encoder
	motor[mGrab] = 50;
	wait10Msec(100);
	motor[mGrab] = 0;
	nMotorEncoder[mGrab] = 0;

	// Move the grabber motor to its open position
	moveMotorTo(mGrab, grabOpenPosition, grabMotorSpeed);

	/*
	*	SERVO INITIALIZATION
	*	This section sets all servos to their starting positions, and also changes some of the servo settings.
	*/
	// Servos should be set to the closed position
	servo[rFlapLeft] 	= flapLeftClosedPosition;
	servo[rFlapRight] 	= flapRightClosedPosition;
	servo[rTrapDoor] 	= trapDoorClosedPosition;
	servo[rBallDoor]	= ballDoorClosedPosition;

	// Set the servo speed of some servos. This makes the servo change positions slower.
	// The number of the setting indicates the number of positions the servo moves every 20 milliseconds
	servoChangeRate[rTrapDoor] = trapDoorChangeRate;

	// Make it so that servos maintain their positions after the program ends
	bSystemLeaveServosEnabledOnProgramStop = true;

	// Take an initial reading of the IR sensor
	prematchIRreading = getIRStrength(infraRed);

	// Initialization done, print to the debug stream
	writeDebugStreamLine("-- ROBOT INITIALIZED --");
}

/*
*	moveMotor
*	Move a motor to a given encoder position
*/
void moveMotorTo(short affectedMotor, long position, short speed)
{
	writeDebugStreamLine("-- MOVING MOTOR TO POSITION --\n\tCurrent position: %d", nMotorEncoder[affectedMotor]);
	writeDebugStreamLine("\tTarget position: %d", position);
	// If the motor is already at the target position, don't change it
	if(nMotorEncoder[affectedMotor]==position)
	{
		return;
	}
	else
	{
		// If the motor is below the target position, move up
		if(position>nMotorEncoder[affectedMotor])
		{
			motor[affectedMotor]= speed;
			while(position>nMotorEncoder[affectedMotor]){}
		}
		// If the motor is above the target position, move down
		else
		{
			motor[affectedMotor]= -speed;
			while(position<nMotorEncoder[affectedMotor]){}
		}

		// Turn off the motor
		motor[affectedMotor]=0;
	}

	writeDebugStreamLine("-- MOTOR MOVED --");
}

void switchTrapdoor()
{
	// If the trapdoor is in the open position, move it to the closed position
	if(servo[rTrapDoor]==trapDoorOpenPosition)
	{
		servo[rTrapDoor] = trapDoorClosedPosition;
	}

	// If the trapdoor is in the align position (second lowest), move it to the open position (lowest)
	else if(servo[rTrapDoor]==trapDoorAlignPosition)
	{
		servo[rTrapDoor] = trapDoorOpenPosition;
	}

	// If the trapdoor is in the closed position, move it to the align position
	else if(servo[rTrapDoor]==trapDoorClosedPosition)
	{
		servo[rTrapDoor] = trapDoorAlignPosition;
	}


	writeDebugStreamLine("Toggled trapdoor to %s position", (servo[rTrapDoor]==trapDoorOpenPosition)?"open":"closed");

}
