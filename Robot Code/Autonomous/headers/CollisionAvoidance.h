#pragma config(StandardModel, "PS CASCADE EFFECT")

/*
*	CollisionAvoidance.h
*	Functions and tasks to avoid on-field collisions.
*
*	THIS CODE IS PROVIDED AS-IS AND WITHOUT WARRANTY.
*	THIS CODE IS OPEN FOR DISTRIBUTION AND MODIFICATION.
*
*	FTC Team #5029, The Powerstackers
*	powerstackersftc.com
*	github.com/powerstackers
*/

#include "Sensors.h"

/*
*	GLOBAL CONSTANTS
*/
// Detecting an ultrasonic sensor value below this threshold will cause the robot to stop
#define ultrasonicThreshold 30

/*
*	FUNCTION PROTOTYPES
*/
task avoidCollision();
void detour();

/*
*	detour
*	Use the gyro sensor and encoders to clear an obstruction.
*/
void detour(){

}

/*
*	avoidCollision
*	Actively watch the ultrasonic sensors to prevent a collision
*/
bool avoidanceActive = false;
task avoidCollision()
{
	avoidanceActive = true;
	writeDebugStreamLine("--COLLISION AVOIDANCE ACTIVATED--")
	// Loop until the switch is thrown
	while(avoidanceActive)
	{
		// If the ultrasonic sensors detect an obstruction
		if(ultraStrengthBack < ultrasonicThreshold || ultraStrengthFront < ultrasonicThreshold)
		{
			// Stop all motors
			allMotorsTo(0);
			// Stop all processes except this one (temporarily stops all other functions)
			hogCPU();

			// Print a STOP message to the NXT screen and debug stream
			writeDebugStreamLine("--!! OBSTRUCTION DETECTED !!--");
			nxtDisplayCenteredBigTextLine(2, "SUSPENDED");
			nxtDisplayCenteredTextLine(6, "--!!COLLISION!!--");
			nxtDisplayCenteredTextLine(7, "--!!DETECTED!!--");

			// Wait until the ultrasonic sensors are clear
			//while(ultraStrengthBack < ultrasonicThreshold || ultraStrengthFront < ultrasonicThreshold){}
			
			// Go around the obstruction
			detour();
			
			// Start up all other processes again
			releaseCPU();

			// Write messages to the nxt and the debug stream
			writeDebugStreamLine("--!! OBSTRUCTION CLEARED !!--");
			nxtDisplayCenteredBigTextLine(2, "RUNNING");
			nxtDisplayCenteredTextLine(6, "--OBSTRUCTION--");
			nxtDisplayCenteredTextLine(7, "--CLEARED--");
		}
	}
	
	writeDebugStreamLine("--COLLISION AVOIDANCE DEACTIVATED--");
}