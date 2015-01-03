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

#include "JoystickDriver.c"

// Constants store the button values
// Left and right arrow buttons are used to switch a value, the orange button is used to move to 
// the next option in the list.
#define NEXT_BUTTON kRightButton
#define PREV_BUTTON kLeftButton
#define DOWN_BUTTON kEnterButton

// Constants to store settings values
#define OFFENSIVE_MODE 	true
#define DEFENSIVE_MODE 	false
#define STARTING_RAMP 	true
#define STARTING_FLOOR 	false

/*
*	SETTINGS OPTIONS
*	These options are edited using this menu program. The options control different tasks and functions
*	during the autonomous position, such as starting position on the field, which strategy we want to use,
*	etc. The options and settings are used by the main autonomous program to make decisions.
*/
bool 	startingPosition 	= STARTING_RAMP;
bool 	offenseOrDefense 	= OFFENSIVE_MODE;
float 	waitTime			= 0.0;

// Maximum allowable wait time
float 	maxDelay = 15.0;

/*
*	switchBool
*	Switch a boolean to the opposite value.
*/
void switchBool(bool* in, TButtons activeButton){
	if(activeButton == NEXT_BUTTON || activeButton == PREV_BUTTON)
		*in = !*in;
}

/*
*	switchInt
*	Increment or decrement an integer by an increment value.
*/
void switchInt(int* in, TButtons activeButton, int incrementValue){
	if(activeButton == NEXT_BUTTON)
		*in = *in + incrementValue;
	if(activeButton == PREV_BUTTON)
		*in = *in - incrementValue;
}

/*
*	switchFloat
*	Increment or decrement a floating point number by an increment value.
*/
void switchFloat(float* in, TButtons activeButton, float incrementValue){
	if(activeButton == NEXT_BUTTON)
		*in = *in + incrementValue;
	if(activeButton == PREV_BUTTON)
		*in = *in - incrementValue;
}


/*
*	printSettings
*	Print the chosen settings to the debug stream for review.
*/
void printSettings(){
	writeDebugStreamLine("\tAutonomous Settings:");
	writeDebugStreamLine("\tStarting position: %s", (startingPosition==STARTING_FLOOR)?"floor":"ramp");
	writeDebugStreamLine("\tGame mode: %s", (offenseOrDefense==OFFENSIVE_MODE)?"offensive":"defensive");
	writeDebugStreamLine("\tWait time: %2.2f", waitTime);
}

void runMenu(){
	// Clear the NXT screen
	bDisplayDiagnostics = false;
	eraseDisplay();

	// Declare variables to store the currently selected variable,
	// And the data type of the currently selected variable
	unsigned void* currVar 	= &startingPosition;
	char currType 			= 'b';

	// Run this code until the ready button is pressed
	bool ready = false;
	while (!ready){

		// If the delay is below zero, set it to zero
		if(waitTime < 0.0)
			waitTime = 0.0;

		// If the delay is above the maximum delay, set it to the maximum delay
		else if(waitTime > maxDelay)
			waitTime = maxDelay;

		// Print all the variable names and their current values to the screen
		nxtDisplayString(0, "StrPos:     %s", startingPosition ? "flr":"rmp");
		nxtDisplayString(1, "OfDef:      %s", offenseOrDefense ? "off":"def");
		nxtDisplayString(6, "Delay:      %2.1f", waitTime);

		// Print a selection icon next to the active variable name
		// Icon is 8 pixels high
		if(currVar == &startingPosition){
			nxtDisplayStringAt(94, 63, "<");
			nxtDisplayString(7,"     ENTER "); // The ready variable option has brackets around it
		}else if(currVar == &offenseOrDefense){
			nxtDisplayStringAt(94, 63, " ");
			nxtDisplayStringAt(94, 55, "<");
		}/*else if(currVar == &rampOtherSide){
			nxtDisplayStringAt(94, 47, " ");
			nxtDisplayStringAt(94, 39, "<");
		}else if(currVar == &delay){
			nxtDisplayStringAt(94, 39, " ");
			nxtDisplayStringAt(94, 31, "<");
		}else if(currVar == &forwardMotorRatio){
			nxtDisplayStringAt(94, 31, " ");
			nxtDisplayStringAt(94, 24, "<");
		}*/else if(currVar == &waitTime){
			nxtDisplayStringAt(94, 55, " ");
			nxtDisplayStringAt(94, 15, "<");
		}else if(currVar == &ready){
			nxtDisplayStringAt(94, 15, " ");
			nxtDisplayString(7, "    [ENTER]"); // The ready vairable option has brackets around it
		}

		// If the right or left arrow button is pressed on the NXT,
		// Perform the appropriate action for the data type of the selected variable
		// Switching the ready variable will end the program
		if(nNxtButtonPressed == NEXT_BUTTON ||
			nNxtButtonPressed == PREV_BUTTON){
			if(currType == 'b')
				switchBool(currVar, nNxtButtonPressed);
			else if(currType == 'i')
				switchInt(currVar, nNxtButtonPressed, 1);
			else if(currType == 'f')
				switchFloat(currVar, nNxtButtonPressed, 0.5);

			// Play a short sound
			PlaySound(soundBlip);

			// Clear the timer. While the timer reads less than four seconds
			// And any button is pressed, do nothing
			ClearTimer(T1);
			while(nNxtButtonPressed != kNoButton && time1[T1] <= 400){}
		}

		// If the orange button is pressed on the NXT,
		// Switch the active variable to the next variable in the list
		if(nNxtButtonPressed == DOWN_BUTTON){
			if(currVar == &startingPosition){
				currVar = &offenseOrDefense;
				currType = 'b';
			}else if(currVar == &offenseOrDefense){
				currVar = &waitTime;
				currType = 'f';
			}else if(currVar == &waitTime){
				currVar = &ready;
				currType = 'b';
			}else if(currVar == &ready){
				currVar = &startingPosition;
				currType = 'b';
			}/*else if(currVar == &delay){
				currVar = &forwardMotorRatio;
				currType = 'l';
			}else if(currVar == &forwardMotorRatio){
				currVar = &backwardMotorRatio;
				currType = 'l';
			}else if(currVar == &backwardMotorRatio){
				currVar = &startNear;
				currType = 'b';
			}*/

			// Play a short sound
			PlaySound(soundBlip);

			// Clear the timer. While the timer reads less than four seconds
			// And any button is pressed, do nothing
			ClearTimer(T1);
			while(nNxtButtonPressed != kNoButton && time1[T1] <= 400){}
		}
	}
	// Clear the screen, and print all the settings decisions
	eraseDisplay();
	printSettings();
}
