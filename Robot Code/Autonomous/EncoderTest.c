#pragma config(StandardModel, "PS CASCADE EFFECT")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
*	EncoderTest.c
*	Code used to test the encoders. Used for testing purposes only.
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

// Include files to handle the joystick messages, and some basic functions.
#include "../drivers/JoystickDriver.c"
#include "../Tele-Op/TeleopFunctions.h"

// Store the motor that is currently in use.
short motorUsing = mLift;

task main()
{
	nMotorEncoder[motorUsing] = 0;	// Set the encoder value to 0
	
	// Make sure that the NXT LCD screen is clear
	bDisplayDiagnostics = false;
	eraseDisplay();
	
	// Loop until the FCS kills the process
	while(true)
	{
		// Retrieve the joystick values
		getJoystickSettings(joystick);

		// If joystick 1 left shoulder is pressed, set the motor to 25
		if(joy1Btn(5)==1)
			motor[motorUsing] = 25;
		
		// If joystick 1 left trigger is pressed, set the motor to -25
		else if(joy1Btn(7)==1)
			motor[motorUsing] = -25;
			
		// If neither button is pressed, set the motor to 0
		else
			motor[motorUsing] = 0;

		// If joystick 1 right shoulder is pressed, lower the trapdoor
		if(joy1Btn(6)==1)
			servo[rTrapDoor] = trapDoorOpenPosition;

		// If joystick 1 blue button is pressed, reset the encoder value
		if(joy1Btn(1)==1)
			nMotorEncoder[motorUsing] = 0;

		// Write the encoder value out to the screen
		nxtDisplayTextLine(2, "Enc:%d", nMotorEncoder[motorUsing]);
	}	// END MAIN LOOP
}	// END
