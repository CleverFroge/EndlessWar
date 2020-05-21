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
	EmitAngle = 180;
	_ExistingParticleNum = 0;
	ParticleEmitters.insert(this);

	//���㻺�����
	glGenBuffers(1, &vbo);
	//���������������
	glGenVertexArrays(1, &vao);
	//�󶨶����������
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//��ν�����������
	//glVertexAttribPointer������λ��ֵ���������Դ�С���������ͣ���׼����0~1������������ʼλ��ƫ����
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
	if (_start)
	{
		if (_emittedParticleNum < MaxParticles)
		{
			//��û���ͷų����������
			//�����֡���ɶ��ٸ�����
			unsigned int particleNum = (Time::GetTime() - _startTime) * EmitSpeed - _emittedParticleNum;
			if (particleNum > MaxParticles - _emittedParticleNum)
			{
				particleNum = MaxParticles - _emittedParticleNum;
			}
			Vector3 forward = _node->GetForward();
			Vector3 right = _node->GetRight();
			int index = 0;
			for (size_t i = 0; i < particleNum; i++)
			{
				Particle* particle = new Particle();
				//�������
				Matrix4 rotateMatrix;
				rotateMatrix.Rotate(forward, Random::Randf(0, 360));
				rotateMatrix.Rotate(right, Random::Randf(0, EmitAngle));
				Vector3 emitDirection = forward * rotateMatrix;
				particle->LocalPosition = _node->GetPosition();
				particle->EmitDirection = emitDirection;
				//���Ӵ��ʱ��
				if (_constantLife)
				{
					particle->LifeTime = _minParticleLife;
				}
				else
				{
					particle->LifeTime = Random::Randf(_minParticleLife, _maxParticleLife);
				}
				//�����ٶ�
				if (_constantSpeed)
				{
					particle->ConstantSpeed = true;
					particle->StartSpeed = _startParticleSpeed;
				}
				else
				{
					particle->ConstantSpeed = false;
					particle->StartSpeed = _startParticleSpeed;
					particle->EndSpeed = _endParticleSpeed;
				}
				//������ɫ
				if (_constantColor)
				{
					particle->ConstanColor = true;
					particle->StartColor = _startParticleColor;
				}
				else
				{
					particle->ConstanColor = false;
					particle->StartColor = _startParticleColor;
					particle->EndColor = _endParticleColor;
				}
				Scene::GetCurrentScene()->_root->AddChild(particle);
				particle->Shadow = false;
				//�ҵ���һ����ָ��
				while (index < _particles.size() && _particles[index])
				{
					index++;
				}
				if (index < _particles.size())
				{
					_particles[index] = particle;
				}
				else
				{
					_particles.push_back(particle);
				}
			}
			_emittedParticleNum += particleNum;
			_ExistingParticleNum += particleNum;
		}
		else if (Loop)
		{
			ReStart();
		}
	}
	//������������״̬
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
			particle->LocalPosition = particle->LocalPosition + particle->EmitDirection * Time::GetDeltaTime() * particle->StartSpeed;
			Vector3 cameraBack = Vector3(0, 0, 0) - Scene::GetCurrentScene()->GetCurrentCamera()->GetForward();
			particle->SetLocalForward(cameraBack);
		}
	}
}
void ParticleEmitter::LateUpdate()
{
}

void ParticleEmitter::DrawAllParticles()
{
	//�ϲ���������һ����Ⱦ
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
		float lifeRate = (Time::GetTime() - particle->BornTime) / particle->LifeTime;
		Vector3 color = particle->StartColor + (particle->EndColor - particle->StartColor) * lifeRate;
		for (size_t j = 0; j < mesh->_vertices.size(); j++)
		{
			if (index >= vertices.size())
			{
				ParticleVertex vertex;
				vertices.push_back(vertex);
			}
			vertices[index].Position = mesh->_vertices[j].Position * model;
			vertices[index].Color = color;
			index++;
		}
	}
	if (index ==0)
	{
		return;
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//OpenGL��������ͬʱ�󶨶�����壬ֻҪ�����ǲ�ͬ�Ļ������͡����ǿ���ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleVertex) * index, &vertices[0], GL_STATIC_DRAW);

	Shader* shader = Shader::GetShader("Particle");
	shader->Use();
	//��shader��������������
	Scene* currentScene = Scene::GetCurrentScene();
	Camera* currentCamera = currentScene->GetCurrentCamera();
	shader->SetMat4("view", currentCamera->GetLookAtMatrix());
	shader->SetMat4("projection", currentCamera->GetProjectionMatrix());

	glDrawArrays(GL_TRIANGLES, 0, index);
}

void ParticleEmitter::Start()
{
	if (!_start)
	{
		_startTime = Time::GetTime();
	}
	_start = true;
}

void ParticleEmitter::Stop()
{
	_start = false;
}

void ParticleEmitter::ReStart()
{
	_startTime = Time::GetTime();
	_start = true;
	_emittedParticleNum = 0;
	_ExistingParticleNum = 0;
}

void ParticleEmitter::SetParticleLife(float life)
{
	_constantLife = true;
	_minParticleLife = life;
}

void ParticleEmitter::SetParticleLife(float minLife, float maxLife)
{
	_constantLife = false;
	_minParticleLife = minLife;
	_maxParticleLife = maxLife;
}

void ParticleEmitter::SetParticleColor(Vector3 color)
{
	_constantColor = true;
	_startParticleColor = color;
}
void ParticleEmitter::SetParticleColor(Vector3 startColor, Vector3 endColor)
{
	_constantColor = false;
	_startParticleColor = startColor;
	_endParticleColor = endColor;
}

void ParticleEmitter::SetParticleSpeed(float speed)
{
	_constantSpeed = true;
	_startParticleSpeed = speed;
}

void ParticleEmitter::SetParticleSpeed(float startSpeed, float endSpeed)
{
	_constantSpeed = false;
	_startParticleSpeed = startSpeed;
	_endParticleSpeed = endSpeed;
}