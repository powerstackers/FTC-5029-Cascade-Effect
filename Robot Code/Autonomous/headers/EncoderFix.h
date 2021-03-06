/*
*	EncoderFix.h
*	Code to watch motor encoders, and stop the motors if they stall.
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
*	February 10 2015
*	Version 1.0
*/

// Include a file with some functions that we will need
#include "AutoFunctions.h"

// The total number of motors available to the robot (based on number of controllers attached)
#define numMotors 4

/*
*	watchMotors
*	While all motors are running, watch their encoders. If the motor gets stuck, stop all motors
*	and back the affected motor up a short distance.
*/
task watchMotors ()
{
	// Store all the previous encoder values for the motors
	long prevEncoderValue[numMotors];

	// The threshold distance. If the encoder changes by less than this much in a rotation, stop the motor
	long encoderThreshold = 300;

	// Load the prevEncoderValue array with the encoders' current values
	for(short i = 0; i<numMotors; i++)
	{
		prevEncoderValue[(tMotor)i] = nMotorEncoder[(tMotor)i];
	}

	// Loop forever, or until this process is killed externally
	while(true)
	{
		// Loop through every available motor
		for(short i = 0; i<numMotors; i++)
		{
			// If the motor speed is set to anything but 0, and the motor encoder value has changed by less than
			// the acceptable threshold since the last loop, stop and reverse the motor
			if(motor[(tMotor)i]!=0 && ((long) abs(prevEncoderValue[(tMotor)i]-nMotorEncoder[(tMotor)i])) > encoderThreshold)
			{

				writeDebugStreamLine("-- HALTED MOTOR DETECTED --\nStopping motor, and reversing");

				// Store the desired speed (also stores the direction)
				short motorSpeed = motor [(tMotor)i];

				// Halt all other tasks
				hogCPU();

				// Turn off all motors
				allMotorsTo(0);

				// If the motor is one of the drive motors, reverse both of the drive motors.
				if((tMotor)i==mDriveLeft || (tMotor)i==mDriveRight)
				{
					writeDebugStreamLine("Drive motors affected. Moving both back for 200 ms");
					// Set both motors to the reverse of the original speed
					driveMotorsTo(-motorSpeed);
					// Wait 200 ms
					//wait10Msec(20);
					// Turn the motors back off
					driveMotorsTo(motorSpeed);
				}
				// For all motors besides the drive motors
				else
				{
					writeDebugStreamLine("Moving motor %d back for 200 ms");
					// Set the affected motor to the reverse of its original speed
					motor[(tMotor)i] = -motorSpeed;
					// Wait 200 ms
					//wait10Msec(20);
					// Turn off the affected motor
					motor[(tMotor)i] = motorSpeed;
				}

				// Allow other tasks to run again
				releaseCPU();
				writeDebugStreamLine("-- RELEASED HALTED MOTOR --");
			}	// End of chekc
		}	// End of for loop
	}	// End of main loop
}	// END
