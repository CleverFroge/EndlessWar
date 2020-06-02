#include"CameraController.h"

CameraController::CameraController() : Component{ "CameraController" }
{

}
CameraController::~CameraController()
{

}
void CameraController::Awake()
{
}
void CameraController::Update()
{
}
void CameraController::LateUpdate()
{
	if (!_tank)
	{
		return;
	}
	if (_tank->GetHealthValue() == 0)
	{
		float deltaTime = Time::GetDeltaTime();
		Vector3 delta = _node->GetPosition() - _tank->GetPosition();
		Matrix4 rotate;
		rotate.Rotate(Vector3(0, 1, 0), deltaTime * 30);
		delta = delta * rotate;
		float distance = delta.Length();
		distance = Clamp(distance + deltaTime * 0.5, 0, 8);
		delta = delta.Normalized() * distance;
		Vector3 pos = delta + _tank->GetPosition();
		float height = Clamp(pos.GetY() + deltaTime * 0.5, 0, 4);
		pos.SetY(height);
		_node->SetLocalPosition(pos);
		_node->SetLocalForward(Vector3(0, 0, 0) - delta);
		return;
	}
	if (!_battery)
	{
		_battery = _tank->Find("Battery");
	}
	if (!_cannon)
	{
		_cannon = _tank->Find("Cannon");
	}
	Vector3 pos;
	if (_battery)
	{
		pos = _battery->GetPosition();
	}
	if (_cannon)
	{
		pos = pos + (Vector3(0, 0.2, 0) + _cannon->GetRight()) * 2.5;
		_node->SetLocalForward(Vector3(0, 0, 0) - _cannon->GetRight());
	}
	_node->SetLocalPosition(pos);
	return;
	//处理鼠标输入
	float deltaX = Input::GetMousePosDeltaX() / Screen::GetWidth();
	float deltaY = Input::GetMousePosDeltaY() / Screen::GetHeight();
	deltaX *= _mouseSensitivity;
	deltaY *= _mouseSensitivity;

	Vector3 eulerAngle = _node->GetLocalEulerAngles();
	float pitch = eulerAngle.GetX();
	float yaw = eulerAngle.GetY();

	pitch -= deltaY;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	yaw -= deltaX;
	_node->SetLocalEulerAngleX(pitch);
	_node->SetLocalEulerAngleY(yaw);

	//处理键盘输入
	Vector3 moveDirection(0, 0, 0);
	if (Input::GetKey(GLFW_KEY_W))
	{
		moveDirection = moveDirection + _node->GetLocalForward();
	}
	if (Input::GetKey(GLFW_KEY_A))
	{
		moveDirection = moveDirection + _node->GetLocalRight();
	}
	if (Input::GetKey(GLFW_KEY_S))
	{
		moveDirection = moveDirection - _node->GetLocalForward();
	}
	if (Input::GetKey(GLFW_KEY_D))
	{
		moveDirection = moveDirection - _node->GetLocalRight();
	}

	if (Input::GetKey(GLFW_KEY_SPACE))
	{
		moveDirection = moveDirection + _node->GetLocalUp();
	}
	if (Input::GetKey(GLFW_KEY_LEFT_SHIFT))
	{
		moveDirection = moveDirection - _node->GetLocalUp();
	}
	if (!(moveDirection == Vector3(0, 0, 0)))
	{
		moveDirection = moveDirection.Normalized();
		_node->LocalPosition = _node->LocalPosition + moveDirection * _movementSpeed * Time::GetDeltaTime();
	}
}