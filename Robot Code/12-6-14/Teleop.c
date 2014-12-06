#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S2,     ,               sensorSoundDB)
#pragma config(Sensor, S3,     mleftIR,        sensorI2CCustom)
#pragma config(Sensor, S4,     mrightIR,       sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     mDriveLeft,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     mDriveRight,   tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     mBrush,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     mLift,         tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
*	Teleop.c
*	Code to run our robot during the telop period.
*
*	THIS CODE IS PROVIDED AS-IS AND WITHOUT WARRANTY.
*	THIS CODE IS OPEN FOR DISTRIBUTION AND MODIFICATION.
*
*	FTC Team #5029, The Powerstackers
*	powerstackersftc.weebly.com
*	github.com/powerstackers
*/

// Include a file to handle messages from the joystick
#include "JoystickDriver.c"
#include "TeleopFunctions.h"

#define liftMotorSpeed 50

task main()
{
	// Sets robot to starting positions
	initializeRobot();

	// Play a ready sound
	PlaySound(soundFastUpwardTones);

	// Print a teleop ready message
	nxtDisplayBigTextLine(1, "TELEOP");
	nxtDisplayBigTextLine(3, "READY");

	// Wait for the start of the match
	//waitForStart();

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

		// BRUSH
		// If button 6 on joystick 1 is pressed, set the brush motor to full power.
		// If it is not pressed, set the brush motor to 0.
		motor[mBrush] = (buttonBrush)? 100 : 0;

		// LIFT
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
