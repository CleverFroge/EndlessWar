#include "FrogEngine.h"
using namespace FrogEngine;

float Time::DeltaTime = 0;
float Time::CurrentTime = 0;

void Time::Update()
{
	if (CurrentTime==0)
	{
		CurrentTime = glfwGetTime();
	}
	float time = glfwGetTime();
	DeltaTime = time - CurrentTime;
	CurrentTime = time;
}

float Time::GetTime()
{
	return CurrentTime;
}

float Time::GetDeltaTime()
{
	return DeltaTime;
}