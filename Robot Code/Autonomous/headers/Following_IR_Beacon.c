/*
*	Following_IR_Beacon.c
*	Code for the mini-bot, used for demonstrations and outreaches.
*	The robot will follow an IR beacon around indefinitely. The kids love it!
*
*	THIS CODE IS PROVIDED AS-IS AND WITHOUT WARRANTY.
*	THIS CODE IS OPEN FOR DISTRIBUTION AND MODIFICATION
*
*	FTC Team #5029, The Powerstackers
*	powerstackersftc.weebly.com
*	github.com/powerstackers
*	December 31 2014
*	Version 0.1
*/

#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     HTIRS2,         sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     motorC,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorB,        tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "hitechnic-irseeker-v2.h"

// main task
task main ()
{
 int _dirAC = 0;
 int acS1, acS2, acS3, acS4, acS5 = 0;

 int maxSig = 0;    // the max signal strength from the seeker.
 int val = 0;       // the translated directional value.

 // we are going to set DSP mode to 1200 Hz.
  tHTIRS2DSPMode _mode = DSP_1200;

  // attempt to set to DSP mode.
  if (HTIRS2setDSPMode(HTIRS2, _mode) == 0)
  {
    // unsuccessful at setting the mode.
    // display error message.
    eraseDisplay();
    nxtDisplayCenteredTextLine(0, "ERROR!");
    nxtDisplayCenteredTextLine(2, "Init failed!");
    nxtDisplayCenteredTextLine(3, "Connect sensor");
    nxtDisplayCenteredTextLine(4, "to Port 1.");

    // make a noise to get their attention.
    PlaySound(soundBeepBeep);

    // wait so user can read message, then leave main task.
    wait10Msec(300);
    return;
  }

  eraseDisplay();

  // loop continuously and read from the sensor.
  while( true )  //while(maxSig<200)
  {
    // read the current modulated signal direction
    _dirAC = HTIRS2readACDir(HTIRS2);
    if (_dirAC < 0)
    {
      // error! - write to debug stream and then break.
      writeDebugStreamLine("Read dir ERROR!");
      break;
    }

    // Get the AC signal strength values.
    if (!HTIRS2readAllACStrength(HTIRS2, acS1, acS2, acS3, acS4, acS5 ))
    {
      // error! - write to debug stream and then break.
      writeDebugStreamLine("Read sig ERROR!");
      break;
    }  else {
      // find the max signal strength of all detectors.
      maxSig = (acS1 > acS2) ? acS1 : acS2;
      maxSig = (maxSig > acS3) ? maxSig : acS3;
      maxSig = (maxSig > acS4) ? maxSig : acS4;
      maxSig = (maxSig > acS5) ? maxSig : acS5;
    }

    // display info
    nxtDisplayCenteredBigTextLine(1, "Dir=%d", _dirAC);
    nxtDisplayCenteredBigTextLine(4, "Sig=%d", maxSig);

    // first translate directional index so 0 is straight ahead.
    val = _dirAC - 5;

    // If the beacon is within a few centimeters of the detector, stop the robot
	if(maxSig > 200)
    {
    	motor[motorC] = 0;
    	motor[motorB] = 0;
    }

    // Turn the robot to the left
    else if (_dirAC > 5)
	{
		motor[motorC]=30;
		motor[motorB]=0;
	}
    // Turn the robot to the right
    else if (_dirAC < 5)
	{
		motor[motorC]=0;
		motor[motorB]=30;
	}

    // Just drive straight
    else
   	{
   		motor[motorC]=50;
   		motor[motorB]=50;
   	}

   	// If the beacon can't be found, spin in a circle until you find it again
   	if(maxSig < 20)
   	{
   		motor[motorB] = 30;
   		motor[motorC] = -30;
   	}
  }
}
