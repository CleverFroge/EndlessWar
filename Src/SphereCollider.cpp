#pragma once
#include"FrogEngine.h"
using namespace FrogEngine;
std::set<SphereCollider*> SphereCollider::_colliders;
std::map<SphereCollider::Layer, std::set<SphereCollider::Layer>> SphereCollider::_layerCollision;
SphereCollider::SphereCollider(Layer layer, float size)
{
	_layer = layer;
	_node = nullptr;
	_size = size;
}

SphereCollider::~SphereCollider()
{
	auto it = _collisionColliders.begin();
	while (it!=_collisionColliders.end())
	{
		SphereCollider* collisionCollider = *it;
		collisionCollider->_collisionColliders.erase(this);
		it++;
	}
}

void SphereCollider::Detected()
{
	auto colliderIt = _colliders.begin();
	while (colliderIt!=_colliders.end())
	{
		auto otherColliderIt = colliderIt;
		otherColliderIt++;
		while (otherColliderIt != _colliders.end())
		{
			if (colliderIt!=otherColliderIt)
			{
				SphereCollider* collider = *colliderIt;
				SphereCollider* otherCollider = *otherColliderIt;
				Node* node = collider->_node;
				Node* otherNode = otherCollider->_node;
				if (node && otherNode)
				{
					Vector3 pos = node->GetPosition();
					Vector3 otherPos = otherNode->GetPosition();
					float distance = (pos - otherPos).Length();
					if (distance < collider->_size + otherCollider->_size)
					{
						node->OnCollision(otherCollider);
						otherNode->OnCollision(collider);
						collider->_collisionColliders.insert(otherCollider);
						otherCollider->_collisionColliders.insert(collider);
					}
					else
					{
						collider->_collisionColliders.erase(otherCollider);
						otherCollider->_collisionColliders.erase(collider);
					}
				}
			}
			otherColliderIt++;
		}
		colliderIt++;
	}
}

bool SphereCollider::CanMove(Vector3 direction) const
{
	bool ret = true;
	auto otherColliderIt = _collisionColliders.begin();
	while (otherColliderIt != _collisionColliders.end())
	{
		SphereCollider* otherCollider = *otherColliderIt;

		Layer otherLayer = otherCollider->GetLayer();
		if (_layerCollision.find(_layer) != _layerCollision.end())
		{
			if (_layerCollision[_layer].find(otherLayer) != _layerCollision[_layer].end())
			{
				Node* node = _node;
				Node* otherNode = otherCollider->_node;
				Vector3 pos = node->GetPosition();
				Vector3 otherPos = otherNode->GetPosition();
				if (Vector3::Dot(otherPos - pos, direction) > 0)
				{
					ret = false;
					break;
				}
			}
		}
		otherColliderIt++;
	}
	return ret;
}

SphereCollider::Layer SphereCollider::GetLayer() const
{
	return _layer;
}

Node* SphereCollider::GetNode() const
{
	return _node;
}

void SphereCollider::SetLayerCollsion(Layer layer1, Layer layer2, bool collision)
{
	if (collision)
	{
		if (_layerCollision.find(layer1) == _layerCollision.end())
		{
			std::set<Layer> collisionSet;
			collisionSet.insert(layer2);
			_layerCollision.insert(std::pair<Layer, std::set<Layer>>(layer1, collisionSet));
		}
		else
		{
			_layerCollision[layer1].insert(layer2);
		}
	}
	else
	{
		if (_layerCollision.find(layer1) != _layerCollision.end())
		{
			_layerCollision[layer1].erase(layer2);
		}
	}
	if (collision)
	{
		if (_layerCollision.find(layer2) == _layerCollision.end())
		{
			std::set<Layer> collisionSet;
			collisionSet.insert(layer1);
			_layerCollision.insert(std::pair<Layer, std::set<Layer>>(layer2, collisionSet));

		}
		else
		{
			_layerCollision[layer2].insert(layer1);
		}
	}
	else
	{
		if (_layerCollision.find(layer2) != _layerCollision.end())
		{
			_layerCollision[layer2].erase(layer1);
		}
	}
}