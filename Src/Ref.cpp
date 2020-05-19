#pragma once
#include "FrogEngine.h"
using namespace FrogEngine;

void Ref::Release()
{
	_referenceCount--;
}

void Ref::Retain()
{
	_referenceCount++;
}

int Ref::GetReferenceCount() const
{
	return _referenceCount;
}

Ref::Ref()
{
	_referenceCount = 0;
	ReleasePool::GetInstance()->AddObject(this);
}

Ref::~Ref()
{
}