/*
*	Menu.h
*	Displays a dynamic menu to choose different options and settings
*	during the autonomous period. All settings are saved to variables
*	which can be accessed from another program.
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
*	This code is modified from original code created by FTC team 3785
*	https://github.com/hprobotics/ftcresources/tree/master/AutonomousChooser
*
*	FTC Team #5029, The Powerstackers
*	powerstackersftc.com
*	github.com/powerstackers
*	December 31 2014
*	Version 0.1
*/

// Include file to handle messages from the joysticks
#include "../drivers/JoystickDriver.c"

// Constants store the button values
// Left and right arrow buttons are used to switch a value, the orange button is used to move to the next option in the list.
#define NEXT_BUTTON kRightButton
#define PREV_BUTTON kLeftButton
#define DOWN_BUTTON kExitButton
#define UP_BUTTON	kEnterButton

// Constants to store settings values
#define OFFENSIVE_MODE 	true
#define DEFENSIVE_MODE 	false
#define STARTING_RAMP 	true
#define STARTING_FLOOR 	false

#define maxLineIndex 	6
#define enterLine 		6
#define maxDelay 		15.0

/*
*	SETTINGS OPTIONS
*	These options are edited using this menu program. The options control different tasks and functions
*	during the autonomous position, such as starting position on the field, which strategy we want to use,
*	etc. The options and settings are used by the main autonomous program to make decisions.
*/
bool 	startingPosition 	= STARTING_RAMP;	// Starting position (ramp or floor)
bool 	offenseOrDefense 	= OFFENSIVE_MODE;	// Game mode (offensive or defensive)
bool	doCenterGoal		= false;			// Place ball in center goal or not
bool	doKickstand			= false;			// Knock over the kickstand or nor
float 	waitTime			= 0.0;				// Delay time
bool	debugMode			= false;			// Debug mode on/off

/*
*	printSettings
*	Print the chosen settings to the debug stream for review.
*/
void printSettings()
{
	writeDebugStreamLine("\tAUTONOMOUS SETTINGS:");
	writeDebugStreamLine("\tStart pos:\t%s", startingPosition==STARTING_FLOOR?"floor":"ramp");
	writeDebugStreamLine("\tGame mode:\t%s", offenseOrDefense==OFFENSIVE_MODE?"offensive":"defensive");
	writeDebugStreamLine("\tDo kickstand:\t%s", doKickstand?"YES":"NO");
	writeDebugStreamLine("\tDo center goal:\t%s", doCenterGoal?"YES":"NO");
	writeDebugStreamLine("\tDebug mode:\t%s", debugMode ? "ON":"OFF");
	writeDebugStreamLine("\tWait time:\t%2.2f", waitTime);
}

void runMenu()
{
	// Set the number of gray button clicks needed to exit the program to something ridiculous
	nNxtExitClicks = 50;

	// Clear the NXT screen
	bDisplayDiagnostics = false;
	eraseDisplay();

	// Store whether the program is ready
	bool ready = false;

	// Store the currently selected line and the previously selected line
	// Initialize the currenly selected line to line 0, and the previous line to the highest available line
	int currLine = 0;
	int prevLine = maxLineIndex;

	// Store whether the UP, DOWN, and ARROW buttons have been recently pressed
	bool upRecentlyPressed = false;
	bool downRecentlyPressed = false;
	bool arrowRecentlyPressed = false;

	// We have to print the ENTER line here at least once
	nxtDisplayCenteredTextLine(enterLine, "ENTER");

	// Run until the enter button is pressed
	while (!ready)
	{
		// If the delay is below zero, set it to zero (you can't wait negative 1 second)
		if(waitTime < 0.0)
			waitTime = 0.0;

		// If the delay is above the maximum delay, set it to the maximum delay
		else if(waitTime > maxDelay)
			waitTime = maxDelay;

		// Print all the variable names and their current values to the screen
		nxtDisplayStringAt(6, 63, "StrPos:   %s", startingPosition==STARTING_FLOOR ? "FLOOR":" RAMP");
		nxtDisplayStringAt(6, 55, "OfDef:  %s", offenseOrDefense==OFFENSIVE_MODE ? "OFFENSE":"DEFENSE");
		nxtDisplayStringAt(6, 47, "Center:     %s", doCenterGoal? "YES":"NO ");
		nxtDisplayStringAt(6, 39, "Kickst:     %s", doKickstand? "YES":"NO ");
		nxtDisplayStringAt(6, 31, "Debug:      %s", debugMode ? " ON":"OFF");
		nxtDisplayStringAt(6, 23, "Delay:      %2.1f", waitTime);

		// Print a selection icon next to the current line, or the appropriate string if we're on the ENTER line
		if(currLine==enterLine)
		{
			nxtDisplayCenteredTextLine(enterLine, "[ENTER]");
		}
		else
		{
			nxtDisplayStringAt(0, 63-(currLine*8), ">");
		}

		// Erase the selection icon from next to the previously selected line
		if(prevLine==enterLine)
		{
			nxtDisplayCenteredTextLine(enterLine, "ENTER");
		}
		else
		{
			nxtDisplayStringAt(0, 63-(prevLine*8), " ");
		}

		// If the right or left arrow button is pressed on the NXT, perform the appropriate action for the data
		// type of the selected variable. Switching the ready variable will end the program
		if((nNxtButtonPressed == NEXT_BUTTON || nNxtButtonPressed == PREV_BUTTON)&&!arrowRecentlyPressed)
		{
			// The arrow button is being pressed, so it has been recently pressed
			arrowRecentlyPressed = true;

			// Switch the variable that is on the currently selected line
			switch(currLine)
			{
				case 0:
					startingPosition = !startingPosition;
					break;
				case 1:
					offenseOrDefense = !offenseOrDefense;
					break;
				case 2:
					doCenterGoal = !doCenterGoal;
					break;
				case 3:
					doKickstand = !doKickstand;
					break;
				case 4:
					debugMode = !debugMode;
					break;
				case 5:
					waitTime += nNxtButtonPressed==NEXT_BUTTON?0.5:-0.5;
					break;
				case 6:
					ready = !ready;
			}

			// Play a short sound
			PlaySound(soundBlip);
		}

		// If the arrow buttons are not being pressed, then they have not been recently pressed
		if(nNxtButtonPressed!=NEXT_BUTTON&&nNxtButtonPressed!=PREV_BUTTON)
			arrowRecentlyPressed = false;

		// If the down button is pressed on the NXT, switch the active variable to the previous variable in the list
		if(nNxtButtonPressed==DOWN_BUTTON&&!downRecentlyPressed)
		{
			// If the down button is being pressed, then it has been recently pressed
			downRecentlyPressed = true;

			// Set the previously selected line to the current line
			prevLine = currLine;

			// Move the current line one closer to the end
			currLine++;

			// If we've set the current line higher than it can go, loop back to the beginning
			if(currLine>maxLineIndex)
				currLine = 0;

			// Play a short sound
			PlaySound(soundBlip);
		}

		// If the down button is not being pressed, then it has not been recently pressed
		if(nNxtButtonPressed!=DOWN_BUTTON)
			downRecentlyPressed = false;

		// If the up button is pressed on the NXT, switch the active variable to the next variable in the list
		if(nNxtButtonPressed==UP_BUTTON&&!upRecentlyPressed)
		{
			// If the up button is being pressed, then it has been recently pressed
			upRecentlyPressed = true;

			// Set the previously selected line to the current line
			prevLine = currLine;

			// Move the currently selected line one towards the beginning
			currLine--;

			// If we've set the current line lower than it can possibly go, move it to the last variable in the list
			if(currLine<0)
				currLine = maxLineIndex;

			// Play a short sound
			PlaySound(soundBlip);
		}

		// If the up button is not being pressed, then it has not been recently pressed
		if(nNxtButtonPressed!=UP_BUTTON)
			upRecentlyPressed = false;

	} // END MAIN LOOP
	// Clear the screen, and print all the settings decisions
	eraseDisplay();
	printSettings();

	// Put the gray button back to normal
	nNxtExitClicks = 1;
} // END
