#pragma once
#include"FrogEngine.h"
using namespace FrogEngine;

class ExplosionNode : public Node
{
private:
	float _startTime;
	Vector3 _pos;
	float _size;

	ParticleEmitter* explositionEmitter;
public:
	ExplosionNode(SphereCollider::Layer layer, Vector3 pos, float size);
	void Start();
	void OnEnter(SphereCollider* collider);
private:
	
	~ExplosionNode();
};