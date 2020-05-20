#pragma once
#include "FrogEngine.h"
using namespace FrogEngine;

Particle::Particle()
{
	Mesh* quad = Mesh::Create(Mesh::Geometry::Quad);
	quad->shader = Shader::GetShader("Particle");
	meshs.push_back(quad);
	LocalScale = Vector3(0.1, 0.1, 0.1);

	BornTime = Time::GetTime();
	LifeTime = 1;
	MoveSpeed = 1;

	AutoRendering = false;
}

Particle::~Particle()
{
}