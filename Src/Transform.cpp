#include "FrogEngine.h"
using namespace FrogEngine;

Transform* Transform::root = new Transform();

Transform::Transform()
{
	_scale = Vector3(1, 1, 1);
	SetPosition(0, 0, 0);
	SetEulerAngle(0, 0, 0);
	InitByEulerAngles();
	_parent = root;
}

Transform::~Transform()
{
}

void Transform::SetScale(float x, float y, float z)
{
	_scale = Vector3(x, y, z);
}

void Transform::SetPosition(const Vector3& position)
{
	_position = position;
}

void Transform::SetPosition(float x, float y, float z)
{
	_position = Vector3(x, y, z);
}

void Transform::SetEulerAngle(const Vector3& eulerAngles)
{
	_eulerAngle.SetX(NormalizedAngle(eulerAngles.GetX()));
	_eulerAngle.SetY(NormalizedAngle(eulerAngles.GetY()));
	_eulerAngle.SetZ(NormalizedAngle(eulerAngles.GetZ()));
	InitByEulerAngles();
}

void Transform::SetEulerAngle(float x, float y, float z)
{
	_eulerAngle = Vector3(
		NormalizedAngle(x),
		NormalizedAngle(y),
		NormalizedAngle(z)
	);
	InitByEulerAngles();
}

void Transform::SetEulerAngleX(float eularAngleX)
{
	_eulerAngle.SetX(NormalizedAngle(eularAngleX));
	InitByEulerAngles();
}

void Transform::SetEulerAngleY(float eularAngleY)
{
	_eulerAngle.SetY(NormalizedAngle(eularAngleY));
	InitByEulerAngles();
}

void Transform::SetEulerAngleZ(float eularAngleZ)
{
	_eulerAngle.SetZ(NormalizedAngle(eularAngleZ));
	InitByEulerAngles();
}

void Transform::InitByEulerAngles()
{
	Matrix4 matrix;
	matrix.Rotate(Vector3(1, 0, 0), -_eulerAngle.GetX());
	matrix.Rotate(Vector3(0, 1, 0), _eulerAngle.GetY());
	matrix.Rotate(Vector3(0, 0, 1), _eulerAngle.GetZ());
	_front = Vector3::FRONT * matrix;
	_right = Vector3::RIGHT * matrix;
	_up = Vector3::UP * matrix;
}

const Vector3& Transform::Scale() const
{
	return _scale;
}

const Vector3& Transform::Position() const
{
	return _position;
}

const Vector3& Transform::EulerAngle() const
{
	return _eulerAngle;
}

const Vector3& Transform::Forward() const
{
	return _front;
}

const Vector3& Transform::Up() const
{
	return _up;
}

const Vector3& Transform::Right() const
{
	return _right;
}

void Transform::Print() const
{
	std::cout << "Position:";
	_position.Print();
	std::cout << "Forward:";
	_front.Print();
	std::cout << "Up:";
	_up.Print();
	std::cout << "Right:";
	_right.Print();
	std::cout << "EulerAngles:";
	_eulerAngle.Print();
}

void Transform::SetParent(Transform* parent)
{
	if (parent)
	{
		_parent = parent;
	}
}

void Transform::AddChild(Transform* child)
{
	_childs.insert(child);
}

void Transform::RemoveChild(Transform* child)
{
	_childs.erase(child);
}