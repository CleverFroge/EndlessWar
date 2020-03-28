#include <iostream>
#include "FrogEngine.h"

#include "CameraController.h"
#include "TankController.h"

using namespace FrogEngine;

int main()
{
	//初始化GLFW
	glfwInit();
	//配置OpenGL版本号3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//配置GLFW使用核心模式，Core-profile（只能使用OpenGL功能的一个子集，即没有我们已不再需要的向后兼容特性）
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Mac OS X系统需要以下语句
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	//创建窗口
	GLFWwindow* window = glfwCreateWindow(Screen::GetWidth(), Screen::GetHeight(), "", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	Input::SetWindow(window);
	//将已创建窗口的上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(window);
	//GLAD用来管理OpenGL的函数指针，在调用任何OpenGL的函数之前我们需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//OpenGL渲染窗口的尺寸大小，左下角，长，宽
	glViewport(0, 0, Screen::GetWidth(), Screen::GetHeight());

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//当窗口大小改变时，可能需要一些相应的操作，例如改变视口大小
	//窗口注册一个回调函数，函数原型：void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	glfwSetFramebufferSizeCallback(window, Screen::ScreenSizeChanged);
	//鼠标输入回调函数
	glfwSetCursorPosCallback(window, Input::UpdateMousePos);
	//深度测试
	glEnable(GL_DEPTH_TEST);
	//Blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//添加一个方向光
	auto directionalLight = DirectionalLight::Create(Vector3(1, -1, 1), Vector3(1, 1, 1));
	//shader
	Shader::LoadShader("Phong", "../Shader/Common.vs", "../Shader/BlinnPhong.fs");
	Shader::LoadShader("SkyBox", "../Shader/SkyBox.vs", "../Shader/SkyBox.fs");
	//坦克
	Node* tank = Model::LoadModel("../Resource/Tank1/Tank1.FBX", true);
	tank->SetLocalPosition(0,0,0);
	tank->LocalScale = Vector3(0.01,0.01,0.01);
	Node::ROOT->AddChild(tank);
	Component* tankController = new TankController();
	tank->AddComponent("TankController", tankController);
	//相机
	Camera camera;
	Camera::SetCurrentCamera(&camera);
	camera.SetLocalPosition(0, 0, 0);
	camera.SetLocalEulerAngles(0, 0, 0);
	CameraController* cameraController = new CameraController();
	cameraController->_followObject = tank->Find("Cannon");
	camera.AddComponent("CameraController", cameraController);
	//地形
	Node* model = Model::LoadModel("../Resource/Scene/3/Scene.FBX", false);
	Node::ROOT->AddChild(model);
	//天空盒
	auto skyBox = SkyBox("../Resource/skybox/front.jpg", "../Resource/skybox/back.jpg", "../Resource/skybox/left.jpg", "../Resource/skybox/right.jpg", "../Resource/skybox/top.jpg", "../Resource/skybox/bottom.jpg");
	//天空穷
	Node* skyDome = Model::LoadModel("../Resource/SkyDome/SkyDome.fbx",false);
	skyDome->LocalScale = Vector3(0.03, 0.03, 0.03);
	Shader* waterWaveShader = Shader::LoadShader("WaterWave", "../Shader/WaterWave.vs", "../Shader/WaterWave.fs");
	skyDome->mesh->shader = waterWaveShader;
//	skyDome->mesh->shader = Shader::LoadShader("WaterWave", "../Shader/WaterWave.vs", "../Shader/WaterWave.fs");
	Node::ROOT->AddChild(skyDome);

	unsigned int lastPrint = 0;
	unsigned int FPS = 120;
	float FrameTime = 1 / (float)FPS;
	float FrameStartTime = glfwGetTime();

	//Loop
	while (!glfwWindowShouldClose(window))
	{
		while (glfwGetTime() <FrameStartTime)
		{
		}
		FrameStartTime += FrameTime;
		Time::Update();
		//清除颜色缓冲和深度缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (Time::GetCurrentTime() - lastPrint >= 1)
		{
			lastPrint = Time::GetCurrentTime();
			glfwSetWindowTitle(window, (std::string("Endless War    Fps: ") + std::to_string(1 / Time::GetDeltaTime())).c_str());
		}
		Component::UpdateAllComponents();
		
		skyBox.Draw();
		Node::ROOT->Rendering();

		if (Input::GetKey(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, true);
		}
		Input::ClearFrameInput();
		
		//交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
		glfwSwapBuffers(window);
		//是否触发事件，键盘输入、鼠标移动等、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
		glfwPollEvents();
	}
	//释放/删除之前的分配的所有资源
	glfwTerminate();
	return 0;
}
