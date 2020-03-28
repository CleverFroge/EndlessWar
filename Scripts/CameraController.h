#pragma once
#include"FrogEngine.h"
using namespace FrogEngine;

class CameraController:public Component
{
public:
	Node* _followObject = nullptr;
private:
	float _movementSpeed = 25;
	float _mouseSensitivity = 180;
	float _zoom = 45;
public:
	CameraController()
	{

	}
	~CameraController()
	{

	}
	void Awake()
	{

	}
	void Update()
	{
		if (_followObject)
		{
			Vector3 pos = _followObject->GetPosition() + Vector3(0, 2, 0) - _followObject->GetForward() * 8;
			_node->SetLocalPosition(pos);
			_node->SetLocalForward(_followObject->GetForward());
		}
		return;
		////处理鼠标输入
		//float deltaX = -Input::GetMousePosDeltaX() / Screen::GetWidth();
		//float deltaY = Input::GetMousePosDeltaY() / Screen::GetHeight();
		//deltaX *= _mouseSensitivity;
		//deltaY *= _mouseSensitivity;

		//Vector3 eulerAngle = _node->GetLocalEulerAngles();
		//float pitch = eulerAngle.GetX();
		//float yaw = eulerAngle.GetY();

		//pitch += deltaY;
		//if (pitch > 89.0f)
		//	pitch = 89.0f;
		//if (pitch < -89.0f)
		//	pitch = -89.0f;
		//yaw += deltaX;
		//_node->SetLocalEulerAngleX(pitch);
		//_node->SetLocalEulerAngleY(yaw);
		//
		////处理键盘输入
		//Vector3 moveDirection(0,0,0);
		//if (Input::GetKey(GLFW_KEY_W))
		//{
		//	moveDirection = moveDirection + _node->GetLocalForward();
		//}
		//if (Input::GetKey(GLFW_KEY_A))
		//{
		//	moveDirection = moveDirection - _node->GetLocalRight();
		//}
		//if (Input::GetKey(GLFW_KEY_S))
		//{
		//	moveDirection = moveDirection - _node->GetLocalForward();
		//}
		//if (Input::GetKey(GLFW_KEY_D))
		//{
		//	moveDirection = moveDirection + _node->GetLocalRight();
		//}

		//if (Input::GetKey(GLFW_KEY_SPACE))
		//{
		//	moveDirection = moveDirection + _node->GetLocalUp();
		//}
		//if (Input::GetKey(GLFW_KEY_LEFT_SHIFT))
		//{
		//	moveDirection = moveDirection - _node->GetLocalUp();
		//}
		//if (!(moveDirection == Vector3(0, 0, 0)))
		//{
		//	moveDirection = moveDirection.Normalized();		
		//	_node->LocalPosition = _node->LocalPosition + moveDirection * _movementSpeed * Time::GetDeltaTime();
		//}
		//
	}
};
