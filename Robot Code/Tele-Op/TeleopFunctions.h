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
#define motorPowerMultiplier 		0.65

/*
*	FUNCTION PROTOTYPES
*/
void getCustomJoystickSettings ();	// Fetch all the custom joystick configurations

// Utility functions
void printInfoToScreen();			// Print diagnostic informaiton to the NXT LCD screen
short stickToMotorValue(short stickValue);	// Convert a stick value to a motor power level
void switchEncoderTarget(char* currentPosition, char upOrDown);	// Change an encoder target
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

bool 	buttonStraightDrive 	= false;// Straight drive mode button
bool 	buttonBackwardsDrive 	= false;// Backwards drive mode button

bool 	buttonBrush 			= false;// Ball collection brush IN button
bool	buttonBrushReverse		= false;// Ball collection brush OUT button

bool 	buttonLiftUp 			= false;// Vertical lift UP button
bool 	buttonLiftDown 			= false;// Vertical lift DOWN button
bool	buttonTrapDoor 			= false;// Ball bucket door toggle
bool	buttonLiftEncoderReset	= false;// Reset the lift encoder value to 0

bool 	buttonGrabToggle 		= false;// Rolling goal grabber toggle button

bool	buttonFlaps 			= false;// Flaps button
//bool	buttonBallDoor			= false;// Ball door control

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
long grabEncoderTarget 	= grabOpenPosition;


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
	//buttonBallDoor			= false;

	// Player 2
	//		Joysticks
	stickLiftTarget			= joystick.joy2_y2;				// Driver 2 right stick

	//		Buttons
	buttonLiftUp 			= (joy2Btn(6) == 1);			// Driver 2 right shoulder
	buttonLiftDown 			= (joy2Btn(8) == 1);			// Driver 2 right trigger
	buttonLiftEncoderReset	= (joy2Btn(9) == 1);			// Driver 2 START button
	buttonGrabToggle		= (joy2Btn(1) == 1);			// Driver 2 blue button
	buttonTrapDoor			= (joy2Btn(2) == 1);			// Driver 2 green button
	buttonFlaps				= (joy2Btn(4) == 1);			// Driver 2 yellow button
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
*	switchEncoderTarget
*	Move an encoder target up or down one position
*	Inputs: encoder target variable to switch; the current position of the motor; wether to move up or down one position.
*	The tipper, vertical lift, and horizontal lift all have four positions: base, low, medium, and high. The vertical lift
*	also has an additional "center goal" position.
*/
void switchEncoderTarget(char* currentPosition, char upOrDown)
{
	// Check the current position, and move it one up or one down in the list.
	switch(*currentPosition)
	{
	// Target currently at the "center goal" position
	case 'c':
		// If switching the target to a lower position
		if(upOrDown != 'u')
		{
			*currentPosition = 'h';				// Set teh current position variable
			liftEncoderTarget = liftTargetHigh;	// Set the encoder target

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
			*currentPosition = 'c';				// Set the current position variable
			liftEncoderTarget = liftTargetCent;	// Set the encoder target
			writeDebugStreamLine("Encoder target set to CENTER");
		}

		// If moving the target down one level
		else if(upOrDown == 'd')
		{
			*currentPosition = 'm';				// Set the current position variable
			liftEncoderTarget = liftTargetMed;	// Set the encoder target

			writeDebugStreamLine("Encoder target set to MED");
		}
		break;

	// Target currently at the "medium" position
	case 'm':
		// If moving the target up one level
		if(upOrDown == 'u')
		{
			*currentPosition = 'h';				// Set the current position variable
			liftEncoderTarget = liftTargetHigh;	// Set the encoder target

			writeDebugStreamLine("Encoder target set to HIGH");
		}

		// If moving the target down one level
		else if(upOrDown == 'd')
		{
			*currentPosition = 'l';				// Set the current position variable
			liftEncoderTarget = liftTargetLow;	// Set the encoder target

			writeDebugStreamLine("Encoder target set to LOW");
		}
		break;

	// Target currently at the "low" position
	case 'l' :
		// If moving the target up one level
		if(upOrDown == 'u')
		{
			*currentPosition = 'm';				// Set the current position variable
			liftEncoderTarget = liftTargetMed;	// Set the encoder target

			writeDebugStreamLine("Encoder target set to MED");
		}

		// If moving the target down one level
		else if(upOrDown == 'd')
		{
			*currentPosition = 'b';				// Set the current position variable
			liftEncoderTarget = liftTargetBase;	// Set the encoder target

			writeDebugStreamLine("Encoder target set to BASE");
		}
		break;

	// Target currently at the "low" (starting) position
	case 'b' :
		// If moving the target up one level
		if(upOrDown == 'u')
		{
			*currentPosition = 'l';				// Set the current position variable
			liftEncoderTarget = liftTargetLow;	// Set the encoder target

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
// Store the desired positions for the horizontal and vertical lifts
// Initialize them to their starting positions
char liftPosition 	= 'b';
bool checkingButtons = true;
task checkButtons()
{


	// Store whether each button has been recently pushed
	bool buttonLiftUpRecentlyPushed 	= false;
	bool buttonLiftDownRecentlyPushed 	= false;

	writeDebugStreamLine("-- BUTTON CHECKER ACTIVATED --");

	while(checkingButtons)
	{
		// When a button is pressed, switch the variable attached to that function.


		/*
		*	VERTICAL LIFT
		*/

		// UP
		// If the up button is pressed, and has not been recently pressed
		if(buttonLiftUp && !buttonLiftUpRecentlyPushed)
		{
			buttonLiftUpRecentlyPushed = true;
			writeDebugStreamLine("Switching vertical lift encoder target up");
			switchEncoderTarget(&liftPosition, 'u');
		}
		// If the up button is let go, it has not been recently pressed
		if(!buttonLiftUp)
			buttonLiftUpRecentlyPushed = false;

		// DOWN
		// If the down button is pressed, has not been recently pressed, and the up button is not pressed
		if(buttonLiftDown && !buttonLiftDownRecentlyPushed && !buttonLiftUp)
		{
			buttonLiftDownRecentlyPushed = true;
			writeDebugStreamLine("Switching vertical lift encoder target down");
			switchEncoderTarget(&liftPosition, 'd');
		}
		// If the down button is not pressed, it has not been recently pressed
		if(!buttonLiftDown)
			buttonLiftDownRecentlyPushed = false;
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
