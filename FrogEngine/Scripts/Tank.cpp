#pragma once
#include "Missile.h"
#include "Tank.h"

Node* Tank::model = nullptr;
Tank* Tank::Player = nullptr;

Tank::Tank(SphereCollider::Layer layer)
{
	if (layer==SphereCollider::Layer::Player)
	{
		Player = this;
		_fireCD = 0.5;
		_health = 6;
		_movementSpeed = 5;
	}
	_layer = layer;
	SphereCollider* collider = new SphereCollider(_layer, 0.6);
	SetCollider(collider);
	if (!model)
	{
		model = Model::LoadModel("Resource/Tank1/Tank1.FBX");
		model->LocalScale = Vector3(1, 1, 1) * 0.1;
		model->Retain();
	}
	_model = model->Clone();
	_model->SetLocalEulerAngleY(90);
	AddChild(_model);
	_battery = Find("Battery");
	_cannon = Find("Cannon");
	_muzzle = Find("Muzzle");
	_leftMudEmitterNode = Find("LeftMudEmitter");
	_rightMudEmitterNode = Find("RightMudEmitter");

	if (_muzzle)
	{
		Node* particleEmitterNode = ParticleEmitter::Create();
		_muzzleEmitter = (ParticleEmitter*)particleEmitterNode->GetComponent("ParticleEmitter");
		_muzzleEmitter->EmitSpeed = 1000;
		_muzzleEmitter->MaxParticles = 100;
		_muzzleEmitter->EmitAngle = 15;
		_muzzleEmitter->SetParticleColor(Vector3(1, 0.5, 0), Vector3(0.2, 0.1, 0));
		_muzzleEmitter->SetParticleLife(0.06, 0.08);
		_muzzleEmitter->SetParticleSpeed(4, 8);
		_muzzleEmitter->SetParticleSize(0.01, 0.05);
		_muzzle->AddChild(particleEmitterNode);
		particleEmitterNode->SetScale(Vector3(1, 1, 1));
	}

	if (_leftMudEmitterNode)
	{
		Node* particleEmitterNode = ParticleEmitter::Create();
		_leftMudEmitter = (ParticleEmitter*)particleEmitterNode->GetComponent("ParticleEmitter");
		_leftMudEmitter->EmitSpeed = 100;
		_leftMudEmitter->MaxParticles = 10;
		_leftMudEmitter->EmitAngle = 30;
		_leftMudEmitter->SetParticleColor(Vector3(0.5, 0.5, 0.25), Vector3(0.42, 0.31, 0.25));
		_leftMudEmitter->SetParticleLife(0.2, 0.3);
		_leftMudEmitter->SetParticleSpeed(2, 4);
		_leftMudEmitter->SetParticleSize(0.01, 0.05);
		_leftMudEmitter->Loop = true;
		_leftMudEmitterNode->AddChild(particleEmitterNode);
		particleEmitterNode->SetScale(Vector3(1, 1, 1));
	}

	if (_rightMudEmitterNode)
	{
		Node* particleEmitterNode = ParticleEmitter::Create();
		_rightMudEmitter = (ParticleEmitter*)particleEmitterNode->GetComponent("ParticleEmitter");
		_rightMudEmitter->EmitSpeed = 100;
		_rightMudEmitter->MaxParticles = 10;
		_rightMudEmitter->EmitAngle = 30;
		_rightMudEmitter->SetParticleColor(Vector3(0.5, 0.5, 0.25), Vector3(0.42, 0.31, 0.25));
		_rightMudEmitter->SetParticleLife(0.2, 0.3);
		_rightMudEmitter->SetParticleSpeed(2, 4);
		_rightMudEmitter->SetParticleSize(0.01, 0.05);
		_rightMudEmitter->Loop = true;
		_rightMudEmitterNode->AddChild(particleEmitterNode);
		particleEmitterNode->SetScale(Vector3(1, 1, 1));
	}
}

Tank::~Tank()
{
	if (this==Player)
	{
		Player = nullptr;
	}
}

void Tank::RotateCannon(float angle, float deltaTime)
{
	float maxAngle = deltaTime * _cannonRotateSpeed;
	if (_cannon)
	{
		float eulerAngle = _cannon->GetLocalEulerAngles().GetY() + angle;
		if (eulerAngle > _max_cannon_rotate)
		{
			eulerAngle = _max_cannon_rotate;
		}
		else if (eulerAngle < _min_cannon_rotate)
		{
			eulerAngle = _min_cannon_rotate;
		}
		_cannon->SetLocalEulerAngleY(eulerAngle);
	}
}

void Tank::RotateCannonTo(float angle, float deltaTime)
{
	float rotate = angle - _cannon->GetLocalEulerAngles().GetY();
	RotateCannon(rotate, deltaTime);
}

void Tank::RotateBattery(float angle, float deltaTime)
{
	float maxAngle = deltaTime * _cannonRotateSpeed;
	angle = Clamp(angle, -maxAngle, maxAngle);
	if (_battery)
	{
		float eulerAngle = _battery->GetLocalEulerAngles().GetZ() - angle;
		if (eulerAngle > _max_battery_rotate)
		{
			eulerAngle = _max_battery_rotate;
		}
		else if (eulerAngle < _min_battery_rotate)
		{
			eulerAngle = _min_battery_rotate;
		}
		_battery->SetLocalEulerAngleZ(eulerAngle);
	}
}

void Tank::RotateBatteryTo(float angle, float deltaTime)
{
	float rotate = _battery->GetLocalEulerAngles().GetZ() - angle;
	RotateBattery(rotate, deltaTime);
}

void Tank::FaceTo(Vector3 direction, float deltaTime)
{
	//不知为何，显示x轴反过来了，先实现效果吧
	direction.SetX(-direction.GetX());
	float angle = Vector3::Angle(Vector3::FRONT, direction) - GetLocalEulerAngles().GetY();
	//选择左旋右旋
	if (angle > 180)
	{
		angle -= 360;
	}
	else if (angle < -180)
	{
		angle += 360;
	}
	float rotateAngle = std::min(_rotateSpeed * deltaTime, abs(angle));
	if (angle < 0)
	{
		rotateAngle = -rotateAngle;
	}
	SetLocalEulerAngleY(GetLocalEulerAngles().GetY() + rotateAngle);
}

void Tank::ProcessKeyboardInput(Vector2 axis, float deltaTime)
{
	float axisHorizontal = axis.GetX();
	float axisVertical = axis.GetY();
	if (axisVertical > 0)
	{
		_leftMudEmitter->Start();
		_rightMudEmitter->Start();
	}
	else
	{
		_leftMudEmitter->Stop();
		_rightMudEmitter->Stop();
	}

	if (axisHorizontal == 0 && axisVertical == 0)
	{
		return;
	}
	Vector3 moveDirection = Vector3(axisHorizontal, 0, axisVertical);
	if (axisVertical < 0)
	{
		moveDirection = Vector3(0, 0, 0) - moveDirection;
	}
	//应该旋转的角度
	Scene* currentScene = Scene::GetCurrentScene();
	Camera* currentCamera = currentScene->GetCurrentCamera();
	float eulerAngleY = Vector3::Angle(Vector3::FRONT, moveDirection) + currentCamera->GetLocalEulerAngles().GetY() - GetLocalEulerAngles().GetY();

	//选择左旋右旋
	if (eulerAngleY > 180)
	{
		eulerAngleY -= 360;
	}
	else if (eulerAngleY < -180)
	{
		eulerAngleY += 360;
	}
	float rotateAngle = std::min(_rotateSpeed * deltaTime, abs(eulerAngleY));
	if (eulerAngleY < 0)
	{
		rotateAngle = -rotateAngle;
	}
	SetLocalEulerAngleY(GetLocalEulerAngles().GetY() + rotateAngle);
	_battery->SetLocalEulerAngleZ(_battery->GetLocalEulerAngles().GetZ() - rotateAngle);

	if (axisVertical < 0)
	{
		MoveBackward(deltaTime);
	}
	else
	{
		MoveForward(deltaTime);
	}
}

void Tank::MoveForward(float deltaTime)
{
	Vector3 tankForward = GetForward();
	tankForward.SetY(0);
	tankForward = tankForward.Normalized();
	if (!_collider || _collider->CanMove(tankForward))
	{
		LocalPosition = LocalPosition + tankForward * _movementSpeed * deltaTime;
	}
}
void Tank::MoveBackward(float deltaTime)
{
	Vector3 tankBackward = Vector3(0,0,0)-GetForward();
	tankBackward.SetY(0);
	tankBackward = tankBackward.Normalized();
	if (!_collider || _collider->CanMove(tankBackward))
	{
		LocalPosition = LocalPosition + tankBackward * _movementSpeed * deltaTime;
	}
}

void Tank::MoveToward(Vector3 des, float deltaTime)
{
	Vector3 delta = des - GetPosition();
	//不知为何，显示x轴反过来了，先实现效果吧
	delta.SetX(-delta.GetX());
	float angle = Vector3::Angle(Vector3::FRONT, delta) - GetLocalEulerAngles().GetY();
	//选择左旋右旋
	if (angle > 180)
	{
		angle -= 360;
	}
	else if (angle < -180)
	{
		angle += 360;
	}
	float rotateAngle = std::min(_rotateSpeed * deltaTime, abs(angle));
	if (angle < 0)
	{
		rotateAngle = -rotateAngle;
	}
	SetLocalEulerAngleY(GetLocalEulerAngles().GetY() + rotateAngle);
	if (abs(angle) > 90)
	{
		return;
	}
	if (delta.Length() > 10 || angle < 5)
	{
		MoveForward(deltaTime);
	}
}

void Tank::Fire()
{
	if (Time::GetTime() - _lastFireTime < _fireCD)
	{
		//开炮CD没好
		return;
	}
	_lastFireTime = Time::GetTime();
	if (_muzzleEmitter)
	{
		_muzzleEmitter->ReStart();
	}
	Scene* currentScene = Scene::GetCurrentScene();
	Vector3 position = _muzzle->GetPosition();
	Vector3 direction = Vector3(0, 0, 0) - _cannon->GetRight();
	SphereCollider::Layer missileLayer;
	if (_layer==SphereCollider::Layer::Player)
	{
		missileLayer = SphereCollider::Layer::PlayerMissile;
	}
	else
	{
		missileLayer = SphereCollider::Layer::EnemyMissile;
	}
	Node* missile = Missile::Create(missileLayer, position, direction);
	currentScene->AddObject(missile);
}

int Tank::GetHealthValue() const
{
	return _health;
}

void Tank::AddHealthValue(int value)
{
	if (_health + value < 0)
	{
		_health = 0;
	}
	else
	{
		_health += value;
	}
}

void Tank::RotateBatteryAfterDeath(float angle, float deltaTime)
{
	float rotate = angle - _battery->GetLocalEulerAngles().GetY();
	float maxAngle = deltaTime * _cannonRotateSpeed;
	rotate = Clamp(rotate, -maxAngle, maxAngle);
	if (_battery)
	{
		float eulerAngle = _battery->GetLocalEulerAngles().GetY() + rotate;
		_battery->SetLocalEulerAngleY(eulerAngle);
	}
}