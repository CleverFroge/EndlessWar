#include "Missile.h"
#include "Explosion.h"

Node* Missile::missile = nullptr;
float Missile::Gravity = -9.8;
float Missile::speed = 60;

Node* Missile::Create(SphereCollider::Layer layer, Vector3 position, Vector3 direction)
{
	if (!missile)
	{
		missile = Model::LoadModel("Resource/Missile/Missile.FBX");
		missile->LocalScale = Vector3(1, 1, 1) * 0.05;
		missile->Retain();
	}
	Missile* ret = new Missile();
	ret->AddChild(missile->Clone());
	MissileComponent* missileComponent = new MissileComponent();
	ret->startVelocity = direction * speed;
	ret->startPos = position;
	ret->AddComponent(missileComponent);
	ret->SetLocalPosition(position);
	ret->SetLocalForward(direction);
	ret->_layer = layer;

	SphereCollider* collider = new SphereCollider(layer, 0.1);
	ret->SetCollider(collider);
	return ret;
}
Missile::Missile() {}
Missile::~Missile() {}
Vector3 Missile::GetPositionAt(float flyTime)
{
	Vector3 pos = startPos + Vector3(
		startVelocity.GetX() * flyTime,
		startVelocity.GetY() * flyTime + Gravity * flyTime * flyTime / 2,
		startVelocity.GetZ() * flyTime
	);
	return pos;
}
Vector3 Missile::GetForwardAt(float flyTime)
{
	Vector3 forward = Vector3(
		startVelocity.GetX(),
		startVelocity.GetY() + flyTime * Gravity,
		startVelocity.GetZ()
	).Normalized();
	return forward;
}

void Missile::OnEnter(SphereCollider* collider)
{
	if (_collider->GetLayer() == SphereCollider::Layer::PlayerMissile && collider->GetLayer() == SphereCollider::Layer::Enemy
		|| _collider->GetLayer() == SphereCollider::Layer::EnemyMissile && collider->GetLayer() == SphereCollider::Layer::Player)
	{
		Explosion();
	}
}

void Missile::Explosion()
{
	SphereCollider::Layer explosionLayer;
	if (_layer == SphereCollider::Layer::PlayerMissile)
	{
		explosionLayer = SphereCollider::Layer::PlayerExplosion;
	}
	else
	{
		explosionLayer = SphereCollider::Layer::EnemyExplosion;
	}
	ExplosionNode* explositionNode = new ExplosionNode(explosionLayer, LocalPosition, 8);
	Scene::GetCurrentScene()->AddObject(explositionNode);
	explositionNode->Start();
	RemoveFromParent();
}

MissileComponent::MissileComponent() : Component{ "CameraController" }
{
}
MissileComponent::~MissileComponent()
{
}
void MissileComponent::Awake()
{
	missile = (Missile*)_node;
	startTime = Time::GetTime();
};

void MissileComponent::Update()
{
	float flyTime = Time::GetTime() - startTime;

	bool explosition = false;
	if (flyTime > lifeTime)
	{
		flyTime = lifeTime;
		explosition = true;
	}

	Vector3 pos = missile->GetPositionAt(flyTime);
	missile->SetLocalPosition(pos);

	Vector3 forward = missile->GetForwardAt(flyTime);
	missile->SetLocalForward(forward);

	if (pos.GetY() <= 0)
	{
		explosition = true;
	}
	if (explosition)
	{
		missile->Explosion();
	}
}

void MissileComponent::LateUpdate()
{
}