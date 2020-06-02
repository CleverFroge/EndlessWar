#include "AI.h"

DeathState::DeathState(Tank* tank)
{
	_tank = tank;
}

DeathState::~DeathState()
{
}

void DeathState::OnEnter()
{
	_deathTime = Time::GetTime();
}

void DeathState::Tick()
{
	float timeAfterDeath = Time::GetTime() - _deathTime;
	if (timeAfterDeath >= _releaseTime)
	{
		_tank->RemoveFromParent();
		return;
	}
	float angleX = _cann0nAngleY * timeAfterDeath / _releaseTime;
	_tank->RotateBatteryAfterDeath(angleX, Time::GetDeltaTime());
}

void DeathState::OnExit()
{

}