#include "FrogEngine.h"
using namespace FrogEngine;

GLFWwindow* Input::_window;
double Input::_mousePosX;
double Input::_mousePosY;
double Input::_mousePosDeltaX;
double Input::_mousePosDeltaY;

void Input::SetWindow(GLFWwindow* window)
{
	_window = window;
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

void Input::ClearFrameInput()
{
	_mousePosDeltaX = 0;
	_mousePosDeltaY = 0;
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

bool Input::GetKey(int key)
{
	return glfwGetKey(_window, key) == GLFW_PRESS;
}