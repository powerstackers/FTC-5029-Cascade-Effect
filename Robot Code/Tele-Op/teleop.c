#pragma config(StandardModel, "PS CASCADE EFFECT")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
*	Teleop.c
*	Code to run our robot during the telop period.
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

// Include a file to handle messages from the joystick
#include "JoystickDriver.c"
#include "TeleopFunctions.h"

#define liftMotorSpeed 50

task main()
{
	writeDebugStreamLine("Tele-op  Copyright (C) 2015  Powerstackers\nThis program comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are welcome to redistribute it under certain conditions; see LICENST.txt for details.");
	writeDebugStreamLine("Initializing robot...");

	// Sets robot to starting positions
	initializeRobot();

	// Play a ready sound
	PlaySound(soundFastUpwardTones);

	// Print a teleop ready message
	nxtDisplayBigTextLine(1, "TELEOP");
	nxtDisplayBigTextLine(3, "READY");

	// Wait for the start of the match
	writeDebugStreamLine("\nWaiting for match start...\n");
	waitForStart();

	// Print a teleop running message
	nxtDisplayBigTextLine(3, "RUNNING");

	// Play a starting sound
	PlaySound(soundUpwardTones);

	StartTask(checkButtons);

	// Loop forever
	while(true)
	{
		// Updates joystick settings
		getJoystickSettings(joystick);

		getCustomJoystickSettings ();

		printInfoToScreen();

		/*
		*	DRIVE TRAIN
		*	Our drive train consists of two sets of two motors, one on the left, and one on the right.
		*	Pressing the "Straight Drive" button will cause both sets of motors to move together, useful
		*	for driving in straight lines. Pressing a second "backwards" trigger will cause all of the
		*	motors to reverse, and the controls to switch sides, essentially switchin the "back" and "front"
		*	of the robot.
		*/

		// STRAIGHT DRIVE
		// If button 3 on joystick 1 is pressed
		if(buttonStraightDrive)
		{
			// Move both motors together based on left joystick position
			motor[mDriveLeft] = (abs(stickValueLeftForward) > 15)? stickToMotorValue(stickValueLeftForward) : 0;
			motor[mDriveRight] = (abs(stickValueLeftForward) > 15)? stickToMotorValue(stickValueLeftForward) : 0;
		}
		// BACKWARDS DRIVE
		// If button 5 is pressed
		else if(buttonBackwardsDrive)
		{
			motor[mDriveLeft] = (abs(stickValueLeftBackward) > 15)? stickToMotorValue(stickValueLeftBackward) : 0;
			motor[mDriveRight] = (abs(stickValueRightBackward) > 15)? stickToMotorValue(stickValueRightBackward) : 0;
		}
		// NORMAL DRIVE
		// If button 3 on joystick 1 is not pressed
		else
		{
			// Move the motors independently, based on their respective joystick positions
			motor[mDriveLeft] = (abs(stickValueLeftForward) > 15)? stickToMotorValue(stickValueLeftForward) : 0;
			motor[mDriveRight] = (abs(stickValueRightForward) > 15)? stickToMotorValue(stickValueRightForward) : 0;
		}


		/*
		*	MANIPULATORS
		*	There are currently five manipulators on our robot: a brush to collect balls, a lift to
		*	raise them up to the higher goals, a grabber to attach to the rolling goals, and a horizontal
		*	slide to extend the rolling goals out so they can be tipped over. The vertical lift, horizontal
		*	lift, and the tipping portion of the grabber all have different positions they can toggle
		*	between. Those are controlled in the checkButtons task.
		*/

		// BRUSH
		// If button 6 on joystick 1 is pressed, set the brush motor to full power.
		// If it is not pressed, set the brush motor to 0.
		motor[mBrush] = (buttonBrush)? 100 : 0;

		// LIFT
		// The lift control is handled by checkButtons. This block of code only keeps the motors moving
		// towards their target.
		// If the motor encoder value is less than the target, move the lift up
		// If the motor encoder vlaue us greater than the target, move the lift down
		if(nMotorEncoder[mLift] < liftEncoderTarget)
		{
			motor[mLift] = 	liftMotorSpeed;
		}
		else if(nMotorEncoder[mLift] > liftEncoderTarget)
		{
			motor[mLift] = -1 * liftMotorSpeed;
		}
		else
		{
			motor[mLift] = 0;
		}

		// HORIZONTAL LIFT
		// If the motor encoder value is less than the target, move the horizontal lift out
		// If the motor encoder vlaue us greater than the target, move the horizontal lift in
		/*if(nMotorEncoder[mHoriz] < horizEncoderTarget)
		{
			motor[mHoriz] = liftMotorSpeed;
		}
		else if(nMotorEncoder[mHoriz] > horizEncoderTarget)
		{
			motor[mHoriz] = -1 * liftMotorSpeed;
		}
		else
		{
			motor[mHoriz] = 0;
		}*/

	}
}
