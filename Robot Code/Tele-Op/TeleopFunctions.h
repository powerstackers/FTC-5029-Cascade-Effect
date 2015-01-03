/*
*	TeleopFunctions.h
*	Functions and subroutines for use during the teleop period.
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
*	January 3 2015
*	Version 0.2
*/

/*
*	FUNCTION PROTOTYPES
*/

void initializeRobot();
void getCustomJoystickSettings ();

// Utility functions
void printInfoToScreen();
short stickToMotorValue(short stickValue);
void switchEncoderTarget(unsigned long* encoderTarget, char* currentPosition, char upOrDown);
task checkButtons();

/*
*	JOYSTICK ASSIGNMENTS
*	Each function in our program that is user-controlled is hinged on a variable or set of variables.
*	For example, the vertical lift has a button for up and a button for down. Instead of looking at
*	the button variable from JoystickDriver.c, the main program has its own unique variable that it
*	uses in place of the button. This allows us to change which button is assigned to which operation
*	without having to rewrite the whole program every time.
*	For our purposes, a stick is represented by a short integer containing its value, and a button is
*	represented by a boolean representing "pressed" or "not pressed."
*/

short 	stickValueLeftForward;	// Left drive control stick (forward)
short 	stickValueRightForward;	// Right drive control stick (forward)
short 	stickValueLeftBackward;	// Left drive control (backwards)
short 	stickValueRightBackward;// Right drive control (backwards)

bool 	buttonStraightDrive;	// Straight drive mode button
bool 	buttonBackwardsDrive;	// Backwards drive mode button

bool 	buttonBrush;			// Ball collection brush button
bool 	buttonLiftUp;			// Vertical lift UP button
bool 	buttonLiftDown;			// Vertical lift DOWN button
bool	buttonTrapDoor;			// Ball bucket door toggle
bool	buttonDeflectorToggle;	// Ball deflector toggle

bool 	buttonLiftOut;			// Horizontal lift OUT button
bool 	buttonLiftIn;			// Horizontal lift IN button
bool 	buttonGrabToggle;		// Rolling goal grabber toggle button
bool 	buttonTipUp;			// Goal tipper UP button
bool 	buttonTipDown;			// Goal tipper DOWN button

bool	buttonFlaps;			// Flaps button

/*
*	ENCODER TARGETS
*	The vertical lift, horizontal lift, and the tipping mechanism all have specific positions to which
*	they are set. The main program will monitor their position, and try to keep it as close to their
*	target as possible. The targets are switched in functions below, based on button input.
*	The different motors all have a starting position, and then a low, medium, and high goal positions.
*	The vertical lift also has a fifth position for the center goal tube.
*/

// Macros to store the different available positions
#define liftTargetBase 		1	// Vertical lift targets
#define liftTargetLow 		2
#define liftTargetMed 		3
#define liftTargetHigh 		4
#define liftTargetCent 		5

#define horizTargetBase 	1	// Horizontal lift targets
#define horizTargetClose	2
#define horizTargetMed 		3
#define horizTargetFar 		4

#define tipTargetBase 		1	// Tipper targets
#define tipTargetLow 		2
#define tipTargetMed 		3
#define tipTargetHigh 		4

// Long integers store the current target
long liftEncoderTarget 	= 0;
long horizEncoderTarget = 0;
long tipEncoderTarget 	= 0;

/*
*	getCustomJoystickSettings
*	Translate the joystick values to reusable variables.
*	Storing the joystick button values in our own variables makes them easier to use. We can reassign
*	buttons by changing this function, rather than having to rewrite large portions of code. It also
*	makes buttons easier to use, since they have a simple on/off indicator.
*/
void getCustomJoystickSettings ()
{
	// Player 1
	stickValueRightForward 	= joystick.joy1_y2;				// Driver 1 right stick
	stickValueLeftForward 	= joystick.joy1_y1;				// Driver 1 left stick
	stickValueRightBackward = -1 * joystick.joy1_y1;		// Driver 1 left stick
	stickValueLeftBackward 	= -1 * joystick.joy1_y2;		// Driver 1 right stick

	buttonStraightDrive 	= (joy1Btn(3) == 1);			// Driver 1 red button
	buttonBackwardsDrive 	= (joy1Btn(5) == 1);			// Driver 1 left shoulder
	buttonBrush 			= (joy1Btn(6) == 1);			// Driver 1 right shoulder

	// Player 2
	buttonLiftUp 			= (joy2Btn(6) == 1);			// Driver 2 right shoulder
	buttonLiftDown 			= (joy2Btn(8) == 1);			// Driver 2 right trigger
	buttonLiftOut 			= (joy2Btn(5) == 1);			// Driver 2 left shoulder
	buttonLiftIn 			= (joy2Btn(7) == 1);			// Driver 2 left trigger
	buttonGrabToggle		= (joy2Btn(1) == 1);			// Driver 2 blue button
	buttonTipUp				= (joystick.joy2_TopHat == 0);	// Driver 2 top hat up
	buttonTipDown 			= (joystick.joy2_TopHat == 4);	// Driver 2 top hat down
	buttonTrapDoor			= (joy2Btn(2) == 1);			// Driver 2 green button
	buttonFlaps				= (joy2Btn(4) == 1);			// Driver 2 yellow button
	buttonDeflectorToggle	= (joy2Btn(3) == 1);			// Driver 2 red button
}

/*
*	printInfoToScreen
*	Print information to the NXT LCD screen
*/
void printInfoToScreen()
{
	nxtDisplayTextLine(1, "LeftDr:%d", motor[mDriveLeft]);			// Left drive motor settings
	nxtDisplayTextLine(2, "RightDr:%d", motor[mDriveRight]);		// Right drive motor settings
	nxtDisplayTextLine(3, "LeftSt:%d", stickValueLeftForward);		// Left joystick
	nxtDisplayTextLine(4, "RightSt:%d", stickValueRightForward);	// Right joystick
	nxtDisplayTextLine(5, "p2TopHat:%d", joystick.joy2_TopHat);		// Player two arrow keys
	nxtDisplayTextLine(6, "LB:%d", stickValueLeftBackward);			// Left backward stick
}

/*
* 	stickToMotorValue
* 	Convert the -127 - 128 stick value to a -100 - 100 motor value
*/
short stickToMotorValue(short stickValue)
{
	return (short) ( (float)stickValue * 0.78125);
}

/*
*	initializeRobot
*	Sets the robot's motors and servos to their starting values. Also
*	prepares sensors and multiplexers, as well as the NXT.
*/
void initializeRobot()
{
	// Turn off diagnostics from JoystickDriver.c and clear the screen
	bDisplayDiagnostics = false;
	eraseDisplay();

	// Measure and print the battery levels
	writeDebugStreamLine("\tChecking battery levels...\n\tTETRIX battery level: %2.2f volts\n\tNXT battery level: %2.2f volts",
		externalBatteryAvg / 1000.0, nAvgBatteryLevel / 1000.0);	// Battery levels are given to us in kilovolts,

	// If the battery level is low, notify the drivers
	if(externalBatteryAvg < 13000){		// Battery level below 13 volts is considered low
		PlaySound(soundException);		// Beep from the NXT speaker
		// Print Low Battery messages to the debug stream and the NXT LCD screen
		writeDebugStreamLine("--!! MAIN BATTERY LOW !!--\n\t Avg Batt Level: %2.2f volts",
			externalBatteryAvg / 1000.0);
		nxtDisplayTextLine(4, "MAIN BATT LOW");

		// If the battery level reads negative, the battery could be disconnected.
		if(externalBatteryAvg<0.0){
			writeDebugStreamLine("\tCheck that battery is connected.");
		}
	}
	else{
		// If the battery level is acceptable, print to the NXT LCD screen
		nxtDisplayTextLine(4, "MAIN BATT GOOD");
	}

	// NXT Low Battery message
	if(nAvgBatteryLevel < 7500){	// Battery level below 7.5 volts is considered low
		PlaySound(soundException);	// Beep from the NXT speaker
		// Print Low Battery messages to the debug stream and the NXT LCD screen
		writeDebugStreamLine("--!! NXT BATTERY LOW !!--\n\tAvg Batt Level: %2.2f",
			nAvgBatteryLevel / 1000.0);
		nxtDisplayTextLine(5, "NXT BATT LOW");
	}
	else{
		// If the battery level is acceptable, print to the NXT LCD screen
		nxtDisplayTextLine(5, "NXT BATT GOOD");
	}

	// Set all motors and servos to their starting positions
	// All motors should be set to 0 power
	motor[mDriveLeft] 	= 0;
	motor[mDriveRight] 	= 0;
	motor[mBrush] 		= 0;
	motor[mLift] 		= 0;

	// Initialization done, print to the debug stream
	writeDebugStreamLine("-- ROBOT INITIALIZED --");
}


/*
*	switchEncoderTarget
*	Move an encoder target up or down one position
*	Inputs: encoder target variable to switch; the current position of the motor; wether to move up or down one position.
*	The tipper, vertical lift, and horizontal lift all have four positions: base, low, medium, and high. The vertical lift
*	also has an additional "center goal" position.
*/
void switchEncoderTarget(unsigned long* encoderTarget, char* currentPosition, char upOrDown)
{
	// Check the current position, and move it one up or one down in the list.
	switch(*currentPosition)
	{
		// Target currently at the "center goal" position
		// Only the vertical lift can move to this position
		case 'c':
			// If switching the target to a lower position
			if(upOrDown != 'u')
			{
				*currentPosition = 'h';

				// Determine which encoder target we are switching, and change the appropriate variable
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetHigh :
					((encoderTarget == &horizEncoderTarget)?horizTargetFar : tipTargetHigh);

				writeDebugStreamLine("Switched encoder target to HIGH");
			}
			// If switching the target to a higher position, don't change the target
			// The target cannot be set higher than this position
			else
			{
				writeDebugStreamLine("Encoder target cannot be set any higher");
			}
			break;

		// Target currently at the "high" position
		case 'h':
			// If moving the target up one position
			if(upOrDown == 'u')
			{
				// Only move the target up from this position if we are switching the vertical lift
				if(encoderTarget == &liftEncoderTarget)
				{
					*currentPosition = 'c';				// Set the current position variable
					*encoderTarget = liftTargetCent;	// Set the encoder target
					writeDebugStreamLine("Encoder target set to CENTER");
				}
				else
					// If the encoder target can't be set higher
					writeDebugStreamLine("Encoder target cannot be set any higher");
			}

			// If moving the target down one level
			else if(upOrDown == 'd')
			{
				*currentPosition = 'm';		// Set the current position variable

				// Determine which target we are switching, and change the appropriate variable
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetMed :
					((encoderTarget == &horizEncoderTarget)?horizTargetMed : tipTargetMed);

				writeDebugStreamLine("Encoder target set to MED");
			}
			break;

		// Target currently at the "medium" position
		case 'm':
			// If moving the target up one level
			if(upOrDown == 'u')
			{
				*currentPosition = 'h';		// Set the current position variable

				// Determine which target we are switching, and change the appropriate variable
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetHigh :
					((encoderTarget == &horizEncoderTarget)?horizTargetFar : tipTargetHigh);

				writeDebugStreamLine("Encoder target set to HIGH");
			}

			// If moving the target down one level
			else if(upOrDown == 'd')
			{
				*currentPosition = 'l';		// Set the encoder target

				// Determine which target we are switching, and change the appropriate variable
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetLow :
					((encoderTarget == &horizEncoderTarget)?horizTargetClose : tipTargetLow);

				writeDebugStreamLine("Encoder target set to LOW");
			}
			break;

		// Target currently at the "low" position
		case 'l' :
			// If moving the target up one level
			if(upOrDown == 'u')
			{
				*currentPosition = 'm';		// Set the current position variable

				// Determine which target we are switching, and change the appropriate variable
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetMed :
					((encoderTarget == &horizEncoderTarget)?horizTargetMed : tipTargetMed);

				writeDebugStreamLine("Encoder target set to MED");
			}

			// If moving the target down one level
			else if(upOrDown == 'd')
			{
				*currentPosition = 'b';		// Set the current position variable

				// Determine which target we are switching, and change the appropriate variable
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetBase :
					((encoderTarget == &horizEncoderTarget)?horizTargetBase : tipTargetBase);

				writeDebugStreamLine("Encoder target set to BASE");
			}
			break;

		// Target currently at the "low" (starting) position
		case 'b' :
			// If moving the target up one level
			if(upOrDown == 'u')
			{
				*currentPosition = 'l';		// Set the current position variable

				// Determine which target we are switching, and change the appropriate variable
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetLow :
					((encoderTarget == &horizEncoderTarget)?horizTargetClose : tipTargetLow);

				writeDebugStreamLine("Encoder target set to LOW");
			}

			// If moving the target down one level
			// This is the base or starting position. None of the manipulators can go any lower than this.
			else if(upOrDown == 'd')
			{
				writeDebugStreamLine("Encoder target cannot be set any lower");
			}
			break;

		// If the variable passed is not one of the possible choices, do nothing.
		default:
			break;
	}

	// Wait 25 milliseconds before continuing, so the drivers have time to let go of the button
	wait10Msec(25);
}

/*
*	checkButtons
*	Watch the buttons and make changes accordingly
*/
bool checkingButtons = true;
task checkButtons()
{
	// Store the desired positions for the horizontal and vertical lifts
	// Initialize them to their starting positions
	char liftPosition 	= 'b';
	char horizPosition 	= 'b';
	char tipPosition 	= 'b';

	writeDebugStreamLine("-- BUTTON CHECKER ACTIVATED --");

	while(checkingButtons)
	{
		// When a button is pressed, switch the variable attached to that function.

		if(buttonLiftUp)
		{
			writeDebugStreamLine("Switching vertical lift encoder target");
			switchEncoderTarget(&liftEncoderTarget, &liftPosition, 'u');
		}
		else if(buttonLiftDown)
		{
			writeDebugStreamLine("Switching vertical lift encoder target");
			switchEncoderTarget(&liftEncoderTarget, &liftPosition, 'd');
		}

		if(buttonLiftOut)
		{
			writeDebugStreamLine("Switching horizontal lift encoder target");
			switchEncoderTarget(&horizEncoderTarget, &horizPosition, 'u');
		}
		else if(buttonLiftIn)
		{
			writeDebugStreamLine("Switching horizontal lift encoder target");
			switchEncoderTarget(&horizEncoderTarget, &horizPosition, 'd');
		}

		if(buttonTipUp)
		{
			writeDebugStreamLine("Switching tip encoder target");
			switchEncoderTarget(&tipEncoderTarget, &tipPosition, 'u');
		}
		else if(buttonTipDown)
		{
			writeDebugStreamLine("Switching tip encoder target");
			switchEncoderTarget(&tipEncoderTarget, &tipPosition, 'd');
		}
	}

	writeDebugStreamLine("-- BUTTON CHECKER DEACTIVATED --");
}
