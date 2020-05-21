#pragma once
#include "FrogEngine.h"
using namespace FrogEngine;
#include "CameraController.h"
#include "TankController.h"
#include "WaterWaveRendering.h"

class EndlessWar:public Scene
{
public:
	EndlessWar()
	{
		//shader
		Shader::LoadShader("Phong", "../Shader/Common.vs", "../Shader/BlinnPhong.fs");
		Shader::LoadShader("SkyBox", "../Shader/SkyBox.vs", "../Shader/SkyBox.fs");
		Shader::LoadShader("Depth", "../Shader/Depth.vs", "../Shader/Depth.fs");
		Shader::LoadShader("Particle", "../Shader/Particle.vs", "../Shader/Particle.fs");

		//地形
		Node* model = Model::LoadModel("../Resource/Scene/Scene.FBX");
		_root->AddChild(model);

		//	//坦克
		Node* tank = Model::LoadModel("../Resource/Tank1/Tank1.FBX");
		tank->LocalScale = Vector3(1, 1, 1) * 0.1;
		_root->AddChild(tank);

		TankController* tankController = new TankController();
		tank->AddComponent(tankController);
	
		//相机
		Camera* camera = new Camera();
		CameraController* cameraController = new CameraController();
		cameraController->_tank = tank;
		camera->AddComponent(cameraController);
		SetCurrentCamera(camera);
		_root->AddChild(camera);

		//添加一个方向光
		auto directionalLight = DirectionalLight::Create(Vector3(1, -1, 1), Vector3(1, 1, 1));
		directionalLight->center = tank->Find("Cannon");
		AddDirectionalLight(directionalLight);
		//	auto pointLight = PointLight::Create(Vector3(0, 10, 0), Vector3(1, 1, 1), 20);

		//天空盒
		auto skyBox = new SkyBox("../Resource/skybox/front.jpg", "../Resource/skybox/back.jpg", "../Resource/skybox/left.jpg", "../Resource/skybox/right.jpg", "../Resource/skybox/top.jpg", "../Resource/skybox/bottom.jpg");
		camera->SetSkyBox(skyBox);

		//天空穷
		Node* skyDome = Model::LoadModel("../Resource/SkyDome/SkyDome.fbx");
		skyDome->LocalScale = Vector3(0.2, 0.2, 0.2);
		skyDome->SetLocalEulerAngleX(180);
		Shader* waterWaveShader = Shader::LoadShader("WaterWave", "../Shader/WaterWave.vs", "../Shader/WaterWave.fs");
		skyDome->Find("MountainSkybox")->GetMesh(0)->shader = waterWaveShader;
		skyDome->AddComponent(new SkyDome());
		_root->AddChild(skyDome);
	}
	~EndlessWar()
	{

	}

private:

};
