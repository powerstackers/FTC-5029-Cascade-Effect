#pragma config(StandardModel, "PS CASCADE EFFECT")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
*	AfterMatch.c
*	Program to run motors for reset, outside of match play.
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
*	January 8 2015
*	Version 0.2
*/

// Include a file that has some basic functions
#include "TeleopFunctions.h"

// Define constants for the buttons (this way we can remap them if we need to)
#define UP_BUTTON 	kRightButton
#define DOWN_BUTTON	kLeftButton
#define NEXT_BUTTON	kEnterButton
#define PREV_BUTTON kExitButton

float versionNumber = 0.2;
string programName 	= "Aftermatch";

task main()
{
	// Print a welcome message to the debug stream
	printWelcomeMessage(programName, versionNumber);

	// Put all motors and servos to their normal starting positions
	initializeRobot();

	// Make sure that the NXT screen is empty
	bDisplayDiagnostics = false;
	eraseDisplay();

	// Set the number of consecutive gray button clicks it takes to exit the program
	// This allows us to use the gray button to navigate
	nNxtExitClicks = 50;

	int selectedLine = 0;			// Store the currently selected line
	int previousLine = 5;			// Store the previously selected line

	// Start the selection indicator off at the top line
	nxtDisplayStringAt(0, 63, ">");

	// Display the name of the program in big letters at the bottom of the screen
	nxtDisplayCenteredTextLine(6, "AFTMATCH: HOLD");
	nxtDisplayCenteredTextLine(7, "ORANGE TO EXIT");

	// Store whether the enter button has been recently pressed
	bool nextRecentlyPressed 	= false;
	bool prevRecentlyPressed	= false;

	// Store whether the program should be running right now
	bool running = true;
	ClearTimer(T1);

	while(running)
	{
		/*
		*	Print all the motor and servo names and their values to the NXT LCD screen
		*/
		nxtDisplayStringAt(6, 63, "Lift:  %d   ", motor[mLift]);
		nxtDisplayStringAt(6, 55, "TrapD: %s", (servo[rTrapDoor]==trapDoorClosedPosition)?"closed":"open  ");
		nxtDisplayStringAt(6, 47, "Tilt:  %d   ", motor[mTip]);
		nxtDisplayStringAt(6, 39, "Grab:  %s", (servo[rGrabber]==grabberClosedPosition)?"closed":"open  ");
		nxtDisplayStringAt(6, 31, "Horiz: %d   ", motor[mHoriz]);
		nxtDisplayStringAt(6, 23, "Brush: %d   ", motor[mBrush]);

		/*
		*	NEXT BUTTON
		*	If the NEXT button is pressed, it has been recently pressed. Set the previous line to the current line,
		*	and the current line to the next line. If the new current line is past the bottom, bring it up to the top.
		*	Play a short sound to notify the drivers.
		*/
		if(nNxtButtonPressed==NEXT_BUTTON&&!nextRecentlyPressed)
		{
			writeDebugStreamLine("Move up the list");
			nextRecentlyPressed = true;
			previousLine = selectedLine;
			selectedLine--;
			if(selectedLine<0)
				selectedLine = 5;
			PlaySound(soundBlip);
		}
		// If the NEXT button is not pressed, it has not been recently pressed.
		if(nNxtButtonPressed!=NEXT_BUTTON)
			nextRecentlyPressed = false;


		/*
		*	PREVIOUS BUTTON
		*	Same deal as the NEXT button, but move the selector up rather than down.
		*/
		if(nNxtButtonPressed==PREV_BUTTON&&!prevRecentlyPressed)
		{
			writeDebugStreamLine("Move down the list");
			prevRecentlyPressed = true;
			previousLine = selectedLine;
			selectedLine++;
			if(selectedLine>5)
				selectedLine = 0;
			PlaySound(soundBlip);
		}
		// If the PREV button is not pressed, it has not been recently pressed
		if(nNxtButtonPressed!=PREV_BUTTON)
			prevRecentlyPressed = false;

		/*
		*	Erase the selection indicator from the previously selected line, and write a selection icon
		*	at the newly selected line.
		*/
		nxtDisplayStringAt(0, 63-(selectedLine*8), ">");
		nxtDisplayStringAt(0, 63-(previousLine*8), " ");

		/*
		*	UP AND DOWN BUTTONS
		*	If either the up or down button is pressed, change the value of the currently selected motor or servo.
		*/
		if(nNxtButtonPressed==UP_BUTTON||nNxtButtonPressed==DOWN_BUTTON)
		{
			// lift, trap, tilt, grab, horiz, brush
			/*
			*	For motors, if the UP button is pressed, run the motor forwards. If the DOWN button is pressed,
			*	run the motor in reverse.
			*/
			// Lift, line 0
			if(selectedLine==0)
				motor[mLift] = (nNxtButtonPressed==UP_BUTTON)?liftMotorSpeed:-1*liftMotorSpeed;

			// Horizontal, line 4
			if(selectedLine==4)
				motor[mHoriz] = (nNxtButtonPressed==UP_BUTTON)?horizMotorSpeed:-1*horizMotorSpeed;

			// Tip, line 2
			if(selectedLine==2)
				motor[mTip] = (nNxtButtonPressed==UP_BUTTON) ?tipMotorSpeed:-1*tipMotorSpeed;

			// Brush, line 5
			if(selectedLine==5)
				motor[mBrush] = (nNxtButtonPressed==UP_BUTTON)?brushMotorSpeed:-1*brushMotorSpeed;

			/*
			*	For servos, if the UP button is pressed, switch the servo to the open position. If the DOWN button
			*	is pressed, set the servo to its closed position.
			*/
			// Trapdoor, line 1
			if(selectedLine==1)
				servo[rTrapDoor] = (nNxtButtonPressed==UP_BUTTON)?trapDoorOpenPosition:trapDoorClosedPosition;

			// Grabber, line 3
			if(selectedLine==3)
				servo[rGrabber] = (nNxtButtonPressed==UP_BUTTON)?grabberOpenPosition:grabberClosedPosition;
		}

		/*
		*	If neither the UP or DOWN buttons are pressed, turn the motors off.
		*/
		else
		{
			motor[mLift] = 0;
			motor[mHoriz] = 0;
			motor[mTip] = 0;
			motor[mBrush] = 0;
		}

		// Check to see if the orange button is being held
		ClearTimer(T1);
		while(nNxtButtonPressed==NEXT_BUTTON&&time100[T1]<25)
		{
			// If this loop goes on for more than 2 seconds, exit the program
			if(time100[T1]>20)
			{
				running = false;
				writeDebugStreamLine("Exiting program...");
				break;
			}
		}

	}// END MAIN LOOP
	PlaySound(soundBeepBeep);
	eraseDisplay();
	nxtDisplayCenteredBigTextLine(2, "AFTMATCH");
	nxtDisplayCenteredBigTextLine(4, "DONE");
	wait10Msec(100);

}// END
