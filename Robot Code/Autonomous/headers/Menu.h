/*
*	Menu.h
*	Displays a dynamic menu to choose different options and settings
*	during the autonomous period. All settings are saved to a struct
*	which can be accessed from another program.
*
*	THIS CODE IS PROVIDED AS-IS AND WITHOUT WARRANTY.
*	THIS CODE IS OPEN FOR DISTRIBUTION AND MODIFICATION.
*
*	This code is modified from original code created by FTC team 3785
*	https://github.com/hprobotics/ftcresources/tree/master/AutonomousChooser
*
*	FTC Team #5029, The Powerstackers
*	powerstackersftc.weebly.com
*	github.com/powerstackers
*/

#include "JoystickDriver.c"

// Constants store the button values
#define NEXT_BUTTON kRightButton
#define PREV_BUTTON kLeftButton
#define DOWN_BUTTON kEnterButton

// Constants to store settings values
#define OFFENSIVE_MODE 	'o'
#define DEFENSIVE_MODE 	'd'
#define STARTING_RAMP 	'r'
#define STARTING_FLOOR 	'f'

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

// Settings options
bool 	startingPosition;
bool 	offenseOrDefense;
float 	waitTime;
float 	maxDelay = 15.0;

void printSettings(){

}

void runMenu(){
	// Clear the NXT screen
	bDisplayDiagnostics = false;
	eraseDisplay();

	// Declare variables to store the currently selected variable,
	// And the data type of the currently selected variable
	unsigned void* currVar;
	char currType;

	// Initialize the current variable to startNear,
	// And initialize the data type to "boolean"
	currVar = &startingPosition;
	currType = 'b';

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
