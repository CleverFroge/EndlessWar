#pragma once
#include"FrogEngine.h"
using namespace FrogEngine;

class Spawner :public Component
{
private:
	float _lastGenTime;
	float _cd = 5;
	int _maxEnemy = 10;
private:
	void GenEnemy();
	Spawner();
public:
	static Node* Create();
	~Spawner();
	void Awake();
	void Update();
	void LateUpdate();
};