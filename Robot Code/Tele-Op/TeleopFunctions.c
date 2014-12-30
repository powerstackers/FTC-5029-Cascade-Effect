#pragma config(StandardModel, "PS CASCADE EFFECT")

#include "JoystickDriver.c"
#include "TeleopFunctions.h"

/*
*	getCustomJoystickSettings
*	Translate the joystick values to reusable variables
*/
void getCustomJoystickSettings ()
{
	stickValueRightForward = joystick.joy1_y2;			// Driver 1 right stick
	stickValueLeftForward = joystick.joy1_y1;			// Driver 1 left stick
	stickValueRightBackward = -1 * joystick.joy1_y1;	// Driver 1 left stick
	stickValueLeftBackward = -1 * joystick.joy1_y2;		// Driver 1 right stick

	buttonStraightDrive = (joy1Btn(3) == 1);			// Driver 1 red button
	buttonBackwardsDrive = (joy1Btn(5) == 1);			// Driver 1 left shoulder
	buttonBrush = (joy1Btn(6) == 1);					// Driver 1 rigth shoulder
	buttonLiftUp = (joy2Btn(6) == 1);					// Driver 2 right shoulder
	buttonLiftDown = (joy2Btn(8) == 1);					// Driver 2 right trigger
	buttonLiftOut = (joy2Btn(5) == 1);					// Driver 2 left shoulder
	buttonLiftIn = (joy2Btn(7) == 1);					// Driver 2 left trigger
	buttonGrabUp = (joystick.joy2_TopHat == 0);			// Driver 2 top hat up
	buttonGrabDown = (joystick.joy2_TopHat == 4);		// Driver 2 top hat down

}

/*
*	printInfoToScreen
*	Print information to the NXT screen
*/
void printInfoToScreen()
{

	nxtDisplayTextLine(1, "LeftDr:%d", motor[mDriveLeft]);		// Left drive motor settings
	nxtDisplayTextLine(2, "RightDr:%d", motor[mDriveRight]);	// Right drive motor settings
	nxtDisplayTextLine(3, "LeftSt:%d", stickValueLeftForward);	// Left joystick
	nxtDisplayTextLine(4, "RightSt:%d", stickValueRightForward);	// Right joystick
	nxtDisplayTextLine(5, "RB:%d", stickValueRightBackward);
	nxtDisplayTextLine(6, "LB:%d", stickValueLeftBackward);
}

/*
* 	stickToMotorValue
* 	Convert the -127 - 128 stick value to a -100 - 100 motor value
*/
short stickToMotorValue(short stickValue)
{
	return (short) ( (float)stickValue * 0.78125);
}

/*
*	initializeRobot
*	Sets the robot's motors and servos to their starting values. Also
*	prepares sensors and multiplexers, as well as the NXT.
*/
void initializeRobot()
{
	// Clear the nxt screen
	bDisplayDiagnostics = false;
	eraseDisplay();

	// Measure and print the battery levels
	writeDebugStreamLine("extBatt lvl: %2.2f volts\nNXT Batt lvl: %2.2f volts", externalBatteryAvg / 1000.0, nAvgBatteryLevel / 1000.0);
	if(externalBatteryAvg < 13000){
		PlaySound(soundException);
		writeDebugStreamLine("--!! MAIN BATTERY LOW !!--\n\t Avg Batt Level: %2.2f", externalBatteryAvg / 1000.0);
		nxtDisplayTextLine(4, "MAIN BATT LOW");
	}
	else
		nxtDisplayTextLine(4, "MAIN BATT GOOD");

	if(nAvgBatteryLevel < 7500){
		PlaySound(soundException);
		writeDebugStreamLine("--!! NXT BATTERY LOW !!--\n\tAvg Batt Level: %2.2f", nAvgBatteryLevel / 1000.0);
		nxtDisplayTextLine(5, "NXT BATT LOW");
	}
	else
		nxtDisplayTextLine(5, "NXT BATT GOOD");

	// Set all motors and servos to their starting positions
	motor[mDriveLeft] 	= 0;
	motor[mDriveRight] 	= 0;
	motor[mBrush] 		= 0;
	motor[mLift] 		= 0;

	writeDebugStreamLine("-- ROBOT INITIALIZED --");
}

/*
*
*	Watch the buttons and make changes accordingly
*/
task checkButtons()
{
	// Store the desired positions for the horizontal and vertical lifts
	char liftPosition = 'b';
	char horizPosition = 'b';
	while(true)
	{

		if(buttonLiftUp)
		{
			switch(liftPosition)
			{
				case 'b':
					liftPosition = 'l';
					liftEncoderTarget = liftTargetLow;
					writeDebugStreamLine("Switched lift target to LOW");
					break;
				case 'l':
					liftPosition = 'm';
					liftEncoderTarget = liftTargerMed;
					writeDebugStreamLine("Switched lift target to MED");
					break;
				case 'm':
					liftPosition = 'h';
					liftEncoderTarget = liftTargetHigh;
					writeDebugStreamLine("Switched lift target to HIGH");
					break;
				case 'h':
					liftPosition = 'c';
					liftEncoderTarget = liftTargetCent;
					writeDebugStreamLine("Switched lift target to CENT");
					break;
				case 'c':
					writeDebugStreamLine("Lift target cannot go any higher");
					break;
				default:
					break;
			}
			wait10Msec(25);
		}
		else if(buttonLiftDown)
		{
			switch(liftPosition)
			{
				case 'c':
					liftPosition = 'h';
					liftEncoderTarget = liftTargetHigh;
					writeDebugStreamLine("Switched lift target to HIGH");
					break;
				case 'h':
					liftPosition = 'm';
					liftEncoderTarget = liftTargerMed;
					writeDebugStreamLine("Switched lift target to MED");
					break;
				case 'm':
					liftPosition = 'l';
					liftEncoderTarget = liftTargetLow;
					writeDebugStreamLine("Switched lift target to LOW");
					break;
				case 'l' :
					liftPosition = 'b';
					liftEncoderTarget = liftTargetBase;
					writeDebugStreamLine("Switched lift target to BASE");
					break;
				case 'b' :
					writeDebugStreamLine("Lift target cannot go any lower");
					break;
				default:
					break;
			}
			wait10Msec(25);
		}


		if(buttonLiftOut)
		{
			switch(horizPosition)
			{
				case 'b':
					horizPosition = 'c';
					horizEncoderTarget = horizTargetClose;
					writeDebugStreamLine("Switched horiz target to CLOSE");
					break;
				case 'c':
					horizPosition = 'm';
					horizEncoderTarget = horizTargetMed;
					writeDebugStreamLine("Switched horiz target to MED");
					break;
				case 'm':
					horizPosition = 'f';
					horizEncoderTarget = horizTargetFar;
					writeDebugStreamLine("Switched horiz target to FAR");
					break;
				case 'f':
					writeDebugStreamLine("Horiz target cannot go any farther");
					break;
				default:
					break;
			}
			wait10Msec(25);
		}
		else if(buttonLiftIn)
		{
			switch(horizPosition)
			{
				case 'f':
					horizPosition = 'm';
					horizEncoderTarget = horizTargetMed;
					writeDebugStreamLine("Switched horiz target to MED");
					break;
				case 'm':
					horizPosition = 'c';
					horizEncoderTarget = horizTargetClose;
					writeDebugStreamLine("Switched horiz target to CLOSE");
					break;
				case 'c' :
					horizPosition = 'b';
					horizEncoderTarget = horizTargetBase;
					writeDebugStreamLine("Switched horiz target to BASE");
					break;
				case 'b' :
					writeDebugStreamLine("Horiz target cannot go any nearer");
					break;
				default:
					break;
			}
			wait10Msec(25);
		}
	}
}
