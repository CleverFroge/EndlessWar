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
	void OnCollision(SphereCollider* collider);
private:
	
	~ExplosionNode();
};

ExplosionNode::ExplosionNode(SphereCollider::Layer layer, Vector3 pos, float size)
{
	_pos = pos;
	_size = size;
	float scale = std::sqrt(abs(size));
	explositionEmitter = new ParticleEmitter();
	explositionEmitter->EmitSpeed = 2000;
	explositionEmitter->MaxParticles = 500;
	explositionEmitter->EmitAngle = 180;
	explositionEmitter->SetParticleColor(Vector3(1, 0.5, 0), Vector3(0.2, 0.1, 0));
	explositionEmitter->SetParticleLife(0.2 * scale, 0.25 * scale);
	explositionEmitter->SetParticleSpeed(2 * scale, 4 * scale);
	explositionEmitter->SetParticleSize(0.1, 0.2);
	explositionEmitter->Loop = false;
	explositionEmitter->ReleaseAfterEmit = true;
	AddComponent(explositionEmitter);
	LocalPosition = pos;

	SphereCollider* collider = new SphereCollider(layer, size);
	SetCollider(collider);
}

ExplosionNode::~ExplosionNode()
{
}

void ExplosionNode::Start()
{
	explositionEmitter->Start();
}

void ExplosionNode::OnCollision(SphereCollider* collider)
{
	if (_collider->GetLayer() == SphereCollider::Layer::PlayerExplosion && collider->GetLayer() == SphereCollider::Layer::Enemy
		|| _collider->GetLayer() == SphereCollider::Layer::EnemyExplosion && collider->GetLayer() == SphereCollider::Layer::Player)
	{
		collider->GetNode()->RemoveFromParent();
	}
}