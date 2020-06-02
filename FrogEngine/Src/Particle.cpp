#pragma once
#include "FrogEngine.h"
using namespace FrogEngine;

Particle::Particle()
{
	Mesh* quad = Mesh::Create(Mesh::Geometry::Quad);
	quad->shader = Shader::GetShader("Particle");
	AddMesh(quad);

	BornTime = Time::GetTime();

	AutoRendering = false;
}

Particle::~Particle()
{
}