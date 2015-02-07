task stablizePath ()
{
	short defaultMotorSpeed = mDriveLeft;
	float currentHeading = 0.0;
	short correctionAmount = 10;
	while(true)
	{
		currentHeading += currentGryoReading() * 0.01;
		if (abs(currentHeading)>threshold)
		{
			if(currentHeading>0)
			{
				motor[mDriveRight] = desiredMotorSpeed - correctionAmount;
			}
			else
			{
				motor [mDriveLeft] = desiredMotorSpeed - correctionAmount;
			}
		}
		else
		{
			motor [mDriveLeft] = defaultMotorSpeed;
			motor [mDriveRight] = defaultMotorSpeed;
		}
	}
}
