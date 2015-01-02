/*
*	Sensors.h
*	Code to handle all the sensors during the autonomous period.
*	Copyright (C) 2015 Jonathan Thomas
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
*	January 2 2015
*	Version 0.2
*/

#pragma once

// Include files to handle the multiplexer and all the different sensors
#include "hitechnic-sensormux.h"
#include "hitechnic-irseeker-v2.h"
#include "hitechnic-accelerometer.h"
#include "hitechnic-gyro.h"
#include "lego-ultrasound.h"

// Macros to store the sensor addresses
// Sensor addresses may change throughout the season
#define irLeft  	msensor_S2_1		// IR Sensor, NXT 2 MUX 1
#define irRight  	msensor_S2_2 		// IR Sensor, NXT 2 MUX 2

#define infraRed	msensor_S2_1		// Lone IR sensor, NXT 2 MUX 1

#define ultraBack	msensor_S2_3		// Ultrasonic, NXT 2 MUX 3
#define ultraFront	msensor_S2_4		// Ultrasonic, NXT 2 MUX 4
#define ultraTube	msensor_S2_2		// Ultrasonic, NXT 2 MUX 2

#define sGyro		S3					// Gyroscope, NXT 3

// Variables to store the sensor values
int irStrengthLeft;
int irDirectionLeft;
int irStrengthRight;
int irDirectionRight;

int irStrength;
int irDirection;

int ultraStrengthBack;
int ultraStrengthFront;
int ultraStrengthTube;

int accelX = 0;
int accelY = 0;
int accelZ = 0;


// Flag to turn on or off the IR seekers
bool gettingIr 		= false;
bool gettingSmux 	= false;

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
	int strength;
	int acS1, acS2, acS3, acS4, acS5 = 0;

	HTIRS2readAllACStrength(sensor, acS1, acS2, acS3, acS4, acS5 );

	strength = (acS1 > acS2) ? acS1 : acS2;
	strength = (strength > acS3) ? strength : acS3;
	strength = (strength > acS4) ? strength : acS4;
	strength = (strength > acS5) ? strength : acS5;

	return 	strength;
}

/*
*	getIREnhanced
*	Get both the direction and the strength of the chosen IR seeker
*/
void getIREnhanced(tMUXSensor sensor)
{
	if(sensor == irLeft)
		HTIRS2readEnhanced(sensor, irDirectionLeft, irStrengthLeft);
	else if(sensor == irRight)
		HTIRS2readEnhanced(sensor, irDirectionRight, irStrengthRight);
}

/*
*	getIREnhanced
*	Get both the direction and the strength of the chosen IR seeker
*/
void getIREnhanced(tSensors sensor)
{
	if(sensor == irLeft)
		HTIRS2readEnhanced(sensor, irDirectionLeft, irStrengthLeft);
	else if(sensor == irRight)
		HTIRS2readEnhanced(sensor, irDirectionRight, irStrengthRight);
}


/*
*	IR detection functions for the lone IR seeker. No arguments.
*
*/
int getIRDirection(){
	return HTIRS2readACDir(infraRed);
}

int getIRStrength(){
	getIRStrength(infraRed);
}

/*
*	getAccelOrientation
*	Get the accelerometer orientation on all axes
*/
void getAccelOrientation(tMUXSensor sensor)
{
	HTACreadAllAxes(sensor, accelX, accelY, accelZ);
}

/*
*	getAccelOrientation
*	Get the accelerometer orientation on all axes
*/
void getAccelOrientation(tSensors sensor)
{
	HTACreadAllAxes(sensor, accelX, accelY, accelZ);
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
*	getSmux
*	Update all the sensors attached to a multiplexer
*/
task getSmux()
{
	gettingSmux = true;
	// Print a ready message
	writeDebugStreamLine("-- MULTIPLEXER ACTIVATED --");

	// Loop until the switch is pulled
	while (gettingSmux){
		// Only update the IR seeker variables when the IR seekers are turned on
		// This keeps the debug stream clear
		if(gettingIr){
			getIREnhanced(irLeft);
			getIREnhanced(irRight);
		}

		// Store the values of the ultrasonic sensors
		ultraStrengthBack 	= USreadDist(ultraBack);
		ultraStrengthFront 	= USreadDist(ultraFront);
		ultraStrengthTube 	= USreadDist(ultraTube);
	}
	
	writeDebugStreamLine("-- MULTIPLEXER DEACTIVATED --");
}
