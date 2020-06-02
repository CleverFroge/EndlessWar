#pragma once
#include "FrogEngine.h"
using namespace FrogEngine;
#include "Tank.h"

class State
{
public:
	virtual void OnEnter() = 0;
	virtual void Tick() = 0;
	virtual void OnExit() = 0;
	virtual ~State() {};
protected:
	State() {};
}; 
class AITankController;
class PatrolState :public State
{
private:
	float _detectRange = 20;
	float _patrolRange = 10;
	Tank* _tank;
	AITankController* _aiController;
	Vector3 _desPos;
	Vector3 _oriPos;

	float _stopTime = 2;
	float _lastMoveTime = 0;
public:
	PatrolState(Tank* tank, AITankController* aiController);
	~PatrolState();
	virtual void OnEnter();
	virtual void Tick();
	virtual void OnExit();
private:
	void SetRandDes();
};

class ChaseState :public State
{
private:
	float _stopDistance = 5;
	Tank* _tank;
	AITankController* _aiController;
public:
	ChaseState(Tank* tank, AITankController* aiController);
	~ChaseState();
	virtual void OnEnter();
	virtual void Tick();
	virtual void OnExit();
private:

};

class DeathState :public State
{
private:
	Tank* _tank;
	float _deathTime;
private:
	const float _releaseTime = 5;
	const float _cann0nAngleY = -30;
public:
	DeathState(Tank* tank);
	~DeathState();
	virtual void OnEnter();
	virtual void Tick();
	virtual void OnExit();
private:

};
