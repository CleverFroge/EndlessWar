#include "FrogeEngine.h"
using namespace FrogeEngine;

float FrogeEngine::Radians(float angle)
{
	return glm::radians(angle);
}

float FrogeEngine::Angle(float radians)
{
	return radians / PI * 180.0;
}

float FrogeEngine::NormalizedAngle(float angle)
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

float FrogeEngine::GetAttenuationLinear(float distant)
{
	return 0;
}

float FrogeEngine::GetAttenuationQuadratic(float distant)
{
	return 0;
}
