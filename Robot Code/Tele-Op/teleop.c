#pragma config(StandardModel, "PSCE NEW ROBOT")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
*	newrob_teleop.c
*	Code to run our robot V2 during the telop period.
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
*	Version 2.2
*/

// Include a file to handle messages from the joystick
#include "../drivers/JoystickDriver.c"
#include "TeleopFunctions.h"
#include "../Autonomous/headers/Sensors.h"
#include "../Robot.h"

// Version number
#define programVersion 2.2

// Threshold for motor encoder targeting. The program will seek to move the motors to within this distance
// of their targets. This keeps the motor from "wobbling"
#define encoderTargetThreshold 	50
#define stickPushThreshold 		15
#define liftEncoderStepValue 	15
#define tipEncoderStepValue		15


task main()
{
	// Write a copyright and welcome message to the debug stream
	string programName = "Tele-op RV2";
	printWelcomeMessage(programName, programVersion);

	writeDebugStreamLine("Initializing robot...");

	// Sets robot to starting positions
	initializeRobot();

	// Make it so that we control the disabling of the robot in the event of a lost connection
	//bOverrideJoystickDisabling = true;

	// Set the no message counter limit. This is the maximum acceptable number of milliseconds
	// since the last message from the field control system
	nNoMessageCounterLimit = 100;

	// Play a ready sound
	PlaySound(soundFastUpwardTones);

	// Print a teleop ready message to the NXT LCD screen
	nxtDisplayTextLine(0, "2015 Powerstackers");
	nxtDisplayCenteredBigTextLine(1, "TELEOP");
	nxtDisplayCenteredBigTextLine(3, "READY");

	// Wait for the start of the match
	writeDebugStreamLine("\nWaiting for match start...\n");
	waitForStart();

	// Print a teleop running message
	nxtDisplayCenteredBigTextLine(3, "RUNNING");

	// Play a starting sound
	PlaySound(soundUpwardTones);

	// Watch the buttons on joystick 2 and update motor encoder targets based on button input
	StartTask(checkButtons);

	// Store whether buttons have been recently pushed (at the start, no buttons have been recently pushed)
	bool buttonGrabJustPushed 		= false;
	bool buttonTrapDoorJustPushed 	= false;
	bool buttonFlapJustPushed 		= false;
	bool stickLiftRecentlyPushed	= false;

	// Set the number of clicks to the gray button needed to exit the program
	// We want to exit on our own terms, so we set this to 2. The first click will cause this program to shut itself down
	nNxtExitClicks = 2;

	/*
	*	MAIN LOOP
	*	Watch the joysticks, and update motors and servos accordingly
	*/
	// Loop until the Field Control System kills the process or the user exits
	bool running = true;
	while(running)
	{
		// Reset the NXT's internal inactivity timer. This keeps the NXT from falling asleep, even if the sleep time isn't set
		alive();

		// Updates joystick settings
		getJoystickSettings(joystick);
		getCustomJoystickSettings ();

		// Print both battery states, good or bad, to the NXT LCD screen
		nxtDisplayTextLine(5, "MAIN BATT %s", tetrixBatteryGoodState()?"GOOD":"BAD");
		nxtDisplayTextLine(6, "NXT BATT %s", nxtBatteryGoodState()?"GOOD":"BAD");


		// Check that we are still recieving messages from the FCS. If not, halt operation
		if(nNoMessageCounter>nNoMessageCounterLimit)
			haltOperation();

		/*
		*	DRIVE TRAIN
		*	Our drive train consists of two sets of two motors, one on the left, and one on the right.
		*	Pressing the "Straight Drive" button will cause both sets of motors to move together, useful
		*	for driving in straight lines. Pressing a second "backwards" trigger will cause all of the
		*	motors to reverse, and the controls to switch sides, essentially switching the "back" and "front"
		*	of the robot.
		*/

		// STRAIGHT DRIVE
		// If button 3  (red button) on joystick 1 is pressed
		if(buttonStraightDrive)
		{
			// Move both motors together based on left joystick position
			motor[mDriveLeft] = (abs(stickValueLeftForward) > stickPushThreshold)? stickToMotorValue(stickValueLeftForward) : 0;
			motor[mDriveRight] = (abs(stickValueLeftForward) > stickPushThreshold)? stickToMotorValue(stickValueLeftForward) : 0;
		}
		// BACKWARDS DRIVE
		// If button 5 (left shoulder) on joystick 1 is pressed
		else if(buttonBackwardsDrive)
		{
			// Switch and reverse the drive motors, effectively flipping the front and back of the robot
			motor[mDriveLeft] = (abs(stickValueLeftBackward) > stickPushThreshold)? stickToMotorValue(stickValueLeftBackward) : 0;
			motor[mDriveRight] = (abs(stickValueRightBackward) > stickPushThreshold)? stickToMotorValue(stickValueRightBackward) : 0;
		}
		// NORMAL DRIVE
		// If button 3 (red button) on joystick 1 is not pressed AND button 5 (left shoulder) on joystick 1 is not pressed
		else if(!buttonStraightDrive && !buttonBackwardsDrive)
		{
			// Move the motors independently, based on their respective joystick positions
			motor[mDriveLeft] = (abs(stickValueLeftForward) > stickPushThreshold)? stickToMotorValue(stickValueLeftForward) : 0;
			motor[mDriveRight] = (abs(stickValueRightForward) > stickPushThreshold)? stickToMotorValue(stickValueRightForward) : 0;
		}


		/*
		*	MANIPULATORS
		*	There are currently five manipulators on our robot: a brush to collect balls, a lift to
		*	raise them up to the higher goals, a grabber to attach to the rolling goals, and a horizontal
		*	slide to extend the rolling goals out so they can be tipped over. The vertical lift, horizontal
		*	lift, and the tipping portion of the grabber all have different positions they can toggle
		*	between. Those are controlled in the checkButtons task.
		*/

		/*
		*	BRUSH
		*	Simple button on/off
		*/
		// If button 6 (right shoulder) on joystick 1 is pressed, set the brush motor to full power.
		// If button 8 (right trigger) on joystick 1 is pressed, set the brush motor to full reverse power.
		// If it is not pressed, set the brush motor to 0.
		motor[mBrush] = buttonBrush? brushMotorSpeed : (buttonBrushReverse? -1*brushMotorSpeed:0);


		/*
		*	VERTICAL LIFT
		*	Constantly moves toward a target encoder value. The encoder target can be changed manually using
		*	a joystick, or by flipping through preprogrammed positions with buttons. There is a touch sensor
		*	that is activated when the lift is at the base position. If this touch sensor is active, the lift
		*	cannot be moved any lower.
		*/
		// The encoder targets for the lift is updated by the checkButtons task, independent of the main task.
		// This block of code keeps the motors moving towards their target.

		// If the motor encoder value further from its target than a certain threshold, move towards the target
		if(abs(nMotorEncoder[mLift] - liftEncoderTarget)>encoderTargetThreshold)
		{
			motor[mLift] = 	(nMotorEncoder[mLift]<liftEncoderTarget)?liftMotorSpeed:(-1*liftMotorSpeed);
		}
		// If the encoder is within a certain threshold distance of its target, stop
		else
		{
			motor[mLift] = 0;
		}

		// LIFT MANUAL CONTROL
		// If the lift motor manual control stick is pushed past the threshold, change the lift motor encoder target
		if(abs(stickLiftTarget)>stickPushThreshold)
		{
			// The lift stick has been recenly pushed
			stickLiftRecentlyPushed = true;

			// Move the lift down if the stick is pushed down and the touch sensor is not activated
			// If the touch sensor IS activated, stop the motor completely, and only allow it to move up
			if(stickLiftTarget<0)
			{
				if(touchActive(touchLiftStop))
				{
					nMotorEncoder[mLift] = 0;
					liftEncoderTarget = 0;
					liftPosition = 'b';
				}
				else
				{
					liftEncoderTarget -= liftEncoderStepValue;
				}
			}
			// Move the lift up if the stick is pushed up
			else if(stickLiftTarget>0)
			{
				liftEncoderTarget += liftEncoderStepValue;
			}
		}
		// If the stick is released and the stick has been recenly pushed, the lift stops moving
		// We check this so that the lift only halts at this point if the user has pressed and then released the stick
		else if(stickLiftRecentlyPushed)
		{
			stickLiftRecentlyPushed = false;
			liftEncoderTarget = nMotorEncoder[mLift];
		}

		// If the lift encoder reset button is pressed, reset the encoder value to 0
		if(buttonLiftEncoderReset)
			nMotorEncoder[mLift] = 0;

		/*
		*	For the grabber, flaps, and trapdoor, we have a toggling system. Each manipulator has a variable
		*	that stores whether the button for it is currently pushed. We also have one that stores whether
		*	that button has been /recently/ pushed. If the button gets pushed, and has not been recently pushed,
		*	the servo will switch to the opposite position. If it is pushed and it /has/ been recently pushed,
		*	then nothing will happen. When the button is released, then the "recently pushed" indicator is
		*	turned off.
		*	Using this system, the servo will only change position once per button press.
		*/
		// GRABBER

		// Update the grabber motor speed so that it is always moving towards its target
		if(abs(nMotorEncoder[mGrab] - grabEncoderTarget) > encoderTargetThreshold)
		{
			motor[mGrab] = (nMotorEncoder[mGrab] < grabEncoderTarget)? grabMotorSpeed : -grabMotorSpeed;
		}
		else
		{
			motor[mGrab] = 0;
		}

		// If the grab button is pushed, and has not been recently pushed, toggle the encoder target to the other one
		if(buttonGrabToggle && !buttonGrabJustPushed)
		{
			buttonGrabJustPushed = true;
			grabEncoderTarget = (grabEncoderTarget==grabClosedPosition)?grabOpenPosition:grabClosedPosition;
		}
		// If the button is released, it has not been recenlty pressed
		if(!buttonGrabToggle)
			buttonGrabJustPushed = false;

		// FLAPS
		if(buttonFlaps && !buttonFlapJustPushed)
		{
			servo[rFlapLeft] 	= (servo[rFlapLeft]==flapLeftOpenPosition)	?flapLeftClosedPosition:flapLeftOpenPosition;
			servo[rFlapRight] 	= (servo[rFlapRight]==flapRightOpenPosition)?flapRightClosedPosition:flapRightOpenPosition;
			buttonFlapJustPushed = true;
			writeDebugStreamLine("Toggled flaps to %s position", (servo[rFlapLeft]==flapLeftOpenPosition)?"open":"closed");
		}
		if(!buttonFlaps)
			buttonFlapJustPushed = false;


		// TRAPDOOR
		if(buttonTrapDoor && !buttonTrapDoorJustPushed)
		{
			// If the trapdoor is in the open or starting positions, move it to the idle position
			if(servo[rTrapDoor]==trapDoorOpenPosition||servo[rTrapDoor]==trapDoorClosedPosition)
			{
				servo[rTrapDoor] = trapDoorIdlePosition;
			}

			// If the trapdoor is in the align position (second lowest), move it to the open position (lowest)
			else if(servo[rTrapDoor]==trapDoorAlignPosition)
			{
				servo[rTrapDoor] = trapDoorOpenPosition;
			}

			// If the trapdoor is in the idle position (second highest), move it to the align position (second lowest)
			else if(servo[rTrapDoor]==trapDoorIdlePosition)
			{
				servo[rTrapDoor] = trapDoorAlignPosition;
			}


			buttonTrapDoorJustPushed = true;
			writeDebugStreamLine("Toggled trapdoor to %s position", (servo[rTrapDoor]==trapDoorOpenPosition)?"open":"closed");
		}
		if(!buttonTrapDoor)
			buttonTrapDoorJustPushed = false;

		// If the gray button is pressed, exit the main loop
		if(nNxtButtonPressed==0)
			running = false;
	}
	// END MAIN LOOP

	nxtDisplayCenteredBigTextLine(3, "DONE");

	/*
	*	EXIT
	*/
	// Turn all motors off
	motor[mDriveLeft] 	= 0;
	motor[mDriveRight] 	= 0;
	motor[mLift] 		= 0;

	// Notify the drivers that we have exited
	PlaySound(soundBeepBeep);
	writeDebugStreamLine("\n-- EXIT --");
	wait10Msec(100);	// Make sure there's enough time for the sound to play


}	// END TASK MAIN
