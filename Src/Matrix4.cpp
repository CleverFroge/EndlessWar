#include "FrogEngine.h"
using namespace FrogEngine;
Matrix4::Matrix4()
{
	_mat = glm::mat4(1);
}

Matrix4::Matrix4(glm::mat4 value)
{
	_mat = value;
}

Matrix4::~Matrix4()
{
}

float* Matrix4::ValuePtr()
{
	return glm::value_ptr(_mat);
}

void Matrix4::Translate(const Vector3& offset)
{
	_mat = glm::translate(_mat,
		glm::vec3(offset.GetX(), offset.GetY(), offset.GetZ())
	);
}

void Matrix4::Rotate(const Vector3& axis, float angle)
{
	_mat = glm::rotate(_mat, glm::radians(angle), glm::vec3(axis.GetX(), axis.GetY(), axis.GetZ()));
}

void Matrix4::Scale(const Vector3& scale)
{
	_mat = glm::scale(_mat, glm::vec3(
		scale.GetX(),
		scale.GetY(),
		scale.GetZ()
	));
}

void Matrix4::Scale(float scaleX, float scaleY, float scaleZ)
{
	_mat = glm::scale(_mat, glm::vec3(scaleX, scaleY, scaleZ));
}

void Matrix4::LookAt(Vector3 cameraPos, Vector3 targetPos, Vector3 upDirection)
{
	_mat = glm::lookAt(
		glm::vec3(cameraPos.GetX(), cameraPos.GetY(), cameraPos.GetZ()),
		glm::vec3(targetPos.GetX(), targetPos.GetY(), targetPos.GetZ()),
		glm::vec3(upDirection.GetX(), upDirection.GetY(), upDirection.GetZ())
	);
}

void Matrix4::Perspective(float portAngle, float scale, float near, float far)
{
	_mat = glm::perspective(Radians(portAngle), scale, near, far);
}

glm::vec3 Vector3::Value()
{
	return _vec;
}

glm::mat4 Matrix4::Value()
{
	return _mat;
}