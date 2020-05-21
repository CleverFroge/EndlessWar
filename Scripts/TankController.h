#include "FrogEngine.h"
using namespace FrogEngine;

class TankController : public Component
{
public:
	float _movementSpeed = 3;
	float _mouseSensitivity = 0.05;
	float _rotateSpeed = 60;

	float _min_battery_rotate = -60;
	float _max_battery_rotate = 60;
	float _min_cannon_rotate = 0;
	float _max_cannon_rotate = 60;
private:
	Node* _battery = nullptr;
	Node* _cannon = nullptr;
	Node* _muzzle = nullptr;
	Node* _leftMudEmitterNode = nullptr;
	Node* _rightMudEmitterNode = nullptr;

	ParticleEmitter* _muzzleEmitter = nullptr;
	ParticleEmitter* _leftMudEmitter = nullptr;
	ParticleEmitter* _rightMudEmitter = nullptr;
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
		_muzzle = _node->Find("Muzzle");
		_leftMudEmitterNode = _node->Find("LeftMudEmitter");
		_rightMudEmitterNode = _node->Find("RightMudEmitter");

		if (_muzzle)
		{
			Node* particleEmitterNode = ParticleEmitter::Create();
			_muzzleEmitter = (ParticleEmitter*)particleEmitterNode->GetComponent("ParticleEmitter");
			_muzzleEmitter->EmitSpeed = 1000;
			_muzzleEmitter->MaxParticles = 100;
			_muzzleEmitter->EmitAngle = 15;
			_muzzleEmitter->SetParticleColor(Vector3(1, 0.5, 0), Vector3(0.2, 0.1, 0));
			_muzzleEmitter->SetParticleLife(0.06, 0.08);
			_muzzleEmitter->SetParticleSpeed(4, 8);
			_muzzle->AddChild(particleEmitterNode);
			particleEmitterNode->SetScale(Vector3(1, 1, 1));
		}

		if (_leftMudEmitterNode)
		{
			Node* particleEmitterNode = ParticleEmitter::Create();
			_leftMudEmitter = (ParticleEmitter*)particleEmitterNode->GetComponent("ParticleEmitter");
			_leftMudEmitter->EmitSpeed = 100;
			_leftMudEmitter->MaxParticles = 10;
			_leftMudEmitter->EmitAngle = 30;
			_leftMudEmitter->SetParticleColor(Vector3(0.5, 0.5, 0.25), Vector3(0.42, 0.31, 0.25));
			_leftMudEmitter->SetParticleLife(0.2, 0.3);
			_leftMudEmitter->SetParticleSpeed(2, 4);
			_leftMudEmitter->Loop = true;
			_leftMudEmitterNode->AddChild(particleEmitterNode);
			particleEmitterNode->SetScale(Vector3(1, 1, 1));
		}

		if (_rightMudEmitterNode)
		{
			Node* particleEmitterNode = ParticleEmitter::Create();
			_rightMudEmitter = (ParticleEmitter*)particleEmitterNode->GetComponent("ParticleEmitter");
			_rightMudEmitter->EmitSpeed = 100;
			_rightMudEmitter->MaxParticles = 10;
			_rightMudEmitter->EmitAngle = 30;
			_rightMudEmitter->SetParticleColor(Vector3(0.5, 0.5, 0.25), Vector3(0.42, 0.31, 0.25));
			_rightMudEmitter->SetParticleLife(0.2, 0.3);
			_rightMudEmitter->SetParticleSpeed(2, 4);
			_rightMudEmitter->Loop = true;
			_rightMudEmitterNode->AddChild(particleEmitterNode);
			particleEmitterNode->SetScale(Vector3(1, 1, 1));
		}
	}
	void Update()
	{
		if (_muzzleEmitter && Input::GetKey(GLFW_MOUSE_BUTTON_LEFT))
		{
			_muzzleEmitter->ReStart();
		}
		//处理鼠标移动输入
		Aim(Input::GetMousePosDeltaX(), Input::GetMousePosDeltaY());
		//处理键盘输入
		float deltaTime = Time::GetDeltaTime();
		Move(Input::GetAxis(), deltaTime);
	}
	void LateUpdate()
	{
	}
	void Aim(float deltaX, float deltaY)
	{
		if (_battery)
		{
			float eulerAngle = _battery->GetLocalEulerAngles().GetZ() - deltaX * _mouseSensitivity;
			if (eulerAngle > _max_battery_rotate)
			{
				eulerAngle = _max_battery_rotate;
			}
			else if (eulerAngle < _min_battery_rotate)
			{
				eulerAngle = _min_battery_rotate;
			}
			_battery->SetLocalEulerAngleZ(eulerAngle);
		}
		if (_cannon)
		{
			float eulerAngle = _cannon->GetLocalEulerAngles().GetY() + deltaY * _mouseSensitivity;
			if (eulerAngle > _max_cannon_rotate)
			{
				eulerAngle = _max_cannon_rotate;
			}
			else if (eulerAngle < _min_cannon_rotate)
			{
				eulerAngle = _min_cannon_rotate;
			}
			_cannon->SetLocalEulerAngleY(eulerAngle);
		}
	}
	void Move(Vector2 axis, float deltaTime)
	{
		float axisHorizontal = axis.GetX();
		float axisVertical = axis.GetY();
		if (axisVertical > 0)
		{
			_leftMudEmitter->Start();
			_rightMudEmitter->Start();
		}
		else
		{
			_leftMudEmitter->Stop();
			_rightMudEmitter->Stop();
		}

		if (axisHorizontal == 0 && axisVertical == 0)
		{
			return;
		}
		Vector3 moveDirection = Vector3(axisHorizontal, 0, axisVertical);
		if (axisVertical < 0)
		{
			moveDirection = Vector3(0, 0, 0) - moveDirection;
		}
		//应该旋转的角度
		Scene* currentScene = Scene::GetCurrentScene();
		Camera* currentCamera = currentScene->GetCurrentCamera();
		float eulerAngleY = Vector3::Angle(Vector3::FRONT, moveDirection) + currentCamera->GetLocalEulerAngles().GetY() - _node->GetLocalEulerAngles().GetY() + 90;
		//选择左旋右旋
		if (eulerAngleY > 180)
		{
			eulerAngleY -= 360;
		}
		else if (eulerAngleY < -180)
		{
			eulerAngleY += 360;
		}
		float rotateAngle = std::min(_rotateSpeed * deltaTime, abs(eulerAngleY));
		if (eulerAngleY < 0)
		{
			rotateAngle = -rotateAngle;
		}
		_node->SetLocalEulerAngleY(_node->GetLocalEulerAngles().GetY() + rotateAngle);
		_battery->SetLocalEulerAngleZ(_battery->GetLocalEulerAngles().GetZ() - rotateAngle);

		if (axisVertical > 0)
		{
			MoveForward(deltaTime);
		}
		if (axisVertical < 0)
		{
			MoveBackward(deltaTime);
		}
	}
private:
	void MoveForward(float deltaTime)
	{
		Vector3 tankForward = _node->GetRight();
		tankForward.SetY(0);
		tankForward = tankForward.Normalized();
		_node->LocalPosition = _node->LocalPosition - tankForward * _movementSpeed * deltaTime;
	}
	void MoveBackward(float deltaTime)
	{
		Vector3 tankForward = _node->GetRight();
		tankForward.SetY(0);
		tankForward = tankForward.Normalized();
		_node->LocalPosition = _node->LocalPosition + tankForward * _movementSpeed * deltaTime;
	}
};