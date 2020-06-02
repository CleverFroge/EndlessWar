#pragma once
#include "SkyDome.h"

SkyDome::SkyDome() : Component{ "SkyDome" }
{
}

SkyDome::~SkyDome()
{
}

void SkyDome::Awake()
{

}

void SkyDome::Update()
{
	Shader* shader = _node->Find("MountainSkybox")->GetMesh(0)->shader;
	shader->Use();
	shader->SetFloat("alpha", sin(Time::GetTime()) / 5 + 0.2);
	_node->Rendering();
}
void SkyDome::LateUpdate()
{
}