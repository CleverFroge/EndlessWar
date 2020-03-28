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
	
	AspectRatio = 800.0 / 600.0;
}

Camera::~Camera()
{
}
//
//void Camera::ProcessMouseScroll(float scroll)
//{
//	if (_zoom >= 1.0f && _zoom <= 45.0f)
//		_zoom -= scroll;
//	if (_zoom <= 1.0f)
//		_zoom = 1.0f;
//	if (_zoom >= 45.0f)
//		_zoom = 45.0f;
//}

Matrix4 Camera::GetProjectionMatrix() const
{
	Matrix4 projection;
	projection.Perspective(45, AspectRatio, 0.1f, 1000.0f);
	return projection;
}

Matrix4 Camera::GetLookAtMatrix() const
{
	Matrix4 view;
	/*
	GetPosition().Print();
	GetForward().Print();
	GetUp().Print();
	std::cout << std::endl;
	*/
	view.LookAt(GetPosition(), GetPosition() + GetForward(), GetUp());
	return view;
}