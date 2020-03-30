#include "FrogEngine.h"
using namespace FrogEngine;

Scene* Scene::CurrentScene = nullptr;

Scene::Scene()
{
	_root = new Node();
	_root->name = "root";
}

Scene::~Scene()
{
	delete _root;
	delete _currentCamera;
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
	if (_currentCamera&&_currentCamera->GetSkyBox())
	{
		_currentCamera->GetSkyBox()->Draw();
	}
	
	_root->Rendering();
	_root->UpdateComponents();
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