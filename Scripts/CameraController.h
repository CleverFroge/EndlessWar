#pragma once
#include"FrogEngine.h"
using namespace FrogEngine;
#include "Tank.h"

class CameraController:public Component
{
public:
	Tank* _tank = nullptr;
	Node* _battery = nullptr;
	Node* _cannon = nullptr;
private:
	float _movementSpeed = 25;
	float _mouseSensitivity = 180;
	float _zoom = 45;

public:
	CameraController();
	~CameraController();
	void Awake();
	void Update();
	void LateUpdate();
};
