#include "AI.h"
#include "AITankController.h"

PatrolState::PatrolState(Tank* tank, AITankController* aiController)
{
	_tank = tank;
	_aiController = aiController;
}

PatrolState::~PatrolState()
{
}

void PatrolState::OnEnter()
{
	_oriPos = _tank->GetPosition();
	SetRandDes();
}

void PatrolState::Tick()
{
	//ËÀÍö£¿
	if (_tank->GetHealthValue() == 0)
	{
		_aiController->SetState(new DeathState(_tank));
		return;
	}
	Vector3 pos = _tank->GetPosition();
	if ((pos - _desPos).Length() <= 1 || Time::GetTime() - _lastMoveTime >= _stopTime)
	{
		SetRandDes();
	}
	_tank->MoveToward(_desPos, Time::GetDeltaTime());
	if ((pos - _tank->GetPosition()).Length() != 0)
	{
		_lastMoveTime = Time::GetTime();
	}
	Tank* player = Tank::Player;
	if (!player)
	{
		return;
	}
	float distance = (player->GetPosition() - _tank->GetPosition()).Length();
	if (distance <= _detectRange)
	{
		_aiController->SetState(new ChaseState(_tank, _aiController));
		return;
	}
}

void PatrolState::OnExit()
{

}

void PatrolState::SetRandDes()
{
	_desPos = _oriPos + Vector3(Random::Randf(-1, 1), 0, Random::Randf(-1, 1)).Normalized() * _patrolRange;
}