#pragma once
#include "Missile.h"
#include "FrogEngine.h"
using namespace FrogEngine;

class Tank :public Node
{
public:
	float _movementSpeed = 3;
	float _cannonRotateSpeed = 180;
	float _rotateSpeed = 60;

	float _min_battery_rotate = -60;
	float _max_battery_rotate = 60;
	float _min_cannon_rotate = 0;
	float _max_cannon_rotate = 60;
	static Tank* Player;

	int kill;
private:
	SphereCollider::Layer _layer;
	Node* _battery = nullptr;
	Node* _cannon = nullptr;
	Node* _muzzle = nullptr;
	Node* _leftMudEmitterNode = nullptr;
	Node* _rightMudEmitterNode = nullptr;

	ParticleEmitter* _muzzleEmitter = nullptr;
	ParticleEmitter* _leftMudEmitter = nullptr;
	ParticleEmitter* _rightMudEmitter = nullptr;
	Node* _model;
	static Node* model;
	float _fireCD = 3;
	float _lastFireTime = 0;
	int _health = 2;
public:
	Tank(SphereCollider::Layer layer);
	void RotateCannon(float angle, float deltaTime);
	void RotateCannonTo(float angle, float deltaTime);
	void RotateBattery(float angle, float deltaTime);
	void RotateBatteryTo(float angle, float deltaTime);
	void Fire();
	void ProcessKeyboardInput(Vector2 axis, float deltaTime);
	void FaceTo(Vector3 direction, float deltaTime);
	void MoveToward(Vector3 des, float deltaTime);
	int GetHealthValue() const;
	void AddHealthValue(int value);
	void RotateBatteryAfterDeath(float angle, float deltaTime);
private:
	virtual ~Tank();
	void MoveForward(float deltaTime);
	void MoveBackward(float deltaTime);
};