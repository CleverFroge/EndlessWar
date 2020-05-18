#pragma once
#include "FrogEngine.h"
using namespace FrogEngine;

void Ref::Release()
{
	_referenceCount--;
	if (_referenceCount <= 0)
	{
		delete this;
	}
}

void Ref::Retain()
{
	_referenceCount++;
}

int Ref::GetReferenceCount() const
{
	return _referenceCount;
}

void Ref::AutoRelease()
{

}

Ref::Ref()
{
	_referenceCount = 1;
}

Ref::~Ref()
{

}