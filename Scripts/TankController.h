#include "FrogEngine.h"
using namespace FrogEngine;

class TankController : public Component
{
public:
	float _movementSpeed = 10;
	float _mouseSensitivity = 0.05;
	float _rotateSpeed = 60;
private:
	Node* _battery = nullptr;
	Node* _cannon = nullptr;
public:
	TankController() : Component{"TankController"}
	{
	}
	~TankController()
	{
	}
	void Awake()
	{
		_battery = _node->Find("Battery");
		_cannon = _node->Find("Cannon");
	}
	void Update()
	{
		/*
		float eulerAngle = _battery->GetLocalEulerAngles().GetY();
		_battery->SetLocalEulerAngleY(eulerAngle - 60*Time::GetDeltaTime());
		return;
		*/
		//处理鼠标输入
		if (_battery)
		{
			float eulerAngle = _battery->GetLocalEulerAngles().GetY();
			_battery->SetLocalEulerAngleY(eulerAngle - Input::GetMousePosDeltaX() * _mouseSensitivity);
		}

		if (_cannon)
		{
			float eulerAngle = _cannon->GetLocalEulerAngles().GetX();
			_cannon->SetLocalEulerAngleX(eulerAngle + Input::GetMousePosDeltaY() * _mouseSensitivity);
		}
		//处理键盘输入
		Vector3 moveDirection(0, 0, 0);
		if (Input::GetKey(GLFW_KEY_W))
		{
			moveDirection = moveDirection + Vector3::FRONT;
		}
		if (Input::GetKey(GLFW_KEY_A))
		{
			moveDirection = moveDirection - Vector3::RIGHT;
		}
		if (Input::GetKey(GLFW_KEY_S))
		{
			moveDirection = moveDirection - Vector3::FRONT;
		}
		if (Input::GetKey(GLFW_KEY_D))
		{
			moveDirection = moveDirection + Vector3::RIGHT;
		}
		if (!(moveDirection == Vector3(0, 0, 0)))
		{
			moveDirection = moveDirection.Normalized();
			//应该旋转的角度
			Scene* currentScene = Scene::GetCurrentScene();
			Camera* currentCamera = currentScene->GetCurrentCamera();
			float eulerAngleY = Vector3::Angle(Vector3::FRONT, moveDirection)+ currentCamera->GetLocalEulerAngles().GetY()-_node->GetLocalEulerAngles().GetY();
			//选择左旋右旋
			if (eulerAngleY > 180)
			{
				eulerAngleY -= 360;
			}
			else if (eulerAngleY < -180)
			{
				eulerAngleY += 360;
			}
			float rotateAngle = std::min(_rotateSpeed * Time::GetDeltaTime(), abs(eulerAngleY));
			if (eulerAngleY<0)
			{
				rotateAngle = -rotateAngle;
			}
			_node->SetLocalEulerAngleY(_node->GetLocalEulerAngles().GetY() + rotateAngle);
			_battery->SetLocalEulerAngleY(_battery->GetLocalEulerAngles().GetY() - rotateAngle);
			Vector3 tankForward = _node->GetForward();
			tankForward.SetY(0);
			tankForward.Normalized();
			_node->LocalPosition = _node->LocalPosition + tankForward * _movementSpeed * Time::GetDeltaTime();
		}
	}
	void LateUpdate()
	{
	}
};