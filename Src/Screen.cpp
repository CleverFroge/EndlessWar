#include "FrogeEngine.h"
using namespace FrogeEngine;

int Screen::Width = 800;
int Screen::Height = 600;

void Screen::ScreenSizeChanged(GLFWwindow* window, int width, int height)
{
	Width = width;
	Height = height;
	std::cout << "Window Size Changed:" << width << "*" << height << std::endl;
	Camera::GetCurrentCamera()->AspectRatio = (Width / height);
	glViewport(0, 0, width, height);
}

int Screen::GetWidth()
{
	return Width;
}

int Screen::GetHeight()
{
	return Height;
}