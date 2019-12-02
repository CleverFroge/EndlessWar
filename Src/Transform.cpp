#include <stack>
#include "FrogEngine.h"
using namespace FrogEngine;

Node* Node::ROOT = new Node();

Node::Node()
{
	_scale = Vector3(1, 1, 1);
	SetPosition(0, 0, 0);
	SetEulerAngle(0, 0, 0);
	InitByEulerAngles();
}

Node::~Node()
{
}

void Node::SetScale(float x, float y, float z)
{
	_scale = Vector3(x, y, z);
}

void Node::SetPosition(const Vector3& position)
{
	_position = position;
}

void Node::SetPosition(float x, float y, float z)
{
	_position = Vector3(x, y, z);
}

void Node::SetEulerAngle(const Vector3& eulerAngles)
{
	_eulerAngle.SetX(NormalizedAngle(eulerAngles.GetX()));
	_eulerAngle.SetY(NormalizedAngle(eulerAngles.GetY()));
	_eulerAngle.SetZ(NormalizedAngle(eulerAngles.GetZ()));
	InitByEulerAngles();
}

void Node::SetEulerAngle(float x, float y, float z)
{
	_eulerAngle = Vector3(
		NormalizedAngle(x),
		NormalizedAngle(y),
		NormalizedAngle(z)
	);
	InitByEulerAngles();
}

void Node::SetEulerAngleX(float eularAngleX)
{
	_eulerAngle.SetX(NormalizedAngle(eularAngleX));
	InitByEulerAngles();
}

void Node::SetEulerAngleY(float eularAngleY)
{
	_eulerAngle.SetY(NormalizedAngle(eularAngleY));
	InitByEulerAngles();
}

void Node::SetEulerAngleZ(float eularAngleZ)
{
	_eulerAngle.SetZ(NormalizedAngle(eularAngleZ));
	InitByEulerAngles();
}

void Node::InitByEulerAngles()
{
	Matrix4 matrix;
	matrix.Rotate(Vector3(1, 0, 0), -_eulerAngle.GetX());
	matrix.Rotate(Vector3(0, 1, 0), _eulerAngle.GetY());
	matrix.Rotate(Vector3(0, 0, 1), _eulerAngle.GetZ());
	_front = Vector3::FRONT * matrix;
	_right = Vector3::RIGHT * matrix;
	_up = Vector3::UP * matrix;
}

const Vector3& Node::Scale() const
{
	return _scale;
}

const Vector3& Node::Position() const
{
	return _position;
}

const Vector3& Node::EulerAngle() const
{
	return _eulerAngle;
}

const Vector3& Node::Forward() const
{
	return _front;
}

const Vector3& Node::Up() const
{
	return _up;
}

const Vector3& Node::Right() const
{
	return _right;
}

void Node::Print() const
{
	std::cout << "Position:";
	_position.Print();
	std::cout << "Forward:";
	_front.Print();
	std::cout << "Up:";
	_up.Print();
	std::cout << "Right:";
	_right.Print();
	std::cout << "EulerAngles:";
	_eulerAngle.Print();
}

void Node::SetParent(Node* parent)
{
	if (parent)
	{
		_parent = parent;
		parent->_childs.insert(this);
	}
}

void Node::AddChild(Node* child)
{
	if (child)
	{
		_childs.insert(child);
		child->_parent = this;
	}
}

void Node::RemoveChild(Node* child)
{
	_childs.erase(child);
}

void Node::Rendering()
{
	if (mesh)
	{
		Shader* shader = mesh->shader;
		Material* material = mesh->material;
		if (!shader)
		{
			shader = Shader::GetShader("Phong");
		}
		//使用该着色器程序
		shader->Use();
		//向shader发送材质数据
		if (material)
		{
			unsigned int textureIndex = 1;
			if (material->diffuseTexture)
			{
				shader->SetBool("material.alpha", material->diffuseTexture->Alpha);
				shader->SetInt("material.diffuse", textureIndex);
				glActiveTexture(GL_TEXTURE0 + textureIndex);
				glBindTexture(GL_TEXTURE_2D, material->diffuseTexture->ID);
				textureIndex++;
			}
			if (material->specularTexture)
			{
				shader->SetInt("material.specular", textureIndex);
				glActiveTexture(GL_TEXTURE0 + textureIndex);
				glBindTexture(GL_TEXTURE_2D, material->specularTexture->ID);
				textureIndex++;
			}
			if (material->normalTexture)
			{
				shader->SetInt("material.normal", textureIndex);
				glActiveTexture(GL_TEXTURE0 + textureIndex);
				glBindTexture(GL_TEXTURE_2D, material->normalTexture->ID);
				textureIndex++;
			}
			if (material->displacementTexture)
			{
				shader->SetInt("material.displacement", textureIndex);
				glActiveTexture(GL_TEXTURE0 + textureIndex);
				glBindTexture(GL_TEXTURE_2D, material->displacementTexture->ID);
				textureIndex++;
			}
			shader->SetFloat("material.shininess", material->shininess);
		}
		//向shader发送model矩阵
		Matrix4 model;
		std::stack<Node*> temp;
		Node* node = this;
		while (node->_parent != nullptr)
		{
			temp.push(node);
			node = node->_parent;
		}
		while (temp.size()!=0)
		{
			node = temp.top();
			temp.pop();

			model.Translate(node->_position);
			model.Rotate(Vector3(0, 0, 1), node->_eulerAngle.GetZ());
			model.Rotate(Vector3(0, 1, 0), node->_eulerAngle.GetY());
			model.Rotate(Vector3(1, 0, 0), -node->_eulerAngle.GetX());
			model.Scale(node->_scale);
		}
		shader->SetMat4("model", model);

		//向shader发送相机相关数据
		Camera* currentCamera = Camera::GetCurrentCamera();
		shader->SetVector3("viewPos", currentCamera->Position());
		shader->SetMat4("view", currentCamera->GetLookAtMatrix());
		shader->SetMat4("projection", currentCamera->GetProjectionMatrix());

		//向shader发送光源相关数据
		//方向光
		auto directionalLights = DirectionalLight::GetLights();
		shader->SetInt("directionalLightNum", directionalLights.size());
		for (size_t i = 0; i < directionalLights.size(); i++)
		{
			std::string name = std::string("directionalLights[") + std::to_string(i) + std::string("]");
			shader->SetVector3((name + std::string(".direction")).c_str(), directionalLights[i]->direction);
			shader->SetVector3((name + std::string(".color")).c_str(), directionalLights[i]->color);
		}
		//点光源
		auto pointLights = PointLight::GetLights();
		shader->SetInt("pointLightNum", pointLights.size());
		for (size_t i = 0; i < pointLights.size(); i++)
		{
			std::string name = std::string("pointLights[") + std::to_string(i) + std::string("]");
			shader->SetVector3((name + std::string(".position")).c_str(), pointLights[i]->position);
			shader->SetVector3((name + std::string(".color")).c_str(), pointLights[i]->color);
			shader->SetFloat((name + std::string(".constant")).c_str(), pointLights[i]->constant);
			shader->SetFloat((name + std::string(".linear")).c_str(), pointLights[i]->linear);
			shader->SetFloat((name + std::string(".quadratic")).c_str(), pointLights[i]->quadratic);
		}
		//聚光
		auto flashLights = FlashLight::GetLights();
		shader->SetInt("flashLightNum", flashLights.size());
		for (size_t i = 0; i < flashLights.size(); i++)
		{
			std::string name = std::string("flashLights[") + std::to_string(i) + std::string("]");
			shader->SetVector3((name + std::string(".position")).c_str(), flashLights[i]->position);
			shader->SetVector3((name + std::string(".direction")).c_str(), flashLights[i]->direction);
			shader->SetVector3((name + std::string(".color")).c_str(), flashLights[i]->color);
			shader->SetFloat((name + std::string(".innerCone")).c_str(), flashLights[i]->innerCone);
			shader->SetFloat((name + std::string(".outerCone")).c_str(), flashLights[i]->outerCone);
			shader->SetFloat((name + std::string(".constant")).c_str(), flashLights[i]->constant);
			shader->SetFloat((name + std::string(".linear")).c_str(), flashLights[i]->linear);
			shader->SetFloat((name + std::string(".quadratic")).c_str(), flashLights[i]->quadratic);
		}
		mesh->Draw();
	}
	
	for (auto it = _childs.begin(); it!=_childs.end(); it++)
	{
		(*it)->Rendering();
	}
}

Node* Node::Find(std::string& searchName) const
{
	/*
	if (searchName ==name)
	{
		return (Node*)this;
	}
	for (auto it = _childs.begin(); it != _childs.end(); it++)
	{
		Node* subRet = (*it)->Find(searchName);
		if (subRet)
		{
			return subRet;
		}
	}
	return nullptr;
	*/
	
	for (auto it = _childs.begin(); it != _childs.end(); it++)
	{
		if ((*it)->name==searchName&&(*it)->mesh)
		{
			return *it;
		}
		Node* subRet = (*it)->Find(searchName);
		if (subRet)
		{
			return subRet;
		}
	}
	return nullptr;
	
}