#include "FrogEngine.h"
using namespace FrogEngine;

FlashLight* FlashLight::Create(Vector3 position, Vector3 direction, Vector3 color, float innerCone, float outerCone)
{
	FlashLight* light = new FlashLight();
	light->position = position;
	light->direction = direction;
	light->color = color;
	light->innerCone = cos(Radians(innerCone));
	light->outerCone = cos(Radians(outerCone));
	light->constant = 1;
	light->linear = 0.09;
	light->quadratic = 0.032;
	return light;
}

FlashLight::FlashLight()
{
}

FlashLight::~FlashLight()
{
}