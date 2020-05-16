#pragma once
#include "FrogEngine.h"
using namespace FrogEngine;

class SkyDome:public Component
{
public:
	SkyDome() : Component{ "SkyDome" }
	{
	}

	~SkyDome()
	{
	}

	void Awake()
	{

	}

	void Update()
	{
		Shader* shader = _node->Find("MountainSkybox")->meshs[0]->shader;
		shader->Use();
		shader->SetFloat("alpha",sin(Time::GetTime())/5+0.2);
		_node->Rendering();
	}
	void LateUpdate()
	{
	}

private:

};

