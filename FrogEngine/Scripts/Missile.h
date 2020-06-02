#pragma once
#include"FrogEngine.h"
using namespace FrogEngine;
#include "Explosion.h"

class Missile :public Node
{
	friend class MissileComponent;
public:
	static float Gravity;
	static float speed;
private:
	static Node* missile;
	SphereCollider::Layer _layer;
	Vector3 startPos;
	Vector3 startVelocity;
public:
	static Node* Create(SphereCollider::Layer layer, Vector3 position, Vector3 direction);
	void Explosion();
private:
	Missile();
	~Missile();
	void OnEnter(SphereCollider* collider);
	Vector3 GetPositionAt(float flyTime);
	Vector3 GetForwardAt(float flyTime);
};

class MissileComponent :public Component
{
private:
	float startTime;
	Missile* missile;
	float lifeTime = 2;
public:
	MissileComponent();
	~MissileComponent();

	void Awake();
	void Update();
	void LateUpdate();
private:

};