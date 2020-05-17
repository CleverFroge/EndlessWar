#pragma once
#include "FrogEngine.h"
using namespace FrogEngine;

Node* PartilceEmitter::Create()
{
	Node* ret = new Node();
	PartilceEmitter* partilceEmitter = new PartilceEmitter();
	ret->AddComponent(partilceEmitter);
	return ret;
}

PartilceEmitter::PartilceEmitter() : Component{"ParticleEmitter"}
{
	_startTime = 0;
	_emittedParticleNum = 0;
	EmitSpeed = 10;
	MaxParticles = 100;
}

PartilceEmitter::~PartilceEmitter()
{
}

void PartilceEmitter::Awake()
{
}

void PartilceEmitter::Update()
{
	if (_startTime==0)
	{
		_startTime = Time::GetTime();
	}
	if (_emittedParticleNum < MaxParticles)
	{
		//计算该帧生成多少个粒子
		int particleNum = (Time::GetTime() - _startTime) * EmitSpeed - _emittedParticleNum;
		for (size_t i = 0; i < particleNum; i++)
		{
			Particle* particle = new Particle();
			Vector3 emitDirection = Vector3(
				Random::Randf(-1, 1),
				Random::Randf(-1, 1),
				Random::Randf(-1, 1)
			).Normalized();
			particle->EmitDirection = emitDirection;
			particle->SetParent(_node);
			particle->Shadow = false;
			_particles.push_back(particle);

		}
		_emittedParticleNum += particleNum;
		_ExistingParticleNum += particleNum;
	}
	if (_ExistingParticleNum > 0)
	{
		//更新现有粒子状态
		for (size_t i = 0; i < _particles.size(); i++)
		{
			Particle* particle = _particles[i];
			if (!particle)
			{
				continue;
			}
			if (particle->BornTime + particle->LifeTime < Time::GetTime())
			{
				particle->GetParent()->RemoveChild(particle);
				delete particle;
				_particles[i] = nullptr;
				_ExistingParticleNum--;
			}
			else
			{
				particle->LocalPosition = particle->LocalPosition + particle->EmitDirection * Time::GetDeltaTime() * particle->MoveSpeed;
				particle->SetLocalForward(Vector3(0, 0, 0) - Scene::GetCurrentScene()->GetCurrentCamera()->GetForward());
			}
		}
	}
}
void PartilceEmitter::LateUpdate()
{
}