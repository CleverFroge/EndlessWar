#include "FrogEngine.h"
#include "TankController.h"
using namespace FrogEngine;

TankController::TankController() : Component{ "TankController" }
{
}
TankController::~TankController()
{
}
void TankController::Awake()
{
	_tank = (Tank*)_node;
}
void TankController::Update()
{
	float deltaTime = Time::GetDeltaTime();
	//������
	if (_tank->GetHealthValue() == 0)
	{
		_tank->RotateBatteryAfterDeath(-30, deltaTime);
		Tank::Player = nullptr;
		return;
	}
	if (Input::GetKey(GLFW_MOUSE_BUTTON_LEFT))
	{
		_tank->Fire();
	}
	//��������ƶ�����
	float angleX = Input::GetMousePosDeltaY() * _mouseSensitivity;
	float angleY = Input::GetMousePosDeltaX() * _mouseSensitivity;
	_tank->RotateBattery(angleY, deltaTime);
	_tank->RotateCannon(angleX, deltaTime);
	//�����������
	_tank->ProcessKeyboardInput(Input::GetAxis(), deltaTime);
}
void TankController::LateUpdate()
{
}