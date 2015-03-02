#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S2,     tmux,           sensorHiTechnicTouchMux)
#pragma config(Sensor, S3,     smux,           sensorI2CCustom)
#pragma config(Sensor, S4,     gryo,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  mtr_S1_C1_1,     m1,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     m2,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     m3,            tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     m4,            tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
*	MotorControllerTest.c
*	Program to test motor controllers. Contains three motor controllers that all get flashed
*	at various power levels, one after the other.
**	Copyright (C) 2015 Powerstackers
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

task main()
{
	servo[servo1] = 10;
	servo[servo2] = 32;
	writeDebugStreamLine("%d", servo1);
	writeDebugStreamLine("%d", servo2);
	wait10Msec(200);
}
