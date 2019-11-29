#include "FrogEngine.h"
using namespace FrogEngine;

std::vector<PointLight*> PointLight::_lights;

PointLight* PointLight::Create(Vector3 position, Vector3 color, float range)
{
	if (_lights.size() >= MaxInstance)
	{
		return nullptr;
	}
	PointLight* light = new PointLight();
	light->position = position;
	light->color = color;
	light->constant = 1;
	light->linear = 0.09;
	light->quadratic = 0.032;
	_lights.push_back(light);
	return light;
}

const std::vector<PointLight*>& PointLight::GetLights()
{
	return _lights;
}

void PointLight::Release(PointLight* light)
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

PointLight::PointLight()
{
}

PointLight::~PointLight()
{
}