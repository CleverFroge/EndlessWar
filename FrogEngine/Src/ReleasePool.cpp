#pragma once
#include "FrogEngine.h"
using namespace FrogEngine;

ReleasePool* ReleasePool::_instance = nullptr;

ReleasePool* ReleasePool::GetInstance()
{
	if (!_instance)
	{
		_instance = new ReleasePool();
	}
	return _instance;
}

void ReleasePool::AddObject(Ref* object)
{
	if (!object)
	{
		return;
	}
	_objects.insert(object);
}

void ReleasePool::RemoveObject(Ref* object)
{
	if (_objects.find(object)!=_objects.end())
	{
		_objects.erase(object);
	}
}

void ReleasePool::Clear()
{
	auto it = _objects.begin();
	while (it!=_objects.end())
	{
		Ref* ref = *it;
		if (ref->GetReferenceCount() <= 0)
		{
			it = _objects.erase(it);
			delete ref;
		}
		else
		{
			it++;
		}
	}
}

ReleasePool::ReleasePool()
{
}

ReleasePool::~ReleasePool()
{
}