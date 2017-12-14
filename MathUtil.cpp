#include"MathUtil.h"
#include<math.h>

float wrapPi(float theta)
{
	theta += PI;
	theta -= floor(theta*ReciprocalPI)*TwoPI;
	theta -= PI;
	return theta;
}

float safeAcos(float x)
{
	if (x > 1.0f)
	{
		return PI;
	}
	if (x < -1.0f)
	{
		return 0.0f;
	}
	return acos(x);
}