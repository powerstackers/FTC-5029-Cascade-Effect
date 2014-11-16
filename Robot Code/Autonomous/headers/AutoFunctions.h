
/*
*
*	FUNCTION PROTOTYPES
*
*/
void initializeRobot();
task autonomous();

void allMotorsTo(int i)
{
		motor[mDriveLeft] = i;
		motor[mDriveRight] = i;
		motor[mBrush] = i;
		motor[mLift] = i;
}

void initializeRobot()
{

}

int ultrasonicThreshold = 30;

task avoidCollision()
{
	while(true)
	{
		while(SensorValue[sUltraBack] < ultrasonicThreshold || SensorValue[sUltraFront] < ultrasonicThreshold)
		{
			allMotorsTo(0);
			nxtDisplayCenteredBigTextLine(1, "STOP");
			hogCPU();
		}
		releaseCPU();

		//nxtDisplayTextLine(2, "%s", (SensorValue[sUltraBack] < ultrasonicThreshold)? "TRUE" : "FALSE");
		//nxtDisplayTextLine(3, "%s", (SensorValue[sUltraFront] < ultrasonicThreshold)? "TRUE" : "FALSE");

	}
}
