#pragma once
#include "FrogEngine.h"
using namespace FrogEngine;

class SkyDome:public Component
{
public:
	SkyDome();

	~SkyDome();

	void Awake();

	void Update();

	void LateUpdate();
};

