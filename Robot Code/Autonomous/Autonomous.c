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
*	March 1 2015
*	Version 2.1
*/

#include "headers/AutoFunctions.h"
#include "headers/CascadeEffect.h"
#include "headers/Menu.h"
#include "headers/EncoderFix.h"
#include "../Robot.h"

// Version number
#define programVersion 2.1

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
			turnDegrees (-8,preferredTurnSpeed);
			goTicks(inchesToTicks(12), 75);
			turnDegrees(-20, preferredTurnSpeed);
			//this will make the robot move twards the parking zone
			goTicks (inchesToTicks(78), 75);
			//this will make the robot turn so it can be in place so it will move in the parking zone
			turnDegrees (-80,preferredTurnSpeed);
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

		// Detect and store the center goal position
		goTicks(inchesToTicks(-30), 100);
		char goalFacing = findGoalOrientation();

		// OFFENSIVE MODE
		if(offenseOrDefense==OFFENSIVE_MODE)
		{
			// Move slightly forwards to get into a better detecting position
			//goTicks(inchesToTicks(-20), 50);

			// Make a movement based on the position of the center goal
			switch (goalFacing)
			{
				// GOOD
				case CENTGOAL_POSITION_A :
				{
					// In this position, simply go in a straight line
					goTicks(inchesToTicks(-10),50);
					break;
				}
				// BAD
				case CENTGOAL_POSITION_B :
				{
					// Turn 45 degrees, tangent to the center structure
					turnDegrees (45,preferredTurnSpeed);

					// Move into position in front of the goal
					goTicks(inchesToTicks(-30),75);

					// Turn to face the goal
					turnDegrees (-90,preferredTurnSpeed); writeDebugStreamLine("Done");
					break;
				}
				// GOOD
				case CENTGOAL_POSITION_C :
				{
					// Turn 90 degrees, parallel to the center structure
					turnDegrees (-90,preferredTurnSpeed);

					// Move along the center structure until we're past the end
					goTicks(inchesToTicks(-38),50);

					// Turn 90 degrees, tangent to the center structure
					turnDegrees (90,preferredTurnSpeed);

					// Move into position in front of the center structure
					goTicks(inchesToTicks(-62),65);

					//  Turn to face the goal
					turnDegrees(90,preferredTurnSpeed);
					break;
				}

				default:
				{
					// In an ambiguous situtation, assume the goal is in A position
					goTicks(inchesToTicks(-20),75); writeDebugStreamLine("Done");
					break;
				}
			}

			// Print that the alignment is done
			writeDebugStreamLine("Aligned with center structure");


			// Position the robot to drop the ball in the center goal
			nMotorEncoder[mLift] = 0;
			moveMotorTo(mLift, liftTargetCent, 75);

			// Drop the ball in the center goal
			servo[rTrapDoor]=trapDoorOpenPosition;
			wait10Msec (500);
			servo[rTrapDoor]=trapDoorClosedPosition;

			// Drop down the lift
			moveMotorTo(mLift, liftTargetBase, 75);
			// Position the robot correctly to kick the kickstand
			turnDegrees (90, preferredTurnSpeed);
			goTicks(inchesToTicks(-15), 75);
			turnDegrees (90, preferredTurnSpeed);
			goTicks(inchesToTicks(35), 100);
			// Go to the robot's ending position


		}	// END OFFENSE

		// DEFENSIVE MODE
		else if(offenseOrDefense==DEFENSIVE_MODE)
		{
			/*
			*	Move from the starting position to block the opponent's rolling goals
			*/
			// Move 2 feet backwards at full power, to get out of the parking zone
			//goTicks(inchesToTicks(-24), 100);

			// Turn towards the opponent rolling goals
			turnDegrees(90, preferredTurnSpeed);

			// Drive forward and backward to disrupt the opponent 60cm goal
			goTicks(inchesToTicks(60), 100);
			goTicks(inchesToTicks(-12), 100);

			// turn slightly to get the 90cm goals
			turnDegrees(-45, preferredTurnSpeed);

			// Move forward and backwards to disrupt the 90cm goal
			goTicks(inchesToTicks(36), 75);
			goTicks(inchesToTicks(-36), 100);
return;
			// Turn slightly to face the 30cm goal
			turnDegrees(-45, preferredTurnSpeed);

			// Move foward and backward to disrupt the 30cm goal
			goTicks(inchesToTicks(36), 75);
			goTicks(inchesToTicks(-4), 100);

			// Move into position to align on the wall of the ramp
			turnDegrees(-90, preferredTurnSpeed);
			//goTicks(inchesToTicks(12), 100);
			//turnDegrees(90, preferredTurnSpeed);
return;
			// Align ourselves with this wall
			//wallAlign(ALIGN_FORWARD);

			/*
			*	BLOCK CETNER GOAL
			*	Move the robot in front of the opponent's center goal
			*/

			// Move into position for use to block the center goal
			goTicks(inchesToTicks(-6), 100);	// Move away from the wall
			turnDegrees(90, preferredTurnSpeed);				// Turn to face OUR ramp
			goTicks(inchesToTicks(-36), 100);		// Move foward, past the center goal
			//turnDegrees(90, preferredTurnSpeed);				// Turn parallel to the cetner structure

			// Move to block the center goal, in one of three positions
			switch(goalFacing)
			{
				// Farthest position
				case CENTGOAL_POSITION_A:
				{
					writeDebugStreamLine("-- BLOCKING GOAL POSITION A --");
					goTicks(inchesToTicks(-80), 100);	// Move alongside the goal
					turnDegrees(90, preferredTurnSpeed);				// Turn tangent to the center structure
					goTicks(inchesToTicks(20), 100);	// Move in front of the goal
					break;
				}
				// Middle position
				case CENTGOAL_POSITION_B:
				{
					writeDebugStreamLine("-- BLOCKING GOAL POSITION B --");
					goTicks(inchesToTicks(-40), 100);	// Move alongside the goal
					turnDegrees(45, preferredTurnSpeed);				// Turn tangent to the center structure
					goTicks(inchesToTicks(20), 100);	// Move in front of the goal
					break;
				}
				// Closest position
				case CENTGOAL_POSITION_C:
				{
					writeDebugStreamLine("-- BLOCKING GOAL POSITION C --");
					goTicks(inchesToTicks(-20), 100);	// Move in front of the goal
					break;
				}
				default:
				{
					break;
				}
			}

		}	// END DEFENSE

	}	// END FLOOR START

	// After the program has been carried out, play a cute "done" sound
	nxtDisplayCenteredBigTextLine(3, "DONE");
	PlaySound(soundBeepBeep);
	wait10Msec(200);
} // END
