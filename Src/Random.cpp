#pragma once
#include "FrogEngine.h"
using namespace FrogEngine;

float Random::Randf(float min, float max)
{
	float delta = max - min;
	float rand_01 = (float)(rand() % 10000) / 10000;
	return rand_01 * delta + min;
}