
// Include guard. This file can only be included one time
#pragma once

/*
*	FUNCTION PROTOTYPES
*/
void printWelcomeMessage(string programName, float versionNumber);


/*
*	GLOBAL CONSTANTS
*	Important constant values that are used in many places on the robot
*/

// Motor speeds
#define liftMotorSpeed 	50			// Speed of the vertical lift motor
#define horizMotorSpeed	50			// Speed of the horizontal slide motor
#define tipMotorSpeed 	50			// Speed of the rolling goal tipping motor
#define brushMotorSpeed	50			// Speed of the brush motor

// Servo positions
#define grabberOpenPosition		255	// Rolling goal grabber open servo position
#define grabberClosedPosition	110	// Rolling goal grabber closed servo position
#define flapLeftOpenPosition	0	// Left side flap open servo position
#define flapLeftClosedPosition	1	// Left side flap closed servo position
#define flapRightOpenPosition	0	// Right side flap open servo position
#define flapRightClosedPosition	1	// Right side flap closed servo position
#define trapDoorOpenPosition	0	// Trap door open servo position
#define trapDoorClosedPosition	90	// Trap door closed servo position

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
