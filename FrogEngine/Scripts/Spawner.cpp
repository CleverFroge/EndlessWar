#include "Spawner.h"
#include "Tank.h"
#include "EnemyAI/AITankController.h"

Node* Spawner::Create()
{
	Node* ret = new Node();
	Spawner* spawer = new Spawner();
	ret->AddComponent(spawer);
	return ret;
}
Spawner::Spawner() : Component{ "Spawner" }
{
	_lastGenTime = Time::GetTime();
}
Spawner::~Spawner()
{
}
void Spawner::Awake()
{
}
void Spawner::Update()
{
	int currentEnemyNum = _node->GetChildNum();
	if (_maxEnemy - currentEnemyNum > 0 && Time::GetTime() - _lastGenTime > _cd)
	{
		if (Random::Randf(0, 1) < 0.2)
		{
			GenEnemy();
		}
	}
}
void Spawner::LateUpdate()
{
}
void Spawner::GenEnemy()
{
	Tank* enemy = new Tank(SphereCollider::Layer::Enemy);
	enemy->LocalPosition = Vector3(Random::Randf(-60, 60), 0, Random::Randf(-60, 60));
	_node->AddChild(enemy);

	AITankController* aiController = new AITankController();
	enemy->AddComponent(aiController);

	_lastGenTime = Time::GetTime();
}