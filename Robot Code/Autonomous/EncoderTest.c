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

#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     mDriveLeft,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     mDriveRight,   tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	while(true)
	{
		nxtDisplayTextLine(0, "%d", nMotorEncoder[mDriveLeft]);
	}
}
