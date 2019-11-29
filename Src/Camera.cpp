#include "FrogEngine.h"
using namespace FrogEngine;

Camera* Camera::_currentCamera = nullptr;

void Camera::SetCurrentCamera(Camera* camera)
{
	_currentCamera = camera;
}

Camera* Camera::GetCurrentCamera()
{
	return _currentCamera;
}

Camera::Camera()
{
	_movementSpeed = 2.5;
	_mouseSensitivity = 180;
	_zoom = 45;
	AspectRatio = 800.0 / 600.0;
}

Camera::~Camera()
{
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY)
{
	deltaX *= _mouseSensitivity;
	deltaY *= _mouseSensitivity;

	Vector3 eulerAngle = EulerAngle();
	float pitch = eulerAngle.GetX();
	float yaw = eulerAngle.GetY();
	
	pitch += deltaY;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	yaw += deltaX;

	SetEulerAngleX(pitch);
	SetEulerAngleY(yaw);
}

void Camera::Move(Direction direction, float deltaTime)
{
	Vector3 moveDirection;
	switch (direction)
	{
	case FrogEngine::FRONT:
		moveDirection = Forward();
		moveDirection.SetY(0);
		moveDirection = moveDirection.Normalized();
		break;
	case FrogEngine::BACK:
		moveDirection = Vector3(0, 0, 0) - Forward();
		moveDirection.SetY(0);
		moveDirection = moveDirection.Normalized();
		break;
	case FrogEngine::LEFT:
		moveDirection = Vector3(0, 0, 0) - Right();
		moveDirection.SetY(0);
		moveDirection = moveDirection.Normalized();
		break;
	case FrogEngine::RIGHT:
		moveDirection = Right();
		moveDirection.SetY(0);
		moveDirection = moveDirection.Normalized();
		break;
	case FrogEngine::UP:
		moveDirection = Vector3(0, 1, 0);
		break;
	case FrogEngine::DOWN:
		moveDirection = Vector3(0, -1, 0);
		break;
	default:
		break;
	}
	float velocity = _movementSpeed * deltaTime;
	Vector3 position = Position();
	SetPosition(position + moveDirection * velocity);
}

void Camera::ProcessMouseScroll(float scroll)
{
	if (_zoom >= 1.0f && _zoom <= 45.0f)
		_zoom -= scroll;
	if (_zoom <= 1.0f)
		_zoom = 1.0f;
	if (_zoom >= 45.0f)
		_zoom = 45.0f;
}

Matrix4 Camera::GetProjectionMatrix() const
{
	Matrix4 projection;
	projection.Perspective(45, AspectRatio, 0.1f, 100.0f);
	return projection;
}

Matrix4 Camera::GetLookAtMatrix() const
{
	Matrix4 view;
	view.LookAt(Position(), Position() + Forward(), Up());
	return view;
}