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
*	January 19 2015
*	Version 0.4
*/

#include "../drivers/JoystickDriver.c"
#include "../Robot.h"

/*
*	MACROS
*	Macros to store motor speeds, encoder targets, and other values that do not change.
*/

// Multiplier for the drive train motors. A value of 1 changes nothing, a value of 0.5 sets all motors to half power
#define motorPowerMultiplier 		0.75

/*
*	FUNCTION PROTOTYPES
*/

void initializeRobot();				// Put the robot into its starting positions
void getCustomJoystickSettings ();	// Fetch all the custom joystick configurations

// Utility functions
void printInfoToScreen();			// Print diagnostic informaiton to the NXT LCD screen
short stickToMotorValue(short stickValue);	// Convert a stick value to a motor power level
void switchEncoderTarget(unsigned long* encoderTarget, char* currentPosition, char upOrDown);	// Change an encoder target
task checkButtons();				// Loop through and update encoder targets
void haltOperation();				// Halt operation of the robot in the event of a lost connection

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

short 	stickValueLeftForward 	= 0;	// Left drive control stick (forward)
short 	stickValueRightForward 	= 0;	// Right drive control stick (forward)
short 	stickValueLeftBackward 	= 0;	// Left drive control (backwards)
short 	stickValueRightBackward = 0;	// Right drive control (backwards)
short	stickLiftTarget			= 0;	// Raise or lower the lift motor encoder target
short	stickTipTarget			= 0;	// Raise or lower the tip motor encoder target


bool 	buttonStraightDrive 	= false;// Straight drive mode button
bool 	buttonBackwardsDrive 	= false;// Backwards drive mode button

bool 	buttonBrush 			= false;// Ball collection brush IN button
bool	buttonBrushReverse		= false;// Ball collection brush OUT button

bool 	buttonLiftUp 			= false;// Vertical lift UP button
bool 	buttonLiftDown 			= false;// Vertical lift DOWN button
bool	buttonTrapDoor 			= false;// Ball bucket door toggle
bool	buttonDeflectorToggle 	= false;// Ball deflector toggle
bool	buttonLiftEncoderReset	= false;// Reset the lift encoder value to 0

bool 	buttonLiftOut 			= false;// Horizontal lift OUT button
bool 	buttonLiftIn 			= false;// Horizontal lift IN button
bool 	buttonGrabToggle 		= false;// Rolling goal grabber toggle button
bool 	buttonTipUp 			= false;// Goal tipper UP button
bool 	buttonTipDown 			= false;// Goal tipper DOWN button
bool	buttonTipEncoderReset	= false;// Reset the tip encoder value to 0

bool	buttonFlaps 			= false;// Flaps button

/*
*	ENCODER TARGETS
*	The vertical lift, horizontal lift, and the tipping mechanism all have specific positions to which
*	they are set. The main program will monitor their position, and try to keep it as close to their
*	target as possible. The targets are switched in functions below, based on button input.
*	The different motors all have a starting position, and then a low, medium, and high goal positions.
*	The vertical lift also has a fifth position for the center goal tube.
*/

// Long integers store the current target
long liftEncoderTarget 	= 0;	// Vertical lift
long horizEncoderTarget = 0;	// Horizontal lift
long tipEncoderTarget 	= 0;	// Tipper

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
	//		Joysticks
	stickValueRightForward 	= joystick.joy1_y2;				// Driver 1 right stick
	stickValueLeftForward 	= joystick.joy1_y1;				// Driver 1 left stick
	stickValueRightBackward = -1 * joystick.joy1_y1;		// Driver 1 left stick
	stickValueLeftBackward 	= -1 * joystick.joy1_y2;		// Driver 1 right stick

	//		Buttons
	buttonStraightDrive 	= (joy1Btn(3) == 1);			// Driver 1 red button
	buttonBackwardsDrive 	= (joy1Btn(5) == 1);			// Driver 1 left shoulder
	buttonBrush 			= (joy1Btn(6) == 1);			// Driver 1 right shoulder
	buttonBrushReverse		= (joy1Btn(8) == 1);			// Driver 1 right trigger

	// Player 2
	//		Joysticks
	stickLiftTarget			= joystick.joy2_y2;				// Driver 2 right stick
	stickTipTarget			= joystick.joy2_y1;				// Driver 2 left stick

	//		Buttons
	buttonLiftUp 			= (joy2Btn(6) == 1);			// Driver 2 right shoulder
	buttonLiftDown 			= (joy2Btn(8) == 1);			// Driver 2 right trigger
	buttonLiftEncoderReset	= (joy2Btn(9) == 1);			// Driver 2 START button
	buttonLiftOut 			= (joy2Btn(5) == 1);			// Driver 2 left shoulder
	buttonLiftIn 			= (joy2Btn(7) == 1);			// Driver 2 left trigger
	buttonGrabToggle		= (joy2Btn(1) == 1);			// Driver 2 blue button
	//buttonTipUp				= (joystick.joy2_TopHat == 4);	// Driver 2 top hat down
	//buttonTipDown 			= (joystick.joy2_TopHat == 0);	// Driver 2 top hat up
	buttonTipEncoderReset	= (joy2Btn(8) == 1);			// Driver 2 SELECT button
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
	return (short) (motorPowerMultiplier * (float)stickValue * 0.78125);
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

	/*
	*	BATTERY LEVEL CHECK
	*	The program will measure the NXT and TETRIX battery levels and print them to the debug stream and the
	*	NXT LCD screen. It will also display error notices if the battery levels are too low.
	*/
	// Measure and print the battery levels to the debug stream
	writeDebugStreamLine("\tChecking battery levels...\n\tTETRIX battery level: %2.2f volts", externalBatteryAvg / 1000.0); // Battery levels are given to us in kilovolts
	writeDebugStreamLine("\tNXT battery level: %2.2f volts", nAvgBatteryLevel / 1000.0);

	// If the battery level is low, notify the drivers
	if(externalBatteryAvg < tetrixBatteryMinimumLevel)
	{		// Battery level below 13 volts is considered low
		PlaySound(soundException);		// Beep from the NXT speaker
		// Print Low Battery messages to the debug stream and the NXT LCD screen
		writeDebugStreamLine("--!! MAIN BATTERY LOW !!--\n\t Avg Batt Level: %2.2f volts",
			externalBatteryAvg / 1000.0);
		nxtDisplayTextLine(5, "MAIN BATT LOW");

		// If the battery level reads negative, the battery could be disconnected.
		if(externalBatteryAvg<0.0)
		{
			writeDebugStreamLine("\tCheck that battery is connected.");
		}
	}
	else
	{
		// If the battery level is acceptable, print to the NXT LCD screen
		nxtDisplayTextLine(5, "MAIN BATT GOOD");
	}

	// NXT Low Battery message
	if(nAvgBatteryLevel < nxtBatteryMinimumLevel)
	{	// Battery level below 7.5 volts is considered low
		PlaySound(soundException);	// Beep from the NXT speaker
		// Print Low Battery messages to the debug stream and the NXT LCD screen
		writeDebugStreamLine("--!! NXT BATTERY LOW !!--\n\tAvg Batt Level: %2.2f",
			nAvgBatteryLevel / 1000.0);
		nxtDisplayTextLine(6, "NXT BATT LOW");
	}
	else
	{
		// If the battery level is acceptable, print to the NXT LCD screen
		nxtDisplayTextLine(6, "NXT BATT GOOD");
	}

	/*
	*	MOTOR INITIALIZATION
	*	This section sets all the motors to their starting position. Some motors have to be at a specific orientation
	*	at the start of the match in order to function correctly (e.g. the tipper).
	*/
	// Set all motors to their starting power levels
	// All motors should be set to 0 power (i.e. not moving)
	motor[mDriveLeft] 	= 0;
	motor[mDriveRight] 	= 0;
	motor[mBrush] 		= 0;
	motor[mLift] 		= 0;

	// All encoder positions that we use start at zero
	nMotorEncoder[mLift] = 0;
	nMotorEncoder[mTip] = 0;
	nMotorEncoder[mHoriz] = 0;

	/*
	*	SERVO INITIALIZATION
	*	This section sets all servos to their starting positions, and also changes some of the servo settings.
	*/
	// Servos should be set to the closed position
	servo[rFlapLeft] 	= flapLeftClosedPosition;
	servo[rFlapRight] 	= flapRightClosedPosition;
	servo[rGrabber] 	= grabberOpenPosition;
	servo[rTrapDoor] 	= trapDoorClosedPosition;

	// Set the servo speed of some servos. This makes the servo change positions slower.
	// The number of the setting indicates the number of positions the servo moves every 20 milliseconds
	servoChangeRate[rTrapDoor] = 30;

	// Make it so that servos maintain their positions after the program ends
	bSystemLeaveServosEnabledOnProgramStop = true;

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
				if(encoderTarget == &liftEncoderTarget||encoderTarget == &tipEncoderTarget)
				{
					*currentPosition = 'c';				// Set the current position variable
					*encoderTarget = (encoderTarget==&liftEncoderTarget)?liftTargetCent:tipTargetFloor;	// Set the encoder target
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

	// Store whether each button has been recently pushed
	bool buttonLiftUpRecentlyPushed 	= false;
	bool buttonLiftDownRecentlyPushed 	= false;
	bool buttonLiftOutRecentlyPushed 	= false;
	bool buttonLiftInRecentlyPushed 	= false;
	bool buttonTipUpRecentlyPushed 		= false;
	bool buttonTipDownRecentlyPushed 	= false;

	writeDebugStreamLine("-- BUTTON CHECKER ACTIVATED --");

	while(checkingButtons)
	{
		// When a button is pressed, switch the variable attached to that function.


		/*
		*	VERTICAL LIFT
		*/
		// If the up button is pressed, and has not been recently pressed
		if(buttonLiftUp&&!buttonLiftUpRecentlyPushed)
		{
			buttonLiftUpRecentlyPushed = true;
			writeDebugStreamLine("Switching vertical lift encoder target up");
			switchEncoderTarget(&liftEncoderTarget, &liftPosition, 'u');
		}
		// If the up button is let go, it has not been recently pressed
		if(!buttonLiftUp)
			buttonLiftUpRecentlyPushed = false;

		// If the down button is pressed, has not been recently pressed, and the up button is not pressed
		if(buttonLiftDown&&!buttonLiftDownRecentlyPushed&&!buttonLiftUp)
		{
			buttonLiftDownRecentlyPushed = true;
			writeDebugStreamLine("Switching vertical lift encoder target down");
			switchEncoderTarget(&liftEncoderTarget, &liftPosition, 'd');
		}
		// If the down button is not pressed, it has not been recently pressed
		if(!buttonLiftDown)
			buttonLiftDownRecentlyPushed = false;


		/*
		*	HORIZONTAL LIFT
		*/
		// If the out button is pressed and has not been recently pressed
		if(buttonLiftOut&&!buttonLiftOutRecentlyPushed)
		{
			buttonLiftOutRecentlyPushed = true;
			writeDebugStreamLine("Switching horizontal lift encoder target out");
			switchEncoderTarget(&horizEncoderTarget, &horizPosition, 'u');
		}
		// If the out button is let go, it has not been recently pressed
		if(!buttonLiftOut)
			buttonLiftOutRecentlyPushed = false;

		// If the in button is pressed, has not been recently pressed, and the out button is not pressed
		if(buttonLiftIn&&!buttonLiftInRecentlyPushed&&!buttonLiftOut)
		{
			buttonLiftInRecentlyPushed = true;
			writeDebugStreamLine("Switching horizontal lift encoder target in");
			switchEncoderTarget(&horizEncoderTarget, &horizPosition, 'd');
		}
		// If the in button is not pressed, it has not been recently pressed
		if(!buttonLiftIn)
			buttonLiftInRecentlyPushed = false;


		/*
		*	TIPPER
		*/
		// If the up button is pressed and has not been recently pressed
		if(buttonTipUp&&!buttonTipUpRecentlyPushed)
		{
			buttonTipUpRecentlyPushed = true;
			writeDebugStreamLine("Switching tip encoder target up");
			switchEncoderTarget(&tipEncoderTarget, &tipPosition, 'u');
		}
		// If the up button is pressed, it has not been recently pressed
		if(!buttonTipUp)
			buttonTipUpRecentlyPushed = false;

		// If the down button is pressed, has not been recently pressed, and the up button is not pressed
		if(buttonTipDown&&!buttonTipDownRecentlyPushed&&!buttonTipUp)
		{
			buttonTipDownRecentlyPushed = true;
			writeDebugStreamLine("Switching tip encoder target down");
			switchEncoderTarget(&tipEncoderTarget, &tipPosition, 'd');
		}
		// If the down button is not pressed, it has not been recently pressed
		if(!buttonTipDown)
			buttonTipDownRecentlyPushed = false;
	}

	writeDebugStreamLine("-- BUTTON CHECKER DEACTIVATED --");
}

/*
*	haltOperation
*	In the event of a lost connection, halt operation of the robot until the connection is regained.
*/
void haltOperation()
{
	// Print that the connection has been lost
	writeDebugStreamLine("\n--! CONNECTION LOST !--");

	// Reset the timer. We want to know how long we were out for
	ClearTimer(T1);

	// For as long as the time since the last message is longer than the limit
	while(nNoMessageCounter>nNoMessageCounterLimit){
		// Halt all the motors
		motor[mDriveLeft] 	= 0;
		motor[mDriveRight] 	= 0;
		motor[mLift] 		= 0;
		motor[mHoriz] 		= 0;
		motor[mTip] 		= 0;
		motor[mBrush] 		= 0;

		// Make an obnoxious beeping sound
		PlaySound(soundException);

		// Print LOST COMM to the NXT LCD screen
		nxtDisplayCenteredBigTextLine(1, "LOST");
		nxtDisplayCenteredBigTextLine(3, "COMM");
	}	// CONNECTION REESTABLISHED

	// Play a good sound
	ClearSounds();
	PlaySound(soundUpwardTones);

	// Print TELEOP RUNNING to the NXT LCD screen
	nxtDisplayCenteredBigTextLine(1, "TELEOP");
	nxtDisplayCenteredBigTextLine(3, "RUNNING");

	// Print that the connection has been regained
	writeDebugStreamLine("-- CONNECTION ESTABLISHED --\n\tTotal downtime: %3.2f seconds", (float)time100[T1]/10);
}
