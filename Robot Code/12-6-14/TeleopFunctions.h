/*
*	TeleopFunctions.h
*	Functions and subroutines for use during the teleop period.
*
*	THIS CODE IS PROVIDED AS-IS AND WITHOUT WARRANTY.
*	THIS CODE IS OPEN FOR DISTRIBUTION AND MODIFICATION.
*
*	FTC Team #5029, The Powerstackers
*	powerstackersftc.weebly.com
*	github.com/powerstackers
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


#define liftTargetBase 1
#define liftTargetLow 2
#define liftTargetMed 3
#define liftTargetHigh 4
#define liftTargetCent 5

#define horizTargetBase 1
#define horizTargetClose 2
#define horizTargetMed 3
#define horizTargetFar 4

#define grabTargetBase 1
#define grabTargetLow 2
#define grabTargetMed 3
#define grabTargetHigh 4

// Encoder target for the vertical lift and horizontal lift
long liftEncoderTarget = 0;
long horizEncoderTarget = 0;
long grabEncoderTarget = 0;


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
	nxtDisplayTextLine(5, "p2TopHat:%d", joystick.joy2_TopHat);
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



void switchEncoderTarget(long* encoderTarget, char* currentPosition, char upOrDown = 'u')
{

	switch(*currentPosition)
	{
		case 'c':
			if(upOrDown != 'u')
			{
				*currentPosition = 'h';
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetHigh : ((encoderTarget == &horizEncoderTarget)?horizTargetFar : grabTargetHigh);
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
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetMed : ((encoderTarget == &horizEncoderTarget)?horizTargetMed : grabTargetMed);
				writeDebugStreamLine("Encoder target set to MED");
			}
			break;
		case 'm':
			if(upOrDown == 'u')
			{
				*currentPosition = 'h';
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetHigh : ((encoderTarget == &horizEncoderTarget)?horizTargetFar : grabTargetHigh);
				writeDebugStreamLine("Encoder target set to HIGH");
			}
			else if(upOrDown == 'd')
			{
				*currentPosition = 'l';
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetLow : ((encoderTarget == &horizEncoderTarget)?horizTargetClose : grabTargetLow);
				writeDebugStreamLine("Encoder target set to LOW");
			}
			break;
		case 'l' :
			if(upOrDown == 'u')
			{
				*currentPosition = 'm';
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetMed : ((encoderTarget == &horizEncoderTarget)?horizTargetMed : grabTargetMed);
				writeDebugStreamLine("Encoder target set to MED");
			}
			else if(upOrDown == 'd')
			{
				*currentPosition = 'b';
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetBase : ((encoderTarget == &horizEncoderTarget)?horizTargetBase : grabTargetBase);
				writeDebugStreamLine("Encoder target set to BASE");
			}
			break;
		case 'b' :
			if(upOrDown == 'u')
			{
				*currentPosition = 'l';
				*encoderTarget = (encoderTarget == &liftEncoderTarget)? liftTargetLow : ((encoderTarget == &horizEncoderTarget)?horizTargetClose : grabTargetLow);
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
*
*	Watch the buttons and make changes accordingly
*/
task checkButtons()
{
	// Store the desired positions for the horizontal and vertical lifts
	char liftPosition = 'b';
	char horizPosition = 'b';
	char grabPosition = 'b';

	while(true)
	{

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
}
