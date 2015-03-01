#pragma config(StandardModel, "PSCE NEW ROBOT")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
*	Autonomous.c
*	Main autonomous program. This file handles all strategies and decision making.
*	The program will run a menu used to determine the different settings that the users want.
*	It will then make decisions based on those settings.
*	Options include defensive or offensive, starting position, wait times, etc.
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
*	January 30 2015
*	Version 0.5
*/

#include "headers/AutoFunctions.h"
#include "headers/CascadeEffect.h"
#include "headers/Menu.h"
#include "headers/EncoderFix.h"
#include "../Robot.h"

// Version number
#define programVersion 0.5

task main()
{

	/*
	*	INITIALIZATION AND SETUP
	*	This first section gets the robot ready to start the match. It sets all the motor and servo
	*	starting positions, and prints some diagnostic information to the NXT and the debug stream.
	*/

	// Print a copyright notice to the debug stream
	string programName = "Autonomous";
	printWelcomeMessage(programName, programVersion);

	writeDebugStreamLine("Getting autonomous settings...");
	PlaySound(soundBeepBeep);
	runMenu();			// Get gameplay decisions from the operators

	initializeRobot();	// Set the robot to its starting positions

	// Notify the users that the program is ready and running
	writeDebugStreamLine("Waiting for start of match...");
	nxtDisplayTextLine(0, "2015 Powerstackers");
	nxtDisplayCenteredBigTextLine(1, "AUTO");
	nxtDisplayCenteredBigTextLine(3, "READY");
	PlaySound(soundFastUpwardTones);
	wait10Msec(200);

	/*
	*	DEBUG MODE
	*	If debug mode is activated, bypass the waitForStart function.
	*	Display a 3 second audio and visual countdown, and then execute.
	*/
	if(debugMode)
	{
		ClearSounds();
		for(int i = 3; i>0; --i)
		{
			nxtDisplayCenteredBigTextLine(3, "IN %d", i);
			PlaySound(soundException);
			wait10Msec(100);
		}
	}
	else
	{
		// Wait for the "starting gun" from the field control system
		waitForStart();
	}

	// Detect and store the center goal position
	char goalFacing = findGoalOrientation();

	// Notify the users that the program is running
	nxtDisplayCenteredBigTextLine(3, "RUNNING");
	PlaySound(soundUpwardTones);
	//StartTask(watchMotors);

	/*
	*	GAMEPLAY
	*	From this point on, the program is split into different sections based on the options chosen in
	*	menu earlier. The two main options are the starting position and the game mode (offense or defense).
	*/

	if(startingPosition==STARTING_RAMP)
	{
		/*
		*	RAMP POSITION
		*	Starting from this position means that you can only access the rolling goals and the kickstand.
		*	In this position, offensive is mostly the only available game mode.
		*/

		// OFFENSIVE MODE
		if(offenseOrDefense==OFFENSIVE_MODE)
		{
			// Go straight down the ramp
			goTicks(inchesToTicks(-25), 30);
			turnDegrees(5, 50);
			goTicks(inchesToTicks(-40), 50);
			//this will make the robot drop the balls into the rolling goal
			grabTube();
			//this will make the robot turn to move twards the parking zone
			turnDegrees (-8,75);
			goTicks(inchesToTicks(12), 75);
			turnDegrees(-20, 75);
			//this will make the robot move twards the parking zone
			goTicks (inchesToTicks(78), 75);
			//this will make the robot turn so it can be in place so it will move in the parking zone
			turnDegrees (-80,75);
			goTicks(inchesToTicks(10), 50);
			dropBall(liftTargetMed);

		}	// END OFFENSE

		// DEFENSIVE MODE
		else if(offenseOrDefense==DEFENSIVE_MODE)
		{
			// Just go in a straight line down the ramp
			goTicks(inchesToTicks(80), 50);
		}	// END DEFENSE

	}	// END RAMP START
	else if(startingPosition==STARTING_FLOOR)
	{
		/*
		*	FLOOR POSITION
		*	In this position, you are available to block the other team, or to score in the high goal. You
		*	also have easy access to the kickstand. It is easiest to determine the orientation of the
		*	center tower from this position.
		*/

		// OFFENSIVE MODE
		if(offenseOrDefense==OFFENSIVE_MODE)
		{
			// Move slightly forwards to get into a better detecting position
			goTicks(inchesToTicks(-20), 50);

			switch (goalFacing)
			{
				case CENTGOAL_POSITION_A :           //closeness   do not ever put random comments in other stuff until after you think about it.
				{

				goTicks(inchesToTicks(-10),50); writeDebugStreamLine("Done");

				break;
				}

				case CENTGOAL_POSITION_B :           //middle
				{
				turnDegrees (45,75); 	writeDebugStreamLine("Done");

				goTicks(inchesToTicks(-30),75); writeDebugStreamLine("Done");

				turnDegrees (-87,75); writeDebugStreamLine("Done");

				break;
				}

				case CENTGOAL_POSITION_C :			//90 degrees
				{
				wait10Msec(100);

				turnDegrees (87,65);
				wait10Msec(100);
				goTicks(inchesToTicks(-31),65);
				wait10Msec(100);
				turnDegrees (-76,65);
				wait10Msec(100);
				goTicks(inchesToTicks(-43),65);
				wait10Msec(100);
				turnDegrees(-79,65); writeDebugStreamLine("Done");

				break;
				}

				default:
				goTicks(inchesToTicks(-20),75); writeDebugStreamLine("Done");
				{
				break;
				}

			}


			// Position the robot to drop the ball in the center goal
			wait10Msec (200);
			nMotorEncoder[mLift] = 0;
			moveMotorTo(mLift, liftTargetCent, 75);

			// Drop the ball in the center goal
			wait10Msec (100);
			servo[rTrapDoor]=trapDoorOpenPosition;
			wait10Msec (500);
			servo[rTrapDoor]=trapDoorClosedPosition;
			wait10Msec (100);

			// Drop down the lift
			moveMotorTo(mLift, liftTargetBase, 75);
			// Position the robot correctly to kick the kickstand
			turnDegrees (-87, 75);
			goTicks(inchesToTicks(-15), 75);
			turnDegrees (-73, 75);
			goTicks(inchesToTicks(35), 100);
			// Go to the robot's ending position


		}	// END OFFENSE

		// DEFENSIVE MODE
		else if(offenseOrDefense==DEFENSIVE_MODE)
		{
			// Move from the starting position to block the opponent's rolling goals
			goTicks(inchesToTicks(15), 100);	// Move 3 feet forwards at full power
			// Turn towards the opponent rolling goals
			turnDegrees(45, 75);
			// Drive forward to disrupt the opponent rolling goals
			goTicks(inchesToTicks(34), 75);
			goTicks(inchesToTicks(-12), 100);

			// turn slightly to get the rest of the goals
			turnDegrees(40, 50);
			goTicks(inchesToTicks(18), 75);
			goTicks(inchesToTicks(-18), 100);

			// Turn and go around the center goal towards our rolling goals
			turnDegrees(-45, 50);
			goTicks(inchesToTicks(-36), 100);
			turnDegrees(60, 50);
			goTicks(inchesToTicks(-20), 100);
		}	// END DEFENSE

	}	// END FLOOR START

	// After the program has been carried out, play a cute "done" sound
	nxtDisplayCenteredBigTextLine(3, "DONE");
	PlaySound(soundBeepBeep);
	wait10Msec(200);
} // END
