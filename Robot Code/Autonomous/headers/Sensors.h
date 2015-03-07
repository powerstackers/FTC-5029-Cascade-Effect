/*
*	Sensors.h
*	Code to handle all the sensors during the autonomous period.
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
*	March 1 2015
*	Version 2.1
*/

// Include guard. This file can only be included one time.
#pragma once

// Include files to handle the multiplexer and all the different sensors
#include "../../drivers/hitechnic-sensormux.h"
#include "../../drivers/hitechnic-touchmux.h"
#include "../../drivers/hitechnic-irseeker-v2.h"
#include "../../drivers/hitechnic-accelerometer.h"
#include "../../drivers/hitechnic-gyro.h"
#include "../../drivers/lego-ultrasound.h"

// Macros to store the sensor addresses
// Sensor addresses may change throughout the season
#define infraRed	msensor_S2_1		// IR sensor, NXT 2 MUX 1
#define sGyro		msensor_S2_2		// Gyroscope sensor, NXT 2 MUX 2
#define infraRedSide	msensor_S2_4		// IR sensor, NXT 2 MUX 2

// DEPRECATED
#define ultraBack	msensor_S2_3		// Ultrasonic, NXT 2 MUX 3
#define ultraFront	msensor_S2_2		// Ultrasonic, NXT 2 MUX 4

// Touch sensors, used in teleop and initialization
#define touchLiftStop	1
#define touchTipStop	2

//#define sGyro		S3					// Gyroscope, NXT 3

/*
*	FUNCTION PROTOTYPES
*/
int 	getIRDirection(tMUXSensor sensor);
int 	getIRStrength(tMUXSensor sensor);
int 	getIRDirection();
int 	getIRStrength();
float 	currentGryoReading();
float 	getUltraStrength(tMUXSensor sensor);
bool 	touchActive(int sensor);


/*
*	getIRDirection
*	Gets the overall direction of the chosen IR seeker
*/
int getIRDirection(tMUXSensor sensor)
{
	return HTIRS2readACDir(sensor);
}

/*
*	getIRStrength
*	Gets the overall strength of the chosen IR seeker
*/
int getIRStrength(tMUXSensor sensor)
{
	// Declare variables to store the overall strength and the strengths of each individual detector
	int strength;
	int acS1, acS2, acS3, acS4, acS5 = 0;

	// Read the strength of all the detectors
	HTIRS2readAllACStrength(sensor, acS1, acS2, acS3, acS4, acS5 );

	// Figure out which detector had the highest reading, and set that to the overall strength
	strength = (acS1 > acS2) ? acS1 : acS2;
	strength = (strength > acS3) ? strength : acS3;
	strength = (strength > acS4) ? strength : acS4;
	strength = (strength > acS5) ? strength : acS5;

	// Return the overall strength
	return 	strength;
}

/*
*	IR detection functions for the lone IR seeker. No arguments.
*
*/
int getIRDirection()
{
	return HTIRS2readACDir(infraRed);
}

int getIRStrength()
{
	return getIRStrength(infraRed);
}

/*
*	currentGryoReading
*	Return the current angular velocity as measure by the gyroscope
*/
float currentGryoReading()
{
	return HTGYROreadRot(sGyro);
}

/*
*	getUltraStrength
*	Return the strength of the given ultrasonic sensor
*/
int getUltraStrength(tMUXSensor sensor)
{
	return USreadDist(sensor);
}

/*
*	touchActive
*	Returns the state of a given touch sensor
*/
bool touchActive(int sensor)
{
	return HTTMUXisActive(TMUX1, sensor);
}

/*
*	initializeMultiplexers
*	Initialize all the multiplexers and get them ready to run
*/
void initializeMultiplexers()
{

}
