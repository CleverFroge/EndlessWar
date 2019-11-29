#include "FrogEngine.h"
using namespace FrogEngine;

std::vector<DirectionalLight*> DirectionalLight::_lights;

DirectionalLight* DirectionalLight::Create(Vector3 direction, Vector3 color)
{
	if (_lights.size()>= MaxInstance)
	{
		return nullptr;
	}
	DirectionalLight* light = new DirectionalLight();
	light->direction = direction;
	light->color = color;
	_lights.push_back(light);
	return light;
}

const std::vector<DirectionalLight*>& DirectionalLight::GetLights()
{
	return _lights;
}

void DirectionalLight::Release(DirectionalLight* light)
{
	if (!light)
	{
		return;
	}
	auto it = _lights.begin();
	while (it!=_lights.end())
	{
		if (*it==light)
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

DirectionalLight::DirectionalLight()
{
}

DirectionalLight::~DirectionalLight()
{
}