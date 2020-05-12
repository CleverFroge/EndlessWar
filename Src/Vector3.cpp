#include "FrogEngine.h"
using namespace FrogEngine;
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
Vector3 Vector3::FRONT(0, 0, 1);
Vector3 Vector3::BEHIND(0, 0, -1);
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

Vector3::Vector3(FbxVector4 vec)
{
	_vec.x = vec[0];
	_vec.y = vec[1];
	_vec.z = vec[2];
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
	/*glm::vec3 fromVec = glm::normalize(glm::vec3(from.GetX(), from.GetY(), from.GetZ()));
	glm::vec3 toVec = glm::normalize(glm::vec3(to.GetX(), to.GetY(), to.GetZ()));
	float dot = glm::dot(fromVec, toVec);
	if (dot > 1)
	{
		dot = 1;
	}
	if (dot < -1)
	{
		dot = -1;
	}
	float res = glm::acos(dot) / 180 * PI;

	glm::vec4 testFrom = glm::vec4(fromVec, 1);
	glm::mat4 rotate;
	rotate = glm::rotate(rotate, res, glm::cross(fromVec, toVec));
	glm::vec4 testTo = rotate * testFrom;
	if (testTo.x * to.GetX() < 0
		|| testTo.y * to.GetY() < 0
		|| testTo.z * to.GetZ() < 0)
	{
		res = -res;
	}
	return res;*/

	glm::vec3 fromVec = glm::normalize(glm::vec3(from.GetX(), from.GetY(), from.GetZ()));
	glm::vec3 toVec = glm::normalize(glm::vec3(to.GetX(), to.GetY(), to.GetZ()));
	float dot = glm::dot(fromVec, toVec);
	if (dot > 1)
	{
		dot = 1;
	}
	if (dot < -1)
	{
		dot = -1;
	}
	float res = glm::acos(dot);
	glm::vec4 testFrom = glm::vec4(fromVec, 1);
	glm::mat4 rotate;
	rotate = glm::rotate(rotate, res, glm::cross(fromVec, toVec));
	glm::vec4 testTo = testFrom * rotate;
	if (toVec.x - testTo.x > 0.01
		|| toVec.y - testTo.y > 0.01
		|| toVec.z - testTo.z > 0.01)
	{
		res = -res;
	}
	return res / PI * 180;
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