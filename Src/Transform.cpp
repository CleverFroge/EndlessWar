#include <stack>
#include "FrogEngine.h"
using namespace FrogEngine;

Node* Node::ROOT = new Node();

Node::Node()
{
	LocalScale = Vector3(1, 1, 1);
	SetLocalPosition(0, 0, 0);
	SetLocalEulerAngles(0, 0, 0);
	InitByEulerAngles();
}

Node::~Node()
{
}

void Node::SetLocalPosition(float x, float y, float z)
{
	LocalPosition = Vector3(x, y, z);
}

void Node::SetLocalEulerAngles(const Vector3& eulerAngles)
{
	_eulerAngles.SetX(NormalizedAngle(eulerAngles.GetX()));
	_eulerAngles.SetY(NormalizedAngle(eulerAngles.GetY()));
	_eulerAngles.SetZ(NormalizedAngle(eulerAngles.GetZ()));
	InitByEulerAngles();
}

void Node::SetLocalEulerAngles(float x, float y, float z)
{
	_eulerAngles = Vector3(
		NormalizedAngle(x),
		NormalizedAngle(y),
		NormalizedAngle(z)
	);
	InitByEulerAngles();
}

void Node::SetLocalEulerAngleX(float eularAngleX)
{
	_eulerAngles.SetX(NormalizedAngle(eularAngleX));
	InitByEulerAngles();
}

void Node::SetLocalEulerAngleY(float eularAngleY)
{
	_eulerAngles.SetY(NormalizedAngle(eularAngleY));
	InitByEulerAngles();
}

void Node::SetLocalEulerAngleZ(float eularAngleZ)
{
	_eulerAngles.SetZ(NormalizedAngle(eularAngleZ));
	InitByEulerAngles();
}

void Node::InitByEulerAngles()
{
	Matrix4 matrix;
	matrix.Rotate(Vector3(0, 0, 1), _eulerAngles.GetZ());
	matrix.Rotate(Vector3(0, 1, 0), _eulerAngles.GetY());
	matrix.Rotate(Vector3(1, 0, 0), _eulerAngles.GetX());
	_front = Vector3::FRONT * matrix;
	_right = Vector3::RIGHT * matrix;
	_up = Vector3::UP * matrix;
}

const Vector3& Node::GetLocalEulerAngles() const
{
	return _eulerAngles;
}

const Vector3& Node::GetLocalForward() const
{
	return _front;
}

const Vector3& Node::GetLocalUp() const
{
	return _up;
}

const Vector3& Node::GetLocalRight() const
{
	return _right;
}

void Node::Print() const
{
	std::cout << "Position:";
	LocalPosition.Print();
	std::cout << "Forward:";
	_front.Print();
	std::cout << "Up:";
	_up.Print();
	std::cout << "Right:";
	_right.Print();
	std::cout << "EulerAngles:";
	_eulerAngles.Print();
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

			model.Translate(node->LocalPosition);
			model.Rotate(Vector3(0, 0, 1), node->_eulerAngles.GetZ());
			model.Rotate(Vector3(0, 1, 0), node->_eulerAngles.GetY());
			model.Rotate(Vector3(1, 0, 0), -node->_eulerAngles.GetX());
			model.Scale(node->LocalScale);
		}
		
		shader->SetMat4("model", model);

		//向shader发送相机相关数据
		Camera* currentCamera = Camera::GetCurrentCamera();
		shader->SetVector3("viewPos", currentCamera->GetPosition());
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

Node* Node::Find(const char* searchName) const
{
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

Vector3 Node::GetPosition() const
{
	Node* it = (Node*)this->_parent;
	Vector3 res = LocalPosition;
	while (it)
	{
		Matrix4 s;
		s.Scale(it->LocalScale);
		res = res * s;
		Matrix4 rx;
		rx.Rotate(Vector3(1, 0, 0), -it->_eulerAngles.GetX());
		res = res * rx;
		Matrix4 ry;
		ry.Rotate(Vector3(0, 1, 0), it->_eulerAngles.GetY());
		res = res * ry;
		Matrix4 rz;
		rz.Rotate(Vector3(0, 0, 1), it->_eulerAngles.GetZ());
		res = res * rz;
		Matrix4 t;
		t.Translate(it->LocalPosition);
		res = res * t;
		it = it->_parent;
	}
	return res;
}

Vector3 Node::GetForward() const
{
	Node* it = (Node*)this->_parent;
	Vector3 front = _front;
	Vector3 ori(0, 0, 0);
	while (it)
	{
		Matrix4 s;
		s.Scale(it->LocalScale);
		front = front * s;
		ori = ori * s;
		Matrix4 rx;
		rx.Rotate(Vector3(1, 0, 0), -it->_eulerAngles.GetX());
		front = front * rx;
		ori = ori * rx;
		Matrix4 ry;
		ry.Rotate(Vector3(0, 1, 0), it->_eulerAngles.GetY());
		front = front * ry;
		ori = ori * ry;
		Matrix4 rz;
		rz.Rotate(Vector3(0, 0, 1), it->_eulerAngles.GetZ());
		front = front * rz;
		ori = ori * rz;
		Matrix4 t;
		t.Translate(it->LocalPosition);
		front = front * t;
		ori = ori * t;
		it = it->_parent;
	}
	return front-ori;
}

Vector3 Node::GetUp() const
{
	Node* it = (Node*)this->_parent;
	Vector3 up = _up;
	Vector3 ori(0, 0, 0);
	while (it)
	{
		Matrix4 s;
		s.Scale(it->LocalScale);
		up = up * s;
		ori = ori * s;
		Matrix4 rx;
		rx.Rotate(Vector3(1, 0, 0), -it->_eulerAngles.GetX());
		up = up * rx;
		ori = ori * rx;
		Matrix4 ry;
		ry.Rotate(Vector3(0, 1, 0), it->_eulerAngles.GetY());
		up = up * ry;
		ori = ori * ry;
		Matrix4 rz;
		rz.Rotate(Vector3(0, 0, 1), it->_eulerAngles.GetZ());
		up = up * rz;
		ori = ori * rz;
		Matrix4 t;
		t.Translate(it->LocalPosition);
		up = up * t;
		ori = ori * t;
		it = it->_parent;
	}
	return up - ori;
}

Vector3 Node::GetRight() const
{
	Node* it = (Node*)this->_parent;
	Vector3 right = _right;
	Vector3 ori(0, 0, 0);
	while (it)
	{
		Matrix4 s;
		s.Scale(it->LocalScale);
		right = right * s;
		ori = ori * s;
		Matrix4 rx;
		rx.Rotate(Vector3(1, 0, 0), -it->_eulerAngles.GetX());
		right = right * rx;
		ori = ori * rx;
		Matrix4 ry;
		ry.Rotate(Vector3(0, 1, 0), it->_eulerAngles.GetY());
		right = right * ry;
		ori = ori * ry;
		Matrix4 rz;
		rz.Rotate(Vector3(0, 0, 1), it->_eulerAngles.GetZ());
		right = right * rz;
		ori = ori * rz;
		Matrix4 t;
		t.Translate(it->LocalPosition);
		right = right * t;
		ori = ori * t;
		it = it->_parent;
	}
	return right - ori;
}