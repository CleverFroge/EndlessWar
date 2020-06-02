#include "AI.h"
#include "AITankController.h"

ChaseState::ChaseState(Tank* tank, AITankController* aiController)
{
	_tank = tank;
	_aiController = aiController;
}

ChaseState::~ChaseState()
{
}

void ChaseState::OnEnter()
{

}

void ChaseState::Tick()
{
	//ËÀÍö£¿
	if (_tank->GetHealthValue() == 0)
	{
		_aiController->SetState(new DeathState(_tank));
		return;
	}
	Tank* player = Tank::Player;
	if (!player)
	{
		_aiController->SetState(new PatrolState(_tank, _aiController));
		return;
	}
	float deltaTime = Time::GetDeltaTime();
	Vector3 playerPos = player->GetPosition();
	Vector3 pos = _tank->GetPosition();
	Vector3 delta = playerPos - pos;
	float distance = delta.Length();
	float moveDistance = distance - _stopDistance;
	if (moveDistance > 0)
	{
		Vector3 desPos = pos + delta.Normalized() * moveDistance;
		_tank->MoveToward(desPos, deltaTime);
	}
	Vector3 forward = _tank->GetForward();
	float angleX = Clamp(distance - 10, 0, 40) / 60 * 45;
	
	float angleY = Vector3::Angle(forward, delta);
	_tank->RotateCannonTo(angleX, deltaTime);
	_tank->RotateBatteryTo(angleY, deltaTime);
	_tank->Fire();
}

void ChaseState::OnExit()
{

}