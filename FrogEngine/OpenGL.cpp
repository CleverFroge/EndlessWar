#include <iostream>
#include <glad/glad.h> 
#include <glfw3.h>
#include "FrogEngine.h"

using namespace FrogEngine;

Camera camera;

void CameraUpdate()
{
	/*camera.ProcessMouseMovement(-Input::GetMousePosDeltaX() / Screen::GetWidth(), Input::GetMousePosDeltaY() / Screen::GetHeight());
	if (Input::GetKey(GLFW_KEY_W))
	{
		camera.Move(Direction::FRONT, Time::GetDeltaTime());
	}
	if (Input::GetKey(GLFW_KEY_A))
	{
		camera.Move(Direction::LEFT, Time::GetDeltaTime());
	}
	if (Input::GetKey(GLFW_KEY_S))
	{
		camera.Move(Direction::BACK, Time::GetDeltaTime());
	}
	if (Input::GetKey(GLFW_KEY_D))
	{
		camera.Move(Direction::RIGHT, Time::GetDeltaTime());
	}*/

	if (Input::GetKey(GLFW_KEY_SPACE))
	{
		camera.Move(Direction::UP, Time::GetDeltaTime());
	}
	if (Input::GetKey(GLFW_KEY_LEFT_SHIFT))
	{
		camera.Move(Direction::DOWN, Time::GetDeltaTime());
	}
}

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

	Camera::SetCurrentCamera(&camera);

	auto directionalLight = DirectionalLight::Create(Vector3(1, -1, 1), Vector3(1, 1, 1));
//	auto pointLight = PointLight::Create(Vector3(0, 0, 3), Vector3(1, 1, 1), 50);
//	auto flashLight = FlashLight::Create(Vector3(0, 0, 0), Vector3(0, 0, -1), Vector3(1, 1, 1), 15, 20);
	Shader::LoadShader("Phong", "../Shader/Common.vs", "../Shader/BlinnPhong.fs");
	Shader::LoadShader("SkyBox", "../Shader/SkyBox.vs", "../Shader/SkyBox.fs");

	Tank* tank = new Tank("../Resource/Tank2/Tank2.FBX");
	tank->LocalScale = Vector3(0.01,0.01,0.01);
	Node::ROOT->AddChild(tank);

	camera.SetLocalPosition(300,200,0);
	camera.SetLocalEulerAngles(0,90,20);
	tank->Find("Cannon")->AddChild(&camera);

//	Node* model = Model::LoadModel("../Resource/Model/nanosuit.obj");
//	Node* model = Model::LoadModel("../Resource/Tank1/Tank.obj");
//	Node* model = Model::LoadModel("../Resource/M103/M103.obj", false);
//	model->LocalScale = Vector3(0.01, 0.01, 0.01);Node::ROOT->AddChild(model);
//	Node* model = Model::LoadModel("../Resource/Scene/2/Dragon.FBX", false);
	Node* model = Model::LoadModel("../Resource/Scene/3/Scene.FBX", false);
	Node::ROOT->AddChild(model);

	auto skyBox = SkyBox("../Resource/skybox/front.jpg", "../Resource/skybox/back.jpg", "../Resource/skybox/left.jpg", "../Resource/skybox/right.jpg", "../Resource/skybox/top.jpg", "../Resource/skybox/bottom.jpg");
	/*
	Node* cube = new Node();
	cube->mesh = Mesh::Create(Mesh::Cube);
	cube->mesh->material = Material::Create();
	cube->mesh->material->diffuseTexture = Texture2D::Create("../Resource/container2.jpg", false);
	cube->mesh->material->specularTexture = Texture2D::Create("../Resource/container2_specular.jpg", false);
	Node::ROOT->AddChild(cube);
	*/
	unsigned int lastPrint = 0;

	unsigned int FPS = 150;
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
		CameraUpdate();
		

		tank->Aim(Input::GetMousePosDeltaX(), Input::GetMousePosDeltaY());
		Vector3 direction;
		if (Input::GetKey(GLFW_KEY_W))
		{
			direction = direction + Vector3::LEFT;
		}
		if (Input::GetKey(GLFW_KEY_A))
		{
			direction = direction + Vector3::BEHIND;
		}
		if (Input::GetKey(GLFW_KEY_S))
		{
			direction = direction + Vector3::RIGHT;
		}
		if (Input::GetKey(GLFW_KEY_D))
		{
			direction = direction + Vector3::FRONT;
		}
		if (!(direction == Vector3(0, 0, 0)))
		{
			tank->Move(direction.Normalized(), Time::GetDeltaTime());
		}
		
		
		skyBox.Draw();
		Node::ROOT->Rendering();

		Input::ClearFrameInput();

		if (Input::GetKey(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, true);
		}
		//交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
		glfwSwapBuffers(window);
		//是否触发事件，键盘输入、鼠标移动等、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
		glfwPollEvents();
	}
	//释放/删除之前的分配的所有资源
	glfwTerminate();
	return 0;
}
