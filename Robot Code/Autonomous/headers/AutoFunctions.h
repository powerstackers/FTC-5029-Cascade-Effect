#include "Multiplexer.h"

/*
*
*	FUNCTION PROTOTYPES
*
*/
void initializeRobot();
task autonomous();

/*
*	allMotorsTo
*	Set all the motors to the input value
*/
void allMotorsTo(int i)
{
		motor[mDriveLeft] 	= i;
		motor[mDriveRight] 	= i;
		motor[mBrush] 		= i;
		motor[mLift] 		= i;
}

/*
*	initializeRobot
*	Move all motors and servos to their starting positions
*/
void initializeRobot()
{

}

// ultrasonicThreshold
// Detecting an ultrasonic sensor value below this threshold will cause the robot to stop
int ultrasonicThreshold = 30;

/*
*	avoidCollision
*	Actively watch the ultrasonic sensors to prevent a collision
*/
task avoidCollision()
{
	// Loop forever
	while(true)
	{
		// For as long as one or both ultrasonic sensors read below the threshold
		while(ultraStrengthBack < ultrasonicThreshold || ultraStrengthFront < ultrasonicThreshold)
		{
			// Stop all motors
			allMotorsTo(0);

			// Print a STOP message to the NXT screen
			nxtDisplayCenteredBigTextLine(1, "STOP");

			// Stop all processes except this one (temporarily stops all other functions)
			hogCPU();
		}

		// Start up all other processes again
		releaseCPU();

		//nxtDisplayTextLine(2, "%s", (SensorValue[sUltraBack] < ultrasonicThreshold)? "TRUE" : "FALSE");
		//nxtDisplayTextLine(3, "%s", (SensorValue[sUltraFront] < ultrasonicThreshold)? "TRUE" : "FALSE");

	}
}
