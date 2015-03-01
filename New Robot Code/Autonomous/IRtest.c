#pragma config(StandardModel, "PS CASCADE EFFECT")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
*	IRtest.c
*	Functional test for the IR detector. Finds which of three positions the center goal is facing.
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
*	December 31 2014
*	Version 0.1
*/

#include "headers/Sensors.h"

#define positionA 80
#define positionB 95
#define positionC 70

task main()
{
	StartTask(getSmux);
	gettingIr=true;
	wait10Msec(35);

	int avg = ((irStrengthLeft + irStrengthRight) / 2);
	int diffA = abs(avg - positionA);
	int diffB = abs(avg - positionB);
	int diffC = abs(avg - positionC);
	char facing;

	if(diffA < diffB && diffA < diffC)
		facing = 'a';
	else if(diffB < diffA && diffB < diffC)
		facing = 'b';
	else
		facing = 'c';
	writeDebugStreamLine("Left:\t\t%d", irStrengthLeft);
	writeDebugStreamLine("Right:\t\t%d", irStrengthRight);
	writeDebugStreamLine("Average:\t%d", avg);
	writeDebugStreamLine("The thing is in position %c", facing);

	StopAllTasks();
}