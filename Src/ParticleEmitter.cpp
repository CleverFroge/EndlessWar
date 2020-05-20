#pragma once
#include "FrogEngine.h"
using namespace FrogEngine;

std::set<ParticleEmitter*> ParticleEmitter::ParticleEmitters;

Node* ParticleEmitter::Create()
{
	Node* ret = new Node();
	ParticleEmitter* partilceEmitter = new ParticleEmitter();
	ret->AddComponent(partilceEmitter);
	return ret;
}

ParticleEmitter::ParticleEmitter() : Component{"ParticleEmitter"}
{
	_startTime = 0;
	_emittedParticleNum = 0;
	EmitSpeed = 10;
	MaxParticles = 100;
	ParticleEmitters.insert(this);

	//顶点缓冲对象
	glGenBuffers(1, &vbo);
	//创建顶点数组对象
	glGenVertexArrays(1, &vao);
	//绑定顶点数组对象
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//如何解析顶点数据
	//glVertexAttribPointer参数：位置值，顶点属性大小，数据类型，标准化（0~1），步长，起始位置偏移量
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)(offsetof(ParticleVertex, Color)));
	glEnableVertexAttribArray(1);
}

ParticleEmitter::~ParticleEmitter()
{
	ParticleEmitters.erase(this);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void ParticleEmitter::Awake()
{
}

void ParticleEmitter::Update()
{
	if (_startTime==0)
	{
		_startTime = Time::GetTime();
	}
	if (_emittedParticleNum < MaxParticles)
	{
		//计算该帧生成多少个粒子
		int particleNum = (Time::GetTime() - _startTime) * EmitSpeed - _emittedParticleNum;
		if (particleNum > MaxParticles - _emittedParticleNum)
		{
			particleNum = MaxParticles - _emittedParticleNum;
		}
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
				particle->RemoveFromParent();
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
void ParticleEmitter::LateUpdate()
{
}

void ParticleEmitter::DrawAllParticles()
{
	//合并所有粒子一起渲染
	int index = 0;
	for (size_t i = 0; i < _particles.size(); i++)
	{
		Particle* particle = _particles[i];
		if (!particle)
		{
			continue;
		}
		Mesh* mesh = particle->GetMesh(0);
		Matrix4 model = particle->GetModelMatrix();
		for (size_t j = 0; j < mesh->_vertices.size(); j++)
		{
			if (index<vertices.size())
			{
				vertices[index].Position = mesh->_vertices[j].Position * model;
			}
			else
			{
				ParticleVertex vertex;
				vertex.Position = mesh->_vertices[j].Position * model;
				vertices.push_back(vertex);
			}
			index++;
		}
	}
	if (index ==0)
	{
		return;
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。我们可以使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleVertex) * index, &vertices[0], GL_STATIC_DRAW);

	Shader* shader = Shader::GetShader("Particle");
	shader->Use();
	//向shader发送相机相关数据
	Scene* currentScene = Scene::GetCurrentScene();
	Camera* currentCamera = currentScene->GetCurrentCamera();
	shader->SetMat4("view", currentCamera->GetLookAtMatrix());
	shader->SetMat4("projection", currentCamera->GetProjectionMatrix());

	glDrawArrays(GL_TRIANGLES, 0, index);
}