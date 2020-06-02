#include "FrogEngine.h"
using namespace FrogEngine;

PointLight* PointLight::Create(Vector3 position, Vector3 color, float range)
{
	PointLight* light = new PointLight();
	light->position = position;
	light->color = color;
	light->constant = 1;
	light->linear = 0.09;
	light->quadratic = 0.032;
	return light;
}

PointLight::PointLight()
{
}

PointLight::~PointLight()
{
}