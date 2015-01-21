#pragma config(StandardModel, "PS CASCADE EFFECT")
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
*	January 20 2015
*	Version 0.3
*/

#include "headers/AutoFunctions.h"
#include "headers/CascadeEffect.h"
#include "headers/Menu.h"
#include "../Robot.h"

// Version number
#define programVersion 0.3

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
			wait10Msec(150);
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
			goTicks(inchesToTicks(80), 75);
			//this will make the robot drop the balls into the rolling goal
			dropBall(liftTargetMed);
			//this will make the robot turn to move twards the parking zone
			turnDegrees (30,50);
			//this will make the robot move twards the parking zone
			goTicks (inchesToTicks(100), 100);
			//this will make the robot turn so it can be in place so it will move in the parking zone
			turnDegrees (60,50);
			//this will make the robot move forward so it is fully in the parking zone
			goTicks (inchesToTicks(12), 50);


			// Do stuff

		}	// END OFFENSE

		// DEFENSIVE MODE
		else if(offenseOrDefense==DEFENSIVE_MODE)
		{

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
			// Place balls in the center goal
			char goalFacing = findGoalOrientation();
		}	// END OFFENSE

		// DEFENSIVE MODE
		else if(offenseOrDefense==DEFENSIVE_MODE)
		{
			// Move from the starting position to block the opponent's rolling goals
			goTicks(inchesToTicks(60), 100);	// Move 3 feet forwards at full power
		}	// END DEFENSE

	}	// END FLOOR START

	// After the program has been carried out, play a cute "done" sound
	nxtDisplayCenteredBigTextLine(3, "DONE");
	PlaySound(soundBeepBeep);
	wait10Msec(200);
} // END
