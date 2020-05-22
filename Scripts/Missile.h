#pragma once
#include"FrogEngine.h"
using namespace FrogEngine;

class Missile :public Component
{
private:
	static float Gravity;
	static Node* missile;
	static float speed;
	static float lifeTime;

	Vector3 startPos;
	float startTime;
	Vector3 startVelocity;
public:
	~Missile() {}
	static Node* Create(Vector3 position, Vector3 direction)
	{
		if (!missile)
		{
			missile = Model::LoadModel("../Resource/Missile/Missile.FBX");
			missile->LocalScale = Vector3(1, 1, 1) * 0.05;
			missile->Retain();
		}
		Node* ret = missile->Clone();
		Missile* missileComponent = new Missile();
		missileComponent->startVelocity = direction * speed;
		missileComponent->startPos = position;
		missileComponent->startTime = Time::GetTime();
		ret->AddComponent(missileComponent);
		ret->SetLocalPosition(position);
		ret->SetLocalForward(direction);
		return ret;
	}

	void Awake() {};

	void Update()
	{
		float flyTime = Time::GetTime() - startTime;

		bool explosition = false;
		if (flyTime > lifeTime)
		{
			flyTime = lifeTime;
			explosition = true;
		}
		Vector3 pos = GetPosition(flyTime, startPos, startVelocity);
		_node->SetLocalPosition(pos);
		
		Vector3 forward = GetForward(flyTime, startVelocity);
		_node->SetLocalForward(forward);

		if (pos.GetY() <= 0)
		{
			explosition = true;
		}
		if (explosition)
		{
			Node* ExplositionNode = ParticleEmitter::Create();
			auto explositionEmitter = (ParticleEmitter*)ExplositionNode->GetComponent("ParticleEmitter");
			explositionEmitter->EmitSpeed = 2000;
			explositionEmitter->MaxParticles = 500;
			explositionEmitter->EmitAngle = 180;
			explositionEmitter->SetParticleColor(Vector3(1, 0.5, 0), Vector3(0.2, 0.1, 0));
			explositionEmitter->SetParticleLife(0.3,0.7);
			explositionEmitter->SetParticleSpeed(13, 15);
			explositionEmitter->SetParticleSize(0.1,0.2);
			explositionEmitter->Loop = false;
			explositionEmitter->ReleaseAfterEmit = true;
			Scene::GetCurrentScene()->AddObject(ExplositionNode);
			ExplositionNode->LocalPosition = pos;
			explositionEmitter->Start();
			_node->RemoveFromParent();
		}
	}

	void LateUpdate() {};
private:
	Missile() : Component{ "Missile" }
	{

	}
	static Vector3 GetPosition(float flyTime, Vector3 startPosition, Vector3 startVelocity)
	{
		Vector3 pos = startPosition + Vector3(
			startVelocity.GetX() * flyTime,
			startVelocity.GetY() * flyTime + Gravity * flyTime * flyTime / 2,
			startVelocity.GetZ() * flyTime
		);
		return pos;
	}
	static Vector3 GetForward(float flyTime, Vector3 startVelocity)
	{
		Vector3 forward = Vector3(
			startVelocity.GetX(),
			startVelocity.GetY() + flyTime * Gravity,
			startVelocity.GetZ()
		).Normalized();
		return forward;
	}
};
Node* Missile::missile = nullptr;
float Missile::Gravity = -9.8;
float Missile::lifeTime = 2;
float Missile::speed = 60;