#include "FrogEngine.h"
using namespace FrogEngine;

GLFWwindow* Input::_window;
double Input::_mousePosX;
double Input::_mousePosY;
double Input::_mousePosDeltaX;
double Input::_mousePosDeltaY;

bool Input::_mouseLeftClick = false;
bool Input::_mouseMiddleClick = false;
bool Input::_mouseRightClick = false;

Vector2 Input::_axis = Vector2(0, 0);

void Input::SetWindow(GLFWwindow* window)
{
	_window = window;
}

void Input::Update()
{
	//¥¶¿Ìº¸≈Ã ‰»Î
	_axis = Vector2(0, 0);
	if (Input::GetKey(GLFW_KEY_W))
	{
		_axis = _axis + Vector2(0, 1);
	}
	if (Input::GetKey(GLFW_KEY_A))
	{
		_axis = _axis - Vector2(1, 0);
	}
	if (Input::GetKey(GLFW_KEY_S))
	{
		_axis = _axis - Vector2(0, 1);
	}
	if (Input::GetKey(GLFW_KEY_D))
	{
		_axis = _axis + Vector2(1, 0);
	}
	if (!(_axis == Vector2(0, 0)))
	{
		_axis = _axis.Normalized();
	}
}

void Input::UpdateMousePos(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = true;
	if (firstMouse)
	{
		_mousePosX = xpos;
		_mousePosY = ypos;
		firstMouse = false;
		return;
	}
	_mousePosDeltaX = xpos - _mousePosX;
	_mousePosDeltaY = _mousePosY - ypos;

	_mousePosX = xpos;
	_mousePosY = ypos;
}

void Input::MouseClickCallBack(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		_mouseLeftClick = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		_mouseMiddleClick = true;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		_mouseRightClick = true;
		break;
	default:
		return;
	}
	return;
}

void Input::ClearFrameInput()
{
	_mousePosDeltaX = 0;
	_mousePosDeltaY = 0;

	_mouseLeftClick = false;
	_mouseMiddleClick = false;
	_mouseRightClick = false;

	_axis = Vector2(0, 0);
}

double Input::GetMousePosX()
{
	return _mousePosX;
}

double Input::GetMousePosY()
{
	return _mousePosY;
}

double Input::GetMousePosDeltaX()
{
	return _mousePosDeltaX;
}

double Input::GetMousePosDeltaY()
{
	return _mousePosDeltaY;
}

Vector2 Input::GetAxis()
{
	return _axis;
}

bool Input::GetKey(int key)
{
	bool ret = false;
	switch (key)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		ret = _mouseLeftClick;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		ret = _mouseMiddleClick;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		ret = _mouseRightClick;
		break;
	default:
		ret = glfwGetKey(_window, key) == GLFW_PRESS;
		break;
	}
	return ret;
}