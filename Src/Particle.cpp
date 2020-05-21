#pragma once
#include "FrogEngine.h"
using namespace FrogEngine;

Particle::Particle()
{
	Mesh* quad = Mesh::Create(Mesh::Geometry::Quad);
	quad->shader = Shader::GetShader("Particle");
	AddMesh(quad);
	LocalScale = Vector3(0.01, 0.01, 0.01);

	BornTime = Time::GetTime();

	AutoRendering = false;
}

Particle::~Particle()
{
}