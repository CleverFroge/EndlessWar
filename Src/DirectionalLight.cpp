#include "FrogEngine.h"
using namespace FrogEngine;


DirectionalLight* DirectionalLight::Create(Vector3 direction, Vector3 color)
{
	DirectionalLight* light = new DirectionalLight();
	light->direction = direction;
	light->color = color;
	return light;
}

DirectionalLight::DirectionalLight()
{
}

DirectionalLight::~DirectionalLight()
{
}