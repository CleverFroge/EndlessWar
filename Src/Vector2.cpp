#include "FrogEngine.h"
using namespace FrogEngine;
#include <iostream>
Vector2::Vector2()
{
}

Vector2::Vector2(float x, float y)
{
	_vec.x = x;
	_vec.y = y;
}

Vector2::~Vector2()
{
}

float* Vector2::ValuePtr()
{
	return glm::value_ptr(_vec);
}

Vector2 Vector2::Normalized()
{
	glm::vec2 normalizedVec = glm::normalize(_vec);
	return Vector2(normalizedVec.x, normalizedVec.y);
}

bool Vector2::operator==(const Vector2& other) const
{
	return _vec == other._vec;
}

Vector2 Vector2::operator+(const Vector2& other) const
{
	glm::vec2 result = _vec + other._vec;
	return Vector2(result.x, result.y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	glm::vec2 result = _vec - other._vec;
	return Vector2(result.x, result.y);
}

Vector2 Vector2::operator*(float scale) const
{
	glm::vec2 result = _vec * scale;
	return Vector2(result.x, result.y);
}

Vector2 Vector2::operator/(float scale) const
{
	glm::vec2 result = _vec / scale;
	return Vector2(result.x, result.y);
}

float Vector2::GetX() const
{
	return _vec.x;
}

float Vector2::GetY() const
{
	return _vec.y;
}

void Vector2::SetX(float x)
{
	_vec.x = x;
}

void Vector2::SetY(float y)
{
	_vec.y = y;
}

glm::vec2 Vector2::Value()
{
	return _vec;
}