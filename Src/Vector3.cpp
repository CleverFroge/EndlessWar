#include "FrogEngine.h"
using namespace FrogEngine;
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
Vector3 Vector3::FRONT(0, 0, -1);
Vector3 Vector3::BEHIND(0, 0, 1);
Vector3 Vector3::LEFT(-1, 0, 0);
Vector3 Vector3::RIGHT(1, 0, 0);
Vector3 Vector3::UP(0, 1, 0);
Vector3 Vector3::DOWN(0, -1, 0);

Vector3::Vector3()
{
}

Vector3::Vector3(glm::vec3 vec)
{
	_vec = vec;
}

Vector3::Vector3(float x, float y, float z)
{
	_vec.x = x;
	_vec.y = y;
	_vec.z = z;
}

Vector3::~Vector3()
{
}

float* Vector3::ValuePtr()
{
	return glm::value_ptr(_vec);
}

Vector3 Vector3::Normalized()
{
	glm::vec3 normalizedVec = glm::normalize(_vec);
	return Vector3(normalizedVec.x, normalizedVec.y, normalizedVec.z);
}

bool Vector3::operator==(const Vector3& other) const
{
	return _vec == other._vec;
}

Vector3 Vector3::operator+(const Vector3& other) const
{
	glm::vec3 result = _vec + other._vec;
	return Vector3(result.x, result.y, result.z);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	glm::vec3 result = _vec - other._vec;
	return Vector3(result.x, result.y, result.z);
}

Vector3 Vector3::operator*(float scale) const
{
	glm::vec3 result = _vec * scale;
	return Vector3(result.x, result.y, result.z);
}

Vector3 Vector3::operator*(const Matrix4& mat) const
{
	glm::vec4 result = mat._mat * glm::vec4(_vec.x, _vec.y, _vec.z, 1);
	return Vector3(result.x, result.y, result.z);
}

Vector3 Vector3::operator/(float scale) const
{
	glm::vec3 result = _vec / scale;
	return Vector3(result.x, result.y, result.z);
}

Vector3 Vector3::Cross(const Vector3& other) const
{
	glm::vec3 result = glm::cross(_vec, other._vec);
	return Vector3(result.x, result.y, result.z);
}

float Vector3::Angle(Vector3 from, Vector3 to)
{
	glm::vec3 fromVec = glm::vec3(from.GetX(), from.GetY(), from.GetZ());
	glm::vec3 toVec = glm::vec3(to.GetX(), to.GetY(), to.GetZ());
	return 0;
	//	return glm::angle(fromVec, toVec);
}

float Vector3::GetX() const
{
	return _vec.x;
}

float Vector3::GetY() const
{
	return _vec.y;
}

float Vector3::GetZ() const
{
	return _vec.z;
}

void Vector3::SetX(float x)
{
	_vec.x = x;
}

void Vector3::SetY(float y)
{
	_vec.y = y;
}

void Vector3::SetZ(float z)
{
	_vec.z = z;
}