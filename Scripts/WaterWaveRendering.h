#pragma once
#include "FrogEngine.h"
using namespace FrogEngine;

class WaterWaveRendering:public Component
{
public:
	WaterWaveRendering() : Component{ "WaterWaveRendering" }
	{
	}

	~WaterWaveRendering()
	{
	}

	void Awake()
	{

	}

	void Update()
	{
		Shader* shader = _node->Find("MountainSkybox")->mesh->shader;
		shader->Use();
		shader->SetFloat("time",sin(Time::GetCurrentTime()));
		_node->Rendering();
	}

private:

};

