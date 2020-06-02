#pragma once
#include "AITankController.h"

AITankController::AITankController() : Component{ "AITankController" }
{
}
AITankController::~AITankController()
{
	if (_currentState)
	{
		delete _currentState;
	}
}
void AITankController::Awake()
{
	_tank = (Tank*)_node;
	_currentState = new PatrolState(_tank, this);
}
void AITankController::Update()
{
	_currentState->Tick();
}
void AITankController::LateUpdate()
{
}
void AITankController::SetState(State* state)
{
	_currentState->OnExit();
	delete _currentState;
	_currentState = state;
	_currentState->OnEnter();
}