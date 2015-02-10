#pragma config(StandardModel, "PS CASCADE EFFECT")

/*
*	GyroPath.h
*	Tasks to keep the robot moving in a straight line. Uses the gyroscope sensor to correct the robot's heading.
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
*	February 7 2015
*	Version 1.0
*/

// Inlcude guard. Ensure that this file is only included once.
#pragma once

// Include a file to handle the gyroscope sensor information
#include "Sensors.h"

/*
*	stablizePath
*	While a robot is moving, ensure that it stays moving in a straight line.
*/
task stablizePath ()
{
	// Store the original speed of the motors. We may modify the speed later
	short defaultMotorSpeed = motor[mDriveLeft];
	// Create a variable to store the current heading
	float currentHeading = 0.0;
	// The amount to change the motor speed by
	//short correctionAmount = 25;
	// Threshold outside which we say that the robot has turned too far
	float threshold = 10.0;

	// Take an initial reading from the gryo sensor
	const float initialTurnReading = currentGryoReading();

	// Output some debug information
	writeDebugStreamLine("-- GYRO PATH ENABLED --\n\tStarting heading: %2.2f", currentHeading);
	writeDebugStreamLine("\tDefault speed: %d", defaultMotorSpeed);

	ClearTimer(T1);

	// Loop until this task is stopped
	while(true)
	{
		// If the current heading is outside the threshold, modify the motor speed
		if (abs(currentHeading)>threshold)
		{
			PlaySound(soundDownwardTones);
			/*
			if(time100[T1]%10==0)
				writeDebugStreamLine("CORRECTING; CURR HEADING: %2.2f", currentHeading);
			*/

			// If the current heading is above 0 (the robot has turned counterclockwise)
			if(currentHeading>0)
			{
				// If the motor is meant to be moving forwards
				if(defaultMotorSpeed<0)
				{
					// Lower the speed of the right motor, making the robot turn clockwise
					motor[mDriveRight] = 0;//defaultMotorSpeed - correctionAmount;
				}
				// If the motor is meant to be moving backwards
				else
				{
					// Raise the speed of the left motor, making the robot turn clockwise
					motor[mDriveLeft] = 0;//defaultMotorSpeed + correctionAmount;
				}
			}

			// If the current heading is below 0 (the robot has turned counterclockwise)
			else
			{
				// If the motor is meant to be moving forwards
				if(defaultMotorSpeed<0)
				{
					// Lower the speed of the left motor, making the robot turn counterclocwise
					motor[mDriveLeft] = 0;//defaultMotorSpeed - correctionAmount;
				}
				// If the motor is meant to be moving backwards
				else
				{
					// Riase the speed of the right motor, making the robot turn counterclockwise
					motor[mDriveRight] = 0;//defaultMotorSpeed + correctionAmount;
				}
			}
		}

		// If the current heading is within the threshold, set both the motors to their default values
		else
		{
			ClearSounds();
			motor [mDriveLeft] = defaultMotorSpeed;
			motor [mDriveRight] = defaultMotorSpeed;
		}

		/*
		if(time100[T1]%10==0)
		{
			writeDebugStream("\nHEADING: %2.2f ", currentHeading);
			writeDebugStream("RIGHT: %d ", motor[mDriveRight]);
			writeDebugStream("LEFT: %d", motor[mDriveLeft]);
		}
		*/

		// Update the current heading
		currentHeading += (currentGryoReading() - initialTurnReading) * 0.01;
		// Wait 10 milliseconds before looping again
		wait10Msec(1);
	}

	writeDebugStreamLine("\tEnding heading: %2.2f", currentHeading);
}
