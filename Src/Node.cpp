#include <stack>
#include "FrogEngine.h"
using namespace FrogEngine;

Node::Node()
{
	LocalScale = Vector3(1, 1, 1);
	SetLocalPosition(0, 0, 0);
	SetLocalEulerAngles(0, 0, 0);
	InitByEulerAngles();
	AutoRendering = true;
	Shadow = true;
}

Node* Node::Clone() const
{
	//复制节点属性
	Node* ret = new Node();
	ret->name = name;
	for (size_t i = 0; i < meshs.size(); i++)
	{
		ret->AddMesh(meshs[i]);
	}
	ret->geomerty = geomerty;

	ret->_eulerAngles = _eulerAngles;
	ret->_up = _up;
	ret->_front = _front;
	ret->_right = _right;
	ret->LocalScale = LocalScale;
	ret->LocalPosition = LocalPosition;
	ret->AutoRendering = AutoRendering;
	ret->Shadow = Shadow;

	//添加子节点
	auto childIt = _childs.begin();
	while (childIt!=_childs.end())
	{
		Node* child = *childIt;
		ret->AddChild(child->Clone());
		childIt++;
	}
	return ret;
}

Node::~Node()
{
	//释放Mesh
	for (size_t i = 0; i < meshs.size(); i++)
	{
		meshs[i]->Release();
	}
	//删除节点上挂在的组件
	auto componentIt = _components.begin();
	while (componentIt != _components.end())
	{
		Component* component = componentIt->second;
		delete component;
		componentIt = _components.erase(componentIt);
	}
	//删除所有子节点
	auto childIt = _childs.begin();
	while (childIt!=_childs.end())
	{
		(*childIt)->Release();
		childIt = _childs.erase(childIt);
	}
}

void Node::AddComponent(Component* component)
{
	component->_node = this;
	component->Awake();
	_components.insert(std::pair<const char*, Component*>(component->_name, component));
}

Component* Node::GetComponent(const char* name)
{
	auto ret = _components[name];
	return ret;
}

void Node::SetLocalPosition(const Vector3& pos)
{
	LocalPosition = pos;
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

void Node::SetLocalForward(const Vector3& forward)
{
	Vector3 forwardInXZ = forward;
	forwardInXZ.SetY(0);
	float eulerAngleX = Vector3::Angle(forwardInXZ, forward);
	_eulerAngles.SetX(eulerAngleX);
	float eulerAngleY = Vector3::Angle(Vector3::FRONT, forwardInXZ);
	_eulerAngles.SetY(-eulerAngleY);
	_eulerAngles.SetZ(0);

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
	Matrix4 rx;
	rx.Rotate(Vector3(1, 0, 0), _eulerAngles.GetX());
	_front = Vector3::FRONT * rx;
	_up = Vector3::UP * rx;
	_right = Vector3::RIGHT * rx;
	Matrix4 ry;
	ry.Rotate(Vector3(0, 1, 0), _eulerAngles.GetY());
	_front = _front * ry;
	_up = _up * ry;
	_right = _right * ry;
	Matrix4 rz;
	rz.Rotate(_front, _eulerAngles.GetZ());
	_front = _front * rz;
	_up = _up * rz;
	_right = _right * rz;
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
	std::cout << name << std::endl;
	std::cout << "	Position:";
	GetPosition().Print();
	std::cout << "	Forward:";
	GetForward().Print();
	std::cout << "	Up:";
	GetUp().Print();
	std::cout << "	Right:";
	GetRight().Print();
	std::cout << "	EulerAngles:";
	_eulerAngles.Print();
}

void Node::SetParent(Node* parent)
{
	if (parent)
	{
		if (!_parent)
		{
			Retain();
		}
		_parent = parent;
		parent->_childs.insert(this);
	}
}

Node* Node::GetParent() const
{
	return _parent;
}

void Node::AddChild(Node* child)
{
	if (child)
	{
		if (!child->_parent)
		{
			child->Retain();
		}
		_childs.insert(child);
		child->_parent = this;
	}
}

void Node::RemoveChild(Node* child)
{
	if (_childs.find(child)!=_childs.end())
	{
		_childs.erase(child);
		child->_parent = nullptr;
		child->Release();
	}
}

void Node::RemoveFromParent()
{
	if (_parent)
	{
		_parent->_childs.erase(this);
		_parent = nullptr;
		Release();
	}
}

void Node::Rendering()
{
	for (size_t i = 0; i < meshs.size(); i++)
	{
		Mesh* mesh = meshs[i];
		Shader* shader = mesh->shader;
		Material* material = mesh->material;
		if (!shader)
		{
			shader = Shader::GetShader("Phong");
		}
		//使用该着色器程序
		shader->Use();
		unsigned int textureIndex = 1;
		//向shader发送材质数据
		if (material)
		{
			Texture2D* diffuseTexture = material->GetDiffuseTexture();
			if (diffuseTexture)
			{
				shader->SetBool("material.haveDiffuse", true);
				shader->SetBool("material.alpha", diffuseTexture->Alpha);
				shader->SetInt("material.diffuse", textureIndex);
				glActiveTexture(GL_TEXTURE0 + textureIndex);
				glBindTexture(GL_TEXTURE_2D, diffuseTexture->ID);
				textureIndex++;
			}
			else
			{
				shader->SetBool("material.haveDiffuse", false);
			}
			Texture2D* specularTexture = material->GetSpecularTexture();
			if (specularTexture)
			{
				shader->SetBool("material.haveSpecular", true);
				shader->SetInt("material.specular", textureIndex);
				glActiveTexture(GL_TEXTURE0 + textureIndex);
				glBindTexture(GL_TEXTURE_2D, specularTexture->ID);
				textureIndex++;
			}
			else
			{
				shader->SetBool("material.haveSpecular", false);
			}
			Texture2D* normalTexture = material->GetNormalTexture();
			if (normalTexture)
			{
				shader->SetBool("material.haveNormal", true);
				shader->SetInt("material.normal", textureIndex);
				glActiveTexture(GL_TEXTURE0 + textureIndex);
				glBindTexture(GL_TEXTURE_2D, normalTexture->ID);
				textureIndex++;
			}
			else
			{
				shader->SetBool("material.haveNormal", false);
			}
			shader->SetFloat("material.shininess", material->shininess);
		}
		shader->SetMat4("geometry", geomerty);
		//向shader发送model矩阵
		Matrix4 model;
		std::stack<Node*> temp;
		Node* node = this;
		while (node->_parent != nullptr)
		{
			temp.push(node);
			node = node->_parent;
		}
		while (temp.size() != 0)
		{
			node = temp.top();
			temp.pop();

			model.Translate(node->LocalPosition);
			model.Rotate(node->_front, node->_eulerAngles.GetZ());
			model.Rotate(Vector3(0, 1, 0), node->_eulerAngles.GetY());
			model.Rotate(Vector3(1, 0, 0), node->_eulerAngles.GetX());
			model.Scale(node->LocalScale);
		}
		shader->SetMat4("model", model);

		//向shader发送相机相关数据
		Scene* currentScene = Scene::GetCurrentScene();
		Camera* currentCamera = currentScene->GetCurrentCamera();
		shader->SetVector3("viewPos", currentCamera->GetPosition());
		shader->SetMat4("view", currentCamera->GetLookAtMatrix());
		shader->SetMat4("projection", currentCamera->GetProjectionMatrix());

		//向shader发送光源相关数据
		//方向光
		auto directionalLights = currentScene->GetDirectionalLights();
		shader->SetInt("directionalLightNum", directionalLights.size());
		for (size_t i = 0; i < directionalLights.size(); i++)
		{
			std::string name = std::string("directionalLights[") + std::to_string(i) + std::string("]");
			shader->SetVector3((name + std::string(".direction")).c_str(), directionalLights[i]->direction);
			shader->SetVector3((name + std::string(".color")).c_str(), directionalLights[i]->color);
			shader->SetMat4((name + std::string(".lighgtSpaceMat")).c_str(), directionalLights[i]->lightSpaceMatrix);

			shader->SetInt((name + std::string(".depthMap")).c_str(), textureIndex);
			glActiveTexture(GL_TEXTURE0 + textureIndex);
			glBindTexture(GL_TEXTURE_2D, directionalLights[i]->depthMap);
			textureIndex++;
		}
		//点光源
		auto pointLights = currentScene->GetPointLights();
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
		auto flashLights = currentScene->GetFlashLights();
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
		if ((*it)->AutoRendering)
		{
			(*it)->Rendering();
		}
	}
}

void Node::DepthRendering(Matrix4 lightSpaceMatrix)
{
	if (!Shadow)
	{
		return;
	}
	for (size_t i = 0; i < meshs.size(); i++)
	{
		Mesh* mesh = meshs[i];
		Shader* shader = Shader::GetShader("Depth");

		//使用该着色器程序
		shader->Use();
		//向shader发送model矩阵
		Matrix4 model;
		std::stack<Node*> temp;
		Node* node = this;
		while (node->_parent != nullptr)
		{
			temp.push(node);
			node = node->_parent;
		}
		while (temp.size() != 0)
		{
			node = temp.top();
			temp.pop();

			model.Translate(node->LocalPosition);
			model.Rotate(node->_front, node->_eulerAngles.GetZ());
			model.Rotate(Vector3(0, 1, 0), node->_eulerAngles.GetY());
			model.Rotate(Vector3(1, 0, 0), node->_eulerAngles.GetX());
			model.Scale(node->LocalScale);
		}
		shader->SetMat4("model", model);
		shader->SetMat4("geometry", geomerty);
		//向Shader发送转换到光源空间的矩阵
		shader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

		mesh->Draw();
	}

	for (auto it = _childs.begin(); it != _childs.end(); it++)
	{
		if ((*it)->AutoRendering)
		{
			(*it)->DepthRendering(lightSpaceMatrix);
		}
	}
}

Node* Node::Find(const char* searchName) const
{
	for (auto it = _childs.begin(); it != _childs.end(); it++)
	{
		if ((*it)->name==searchName&&(*it)->meshs.size()!=0)
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
		rx.Rotate(Vector3(1, 0, 0), it->_eulerAngles.GetX());
		res = res * rx;
		Matrix4 ry;
		ry.Rotate(Vector3(0, 1, 0), it->_eulerAngles.GetY());
		res = res * ry;
		Matrix4 rz;
		rz.Rotate(it->_front, it->_eulerAngles.GetZ());
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
		rx.Rotate(Vector3(1, 0, 0), it->_eulerAngles.GetX());
		front = front * rx;
		ori = ori * rx;
		Matrix4 ry;
		ry.Rotate(Vector3(0, 1, 0), it->_eulerAngles.GetY());
		front = front * ry;
		ori = ori * ry;
		Matrix4 rz;
		rz.Rotate(it->_front, it->_eulerAngles.GetZ());
		front = front * rz;
		ori = ori * rz;
		Matrix4 t;
		t.Translate(it->LocalPosition);
		front = front * t;
		ori = ori * t;
		it = it->_parent;
	}
	return (front - ori).Normalized();
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
		rx.Rotate(Vector3(1, 0, 0), it->_eulerAngles.GetX());
		up = up * rx;
		ori = ori * rx;
		Matrix4 ry;
		ry.Rotate(Vector3(0, 1, 0), it->_eulerAngles.GetY());
		up = up * ry;
		ori = ori * ry;
		Matrix4 rz;
		rz.Rotate(it->_front, it->_eulerAngles.GetZ());
		up = up * rz;
		ori = ori * rz;
		Matrix4 t;
		t.Translate(it->LocalPosition);
		up = up * t;
		ori = ori * t;
		it = it->_parent;
	}
	return (up - ori).Normalized();
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
		rx.Rotate(Vector3(1, 0, 0), it->_eulerAngles.GetX());
		right = right * rx;
		ori = ori * rx;
		Matrix4 ry;
		ry.Rotate(Vector3(0, 1, 0), it->_eulerAngles.GetY());
		right = right * ry;
		ori = ori * ry;
		Matrix4 rz;
		rz.Rotate(it->_front, it->_eulerAngles.GetZ());
		right = right * rz;
		ori = ori * rz;
		Matrix4 t;
		t.Translate(it->LocalPosition);
		right = right * t;
		ori = ori * t;
		it = it->_parent;
	}
	return (right - ori).Normalized();
}

void Node::ComponentsUpdate()
{
	for (auto it = _components.begin(); it != _components.end(); it++)
	{
		auto component = it->second;
		component->Update();
	}
	for (auto it = _childs.begin(); it != _childs.end(); it++)
	{
		(*it)->ComponentsUpdate();
	}
}

void Node::ComponentsLateUpdate()
{
	for (auto it = _components.begin(); it != _components.end(); it++)
	{
		auto component = it->second;
		component->LateUpdate();
	}
	for (auto it = _childs.begin(); it != _childs.end(); it++)
	{
		(*it)->ComponentsLateUpdate();
	}
}

void Node::AddMesh(Mesh* mesh)
{
	if (!mesh)
	{
		return;
	}
	mesh->Retain();
	meshs.push_back(mesh);
}

Mesh* Node::GetMesh(int index)
{
	if (index>=meshs.size())
	{
		return nullptr;
	}
	return meshs[index];
}