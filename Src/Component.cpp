#include "FrogEngine.h"
using namespace FrogEngine;

std::set<Component*> Component::components;

Component::Component()
{
	_node = nullptr;
}

Component::~Component()
{
}
//调用所有组件的Update
void Component::UpdateAllComponents()
{
	auto it = components.begin();
	for (auto it = components.begin(); it != components.end(); it++)
	{
		auto component = *it;
		component->Update();
	}
}

void Component::AddToUpdatePool()
{
	components.insert(this);
}

void Component::RemoveFromUpdatePool()
{
	components.erase(this);
}