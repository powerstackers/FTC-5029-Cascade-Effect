#pragma config(StandardModel, "PS CASCADE EFFECT")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
*	SensorsTest.c
*	Shows sensor values on the NXT screen.
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
*	January 2 2015
*	Version 0.1
*/


#include "headers/Sensors.h"

task main()
{
	//StartTask(getSmux);
	while(true)
	{
		nxtDisplayTextLine(0, "UsonFr: %d", getUltraStrength(ultraFront));
		nxtDisplayTextLine(1, "UsonBa: %d", getUltraStrength(ultraBack));
		nxtDisplayTextLine(2, "IR: %d", getIRStrength(infraRed));
	}
}
