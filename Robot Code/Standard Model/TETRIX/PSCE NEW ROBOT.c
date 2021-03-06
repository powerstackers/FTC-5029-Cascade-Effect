#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S2,     SMUX1,          sensorI2CCustom)
#pragma config(Sensor, S3,     touchLiftStop,  sensorTouch)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          mGrab,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          mBrush,        tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     mDriveLeft,    tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     mDriveRight,   tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     mLift,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    rTrapDoor,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    rFlapLeft,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    rFlapRight,           tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    rBallDoor,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
*	PSCE NEW ROBOT.c
*	Motor and sensor definitions for our 2014-15 robot, version 2.
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
*	January 16 2015
*	Version 0.2
*/
