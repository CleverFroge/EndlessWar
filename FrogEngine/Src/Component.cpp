#include "FrogEngine.h"
using namespace FrogEngine;

Component::Component(const char* name)
{
	_node = nullptr;
	_name = name;
}

Component::~Component()
{
}