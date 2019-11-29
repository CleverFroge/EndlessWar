#include "FrogEngine.h"
using namespace FrogEngine;

float FrogEngine::Radians(float angle)
{
	return glm::radians(angle);
}

float FrogEngine::Angle(float radians)
{
	return radians / PI * 180.0;
}

float FrogEngine::NormalizedAngle(float angle)
{
	int multiplePI = angle / 180;
	if (multiplePI % 2 == 0)
	{
		return angle-multiplePI*180;
	}
	else
	{
		return angle-(multiplePI + 1)*180;
	}
}

float FrogEngine::GetAttenuationLinear(float distant)
{
	return 0;
}

float FrogEngine::GetAttenuationQuadratic(float distant)
{
	return 0;
}
