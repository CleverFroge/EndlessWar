#include "FrogEngine.h"
using namespace FrogEngine;

Camera::Camera()
{
	
	AspectRatio = 800.0 / 600.0;
}

Camera::~Camera()
{
	if (_skyBox)
	{
		delete _skyBox;
	}
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

void Camera::SetSkyBox(SkyBox* skyBox)
{
	if (_skyBox)
	{
		delete _skyBox;
	}
	_skyBox = skyBox;
}

SkyBox* Camera::GetSkyBox() const
{
	return _skyBox;
}