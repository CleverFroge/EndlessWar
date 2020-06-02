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
	//死亡？
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
	//处理鼠标移动输入
	float angleX = Input::GetMousePosDeltaY() * _mouseSensitivity;
	float angleY = Input::GetMousePosDeltaX() * _mouseSensitivity;
	_tank->RotateBattery(angleY, deltaTime);
	_tank->RotateCannon(angleX, deltaTime);
	//处理键盘输入
	_tank->ProcessKeyboardInput(Input::GetAxis(), deltaTime);
}
void TankController::LateUpdate()
{
}