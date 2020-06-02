#include"Explosion.h"
#include "Tank.h"

ExplosionNode::ExplosionNode(SphereCollider::Layer layer, Vector3 pos, float size)
{
	_pos = pos;
	_size = size;
	float scale = std::sqrt(abs(size));
	explositionEmitter = new ParticleEmitter();
	explositionEmitter->EmitSpeed = 2000;
	explositionEmitter->MaxParticles = 500;
	explositionEmitter->EmitAngle = 180;
	explositionEmitter->SetParticleColor(Vector3(1, 0.5, 0), Vector3(0.2, 0.1, 0));
	explositionEmitter->SetParticleLife(0.2 * scale, 0.25 * scale);
	explositionEmitter->SetParticleSpeed(2 * scale, 4 * scale);
	explositionEmitter->SetParticleSize(0.1, 0.2);
	explositionEmitter->Loop = false;
	explositionEmitter->ReleaseAfterEmit = true;
	AddComponent(explositionEmitter);
	LocalPosition = pos;

	SphereCollider* collider = new SphereCollider(layer, size);
	SetCollider(collider);
}

ExplosionNode::~ExplosionNode()
{
}

void ExplosionNode::Start()
{
	explositionEmitter->Start();
}

void ExplosionNode::OnEnter(SphereCollider* collider)
{
	if (_collider->GetLayer() == SphereCollider::Layer::PlayerExplosion && collider->GetLayer() == SphereCollider::Layer::Enemy)
	{
		if (((Tank*)(collider->GetNode()))->GetHealthValue() != 0)
		{
			((Tank*)(collider->GetNode()))->AddHealthValue(-1);

			if (((Tank*)(collider->GetNode()))->GetHealthValue() == 0)
			{
				std::cout << "击杀一名敌人！血量+1！" << std::endl;
				Tank::Player->kill++;
				Tank::Player->AddHealthValue(1);
			}
			else
			{
				std::cout << "击中一名敌人！" << std::endl;
			}
			std::cout << "	当前击杀：" << Tank::Player->kill << std::endl;
			std::cout << "	当前血量：" << Tank::Player->GetHealthValue() << std::endl;
		}
	}
	if (_collider->GetLayer() == SphereCollider::Layer::EnemyExplosion && collider->GetLayer() == SphereCollider::Layer::Player)
	{
		((Tank*)(collider->GetNode()))->AddHealthValue(-1);

		std::cout << "你被击中了！血量 -1！" << std::endl;
		std::cout << "	当前击杀：" << Tank::Player->kill << std::endl;
		std::cout << "	当前血量：" << Tank::Player->GetHealthValue() << std::endl;
	}
}