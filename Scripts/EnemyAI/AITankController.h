#pragma once
#include "AI.h"
class AITankController :public Component
{
private:
	Tank* _tank;
	State* _currentState;
public:
	AITankController();
	~AITankController();
	void Awake();
	void Update();
	void LateUpdate();
	void SetState(State* state);
};