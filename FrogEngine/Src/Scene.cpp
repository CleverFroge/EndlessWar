#include "FrogEngine.h"
using namespace FrogEngine;

Scene* Scene::CurrentScene = nullptr;

Scene::Scene()
{
	_root = new Node();
	_root->Retain();
	_root->name = "root";
}

Scene::~Scene()
{
	_root->Release();
	_currentCamera->Release();
	auto directionalLightIt = _directionalLights.begin();
	while (directionalLightIt != _directionalLights.end())
	{
		delete (*directionalLightIt);
		_directionalLights.erase(directionalLightIt);
	}
	auto pointLightIt = _pointLights.begin();
	while (pointLightIt != _pointLights.end())
	{
		delete (*pointLightIt);
		_pointLights.erase(pointLightIt);
	}
	auto flashLightIt = _flashLights.begin();
	while (flashLightIt != _flashLights.end())
	{
		delete (*flashLightIt);
		_flashLights.erase(flashLightIt);
	}
}

void Scene::SetCurrentScene(Scene* scene)
{
	CurrentScene = scene;
}

Scene* Scene::GetCurrentScene()
{
	return CurrentScene;
}

void Scene::Run()
{
	_root->ComponentsUpdate();
	for (size_t i = 0; i < _directionalLights.size(); i++)
	{
		_directionalLights[i]->GenerateDepthMap();
	}
	glViewport(0, 0, Screen::GetWidth(), Screen::GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (_currentCamera&&_currentCamera->GetSkyBox())
	{
		_currentCamera->GetSkyBox()->Draw();
	}
	_root->ComponentsLateUpdate();
	RenderingScene();
	_root->ComponentsUpdateAfterRendering();
}

void Scene::RenderingScene()
{
	_root->Rendering();
	auto it = ParticleEmitter::ParticleEmitters.begin();
	while (it!= ParticleEmitter::ParticleEmitters.end())
	{
		ParticleEmitter* particleEmitter = *it;
		if ((*it)->_node->IsDescendantOf(_root))
		{
			particleEmitter->DrawAllParticles();
		}
		it++;
	}
}

void Scene::SetCurrentCamera(Camera* camera)
{
	_currentCamera = camera;
}

Camera* Scene::GetCurrentCamera()
{
	return _currentCamera;
}

void Scene::AddDirectionalLight(DirectionalLight* light)
{
	_directionalLights.push_back(light);
}

std::vector<DirectionalLight*>& Scene::GetDirectionalLights()
{
	return _directionalLights;
}

void Scene::AddPointLight(PointLight* light)
{
	_pointLights.push_back(light);
}

std::vector<PointLight*>& Scene::GetPointLights()
{
	return _pointLights;
}

void Scene::AddFlashLight(FlashLight* light)
{
	_flashLights.push_back(light);
}

std::vector<FlashLight*>& Scene::GetFlashLights()
{
	return _flashLights;
}

void Scene::AddObject(Node* node)
{
	_root->AddChild(node);
}