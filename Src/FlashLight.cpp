#include "FrogEngine.h"
using namespace FrogEngine;

std::vector<FlashLight*> FlashLight::_lights;

FlashLight* FlashLight::Create(Vector3 position, Vector3 direction, Vector3 color, float innerCone, float outerCone)
{
	if (_lights.size() >= MaxInstance)
	{
		return nullptr;
	}
	FlashLight* light = new FlashLight();
	light->position = position;
	light->direction = direction;
	light->color = color;
	light->innerCone = cos(Radians(innerCone));
	light->outerCone = cos(Radians(outerCone));
	light->constant = 1;
	light->linear = 0.09;
	light->quadratic = 0.032;
	_lights.push_back(light);
	return light;
}

const std::vector<FlashLight*>& FlashLight::GetLights()
{
	return _lights;
}

void FlashLight::Release(FlashLight* light)
{
	if (!light)
	{
		return;
	}
	auto it = _lights.begin();
	while (it != _lights.end())
	{
		if (*it == light)
		{
			it = _lights.erase(it);
			delete light;
		}
		else
		{
			it++;
		}
	}
}

FlashLight::FlashLight()
{
}

FlashLight::~FlashLight()
{
}