/*
*	TeleopFunctions.h
*	Functions and subroutines for use during the teleop period.
*
*	THIS CODE IS PROVIDED AS-IS AND WITHOUT WARRANTY.
*	THIS CODE IS OPEN FOR DISTRIBUTION AND MODIFICATION.
*
*	FTC Team #5029, The Powerstackers
*	powerstackersftc.com
*	github.com/powerstackers
*	December 31 2014
*	Version 0.1
*/

/*
*	FUNCTION PROTOTYPES
*/

void initializeRobot();

// Utility functions
void printInfoToScreen();
short stickToMotorValue(short stickValue);
void switchEncoderTarget(long* encoderTarget, char* currentPosition, char upOrDown = 'u');
task checkButtons();

/*
*	JOYSTICK ASSIGNMENTS
*	Each function in our program that is user-controlled is hinged on a variable or set of variables.
*	For example, the vertical lift has a button for up and a button for down. Instead of looking at 
*	the button variable from JoystickDriver.c, the main program has its own unique variable that it
*	uses in place of the button. This allows us to change which button is assigned to which operation
*	without having to rewrite the whole program every time.
*	For our purposes, a stick is represented by a short integer containing its value, and a button is
*	represented by a boolean representing "pressed" or "not pressed."
*/

short stickValueLeftForward;
short stickValueRightForward;
short stickValueLeftBackward;
short stickValueRightBackward;

bool buttonStraightDrive;
bool buttonBackwardsDrive;
bool buttonBrush;

bool buttonLiftUp;
bool buttonLiftDown;
bool buttonLiftOut;
bool buttonLiftIn;
bool buttonGrabUp;
bool buttonGrabDown;

/*
*	ENCODER TARGETS
*	The vertical lift, horizontal lift, and the tipping mechanism all have specific positions to which
*	they are set. The main program will monitor their position, and try to keep it as close to their 
*	target as possible. The targets are switched in functions below, based on button input.
*	The different motors all have a starting position, and then a low, medium, and high positions.
*	The vertical lift also has a fifth position for the center goal tube.
*/

// Macros to store the different available positions
#define liftTargetBase 		1
#define liftTargetLow 		2
#define liftTargetMed 		3
#define liftTargetHigh 		4
#define liftTargetCent 		5

#define horizTargetBase 	1
#define horizTargetClose	2
#define horizTargetMed 		3
#define horizTargetFar 		4

#define grabTargetBase 		1
#define grabTargetLow 		2
#define grabTargetMed 		3
#define grabTargetHigh 		4

// Long integers store the current target
long liftEncoderTarget 	= 0;
long horizEncoderTarget = 0;
long grabEncoderTarget 	= 0;


/*
*	getCustomJoystickSettings
*	Translate the joystick values to reusable variables
*/
void getCustomJoystickSettings ()
{
	stickValueRightForward 	= joystick.joy1_y2;				// Driver 1 right stick
	stickValueLeftForward 	= joystick.joy1_y1;				// Driver 1 left stick
	stickValueRightBackward = -1 * joystick.joy1_y1;		// Driver 1 left stick
	stickValueLeftBackward 	= -1 * joystick.joy1_y2;		// Driver 1 right stick

	buttonStraightDrive 	= (joy1Btn(3) == 1);			// Driver 1 red button
	buttonBackwardsDrive 	= (joy1Btn(5) == 1);			// Driver 1 left shoulder
	buttonBrush 			= (joy1Btn(6) == 1);			// Driver 1 rigth shoulder
	buttonLiftUp 			= (joy2Btn(6) == 1);			// Driver 2 right shoulder
	buttonLiftDown 			= (joy2Btn(8) == 1);			// Driver 2 right trigger
	buttonLiftOut 			= (joy2Btn(5) == 1);			// Driver 2 left shoulder
	buttonLiftIn 			= (joy2Btn(7) == 1);			// Driver 2 left trigger
	buttonGrabUp 			= (joystick.joy2_TopHat == 0);	// Driver 2 top hat up
	buttonGrabDown 			= (joystick.joy2_TopHat == 4);	// Driver 2 top hat down

}

/*
*	printInfoToScreen
*	Print information to the NXT screen
*/
void printInfoToScreen()
{
	nxtDisplayTextLine(1, "LeftDr:%d", motor[mDriveLeft]);			// Left drive motor settings
	nxtDisplayTextLine(2, "RightDr:%d", motor[mDriveRight]);		// Right drive motor settings
	nxtDisplayTextLine(3, "LeftSt:%d", stickValueLeftForward);		// Left joystick
	nxtDisplayTextLine(4, "RightSt:%d", stickValueRightForward);	// Right joystick
	nxtDisplayTextLine(5, "p2TopHat:%d", joystick.joy2_TopHat);		// Player two arrow keys
	nxtDisplayTextLine(6, "LB:%d", stickValueLeftBackward);			// Left backward stick
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
	writeDebugStreamLine("\tChecking battery levels...\n\textBatt lvl: %2.2f volts\n\tNXT Batt level: %2.2f volts", 
		externalBatteryAvg / 1000.0, nAvgBatteryLevel / 1000.0);
	
	// If the battery level is low, notify the drivers
	if(externalBatteryAvg < 13000){
		PlaySound(soundException);
		writeDebugStreamLine("--!! MAIN BATTERY LOW !!--\n\t Avg Batt Level: %2.2f volts", 
			externalBatteryAvg / 1000.0);
		nxtDisplayTextLine(4, "MAIN BATT LOW");
		
		// If the battery level reads negative, the battery could be disconnected.
		if(externalBatteryAvg<0.0)
			writeDebugStreamLine("\tCheck that batter is connected.");
	}
	else
		nxtDisplayTextLine(4, "MAIN BATT GOOD");

	if(nAvgBatteryLevel < 7500){
		PlaySound(soundException);
		writeDebugStreamLine("--!! NXT BATTERY LOW !!--\n\tAvg Batt Level: %2.2f", 
			nAvgBatteryLevel / 1000.0);
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
*	switchEncoderTarget
*	Move an encoder target up or down one position
*/
void switchEncoderTarget(long* encoderTarget, char* currentPosition, char upOrDown = 'u')
{
	// Check the current position, and move it one up or one down in the list.
	switch(*currentPosition)
	{
		case 'c':
			if(upOrDown != 'u')
			{
				*currentPosition = 'h';
				// Determine which encoder target we are switching, and change the appropriate variable
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetHigh : 
					((encoderTarget == &horizEncoderTarget)?horizTargetFar : grabTargetHigh);
				writeDebugStreamLine("Switched encoder target to HIGH");
			}
			else
			{
				writeDebugStreamLine("Encoder target cannot be set any higher");
			}
			break;
		case 'h':
			if(upOrDown == 'u')
			{
				// Only move the target up from this position if we are switching the vertical lift
				if(encoderTarget == &liftEncoderTarget)
				{
					*currentPosition = 'c';
					*encoderTarget = liftTargetCent;
					writeDebugStreamLine("Encoder target set to CENTER");
				}
				else
					writeDebugStreamLine("Encoder target cannot be set any higher");
			}
			else if(upOrDown == 'd')
			{
				*currentPosition = 'm';
				// Determine which target we are switching, and change the appropriate variable
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetMed : 
					((encoderTarget == &horizEncoderTarget)?horizTargetMed : grabTargetMed);
				writeDebugStreamLine("Encoder target set to MED");
			}
			break;
		case 'm':
			if(upOrDown == 'u')
			{
				*currentPosition = 'h';
				// Determine which target we are switching, and change the appropriate variable
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetHigh : 
					((encoderTarget == &horizEncoderTarget)?horizTargetFar : grabTargetHigh);
				writeDebugStreamLine("Encoder target set to HIGH");
			}
			else if(upOrDown == 'd')
			{
				*currentPosition = 'l';
				// Determine which target we are switching, and change the appropriate variable
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetLow : 
					((encoderTarget == &horizEncoderTarget)?horizTargetClose : grabTargetLow);
				writeDebugStreamLine("Encoder target set to LOW");
			}
			break;
		case 'l' :
			if(upOrDown == 'u')
			{
				*currentPosition = 'm';
				// Determine which target we are switching, and change the appropriate variable
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetMed : 
					((encoderTarget == &horizEncoderTarget)?horizTargetMed : grabTargetMed);
				writeDebugStreamLine("Encoder target set to MED");
			}
			else if(upOrDown == 'd')
			{
				*currentPosition = 'b';
				// Determine which target we are switching, and change the appropriate variable
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetBase : 
					((encoderTarget == &horizEncoderTarget)?horizTargetBase : grabTargetBase);
				writeDebugStreamLine("Encoder target set to BASE");
			}
			break;
		case 'b' :
			if(upOrDown == 'u')
			{
				*currentPosition = 'l';
				// Determine which target we are switching, and change the appropriate variable
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetLow : 
					((encoderTarget == &horizEncoderTarget)?horizTargetClose : grabTargetLow);
				writeDebugStreamLine("Encoder target set to LOW");
			}
			else if(upOrDown == 'd')
			{
				writeDebugStreamLine("Encoder target cannot be set any lower");
			}
			break;
		default:
			break;
	}
	wait10Msec(25);
}

/*
*	checkButtons
*	Watch the buttons and make changes accordingly
*/
bool checkingButtons = true;
task checkButtons()
{
	// Store the desired positions for the horizontal and vertical lifts
	// Initialize them to their starting positions
	char liftPosition 	= 'b';
	char horizPosition 	= 'b';
	char grabPosition 	= 'b';

	writeDebugStreamLine("Button checker activated");
	
	while(checkingButtons)
	{
		// When a button is pressed, switch the variable attached to that function.
		
		if(buttonLiftUp)
		{
			writeDebugStreamLine("Switching vertical lift encoder target");
			switchEncoderTarget(&liftEncoderTarget, &liftPosition, 'u');
		}
		else if(buttonLiftDown)
		{
			writeDebugStreamLine("Switching vertical lift encoder target");
			switchEncoderTarget(&liftEncoderTarget, &liftPosition, 'd');
		}

		if(buttonLiftOut)
		{
			writeDebugStreamLine("Switching horizontal lift encoder target");
			switchEncoderTarget(&horizEncoderTarget, &horizPosition, 'u');
		}
		else if(buttonLiftIn)
		{
			writeDebugStreamLine("Switching horizontal lift encoder target");
			switchEncoderTarget(&horizEncoderTarget, &horizPosition, 'd');
		}

		if(buttonGrabUp)
		{
			writeDebugStreamLine("Switching grabber encoder target");
			switchEncoderTarget(&grabEncoderTarget, &grabPosition, 'u');
		}
		else if(buttonGrabDown)
		{
			writeDebugStreamLine("Switching grabber encoder target");
			switchEncoderTarget(&grabEncoderTarget, &grabPosition, 'd');
		}
	}
	
	writeDebugStreamLine("Button checker deactivated");
}
