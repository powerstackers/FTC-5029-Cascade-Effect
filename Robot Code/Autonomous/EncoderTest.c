#pragma config(StandardModel, "PS CASCADE EFFECT")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
*	EncoderTest.c
*	Code used to test the encoders. Used for testing purposes only.
*
*	THIS CODE IS PROVIDED AS-IS AND WITHOUT WARRANTY.
*	THIS CODE IS OPEN FOR DISTRIBUTION AND MODIFICATION.
*
*	FTC Team #5029, The Powerstackers
*	powerstackersftc.weebly.com
*	github.com/powerstackers
*/

task main()
{
	nMotorEncoder[mDriveLeft] = 0;
	motor[mDriveLeft] = -100;

	while(nMotorEncoder[mDriveLeft] < 1120) // While encoder ticks less than 1120 (full rotation)
	{
		nxtDisplayTextLine(0, "%d", nMotorEncoder[mDriveLeft]);
	}

	motor[mDriveLeft] = 0;
}