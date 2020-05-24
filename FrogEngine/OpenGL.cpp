#include <iostream>
#include "FrogEngine.h"
//using namespace FrogEngine;
#include "EndlessWar.h"
#include "windows.h"

//#undef GetCurrentTime()
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
	glfwSetMouseButtonCallback(window, Input::MouseClickCallBack);
	//深度测试
	glEnable(GL_DEPTH_TEST);
	//Blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//面剔除
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	Scene* endlessWar = new EndlessWar();
	Scene::SetCurrentScene(endlessWar);

	unsigned int lastPrint = 0;
	unsigned int FPS = 12000;
	float FrameTime = 1 / (float)FPS;
	float FrameStartTime = glfwGetTime();

	//Loop
	while (!glfwWindowShouldClose(window))
	{
		float restTime = (FrameStartTime - glfwGetTime()) * 1000;
		if (restTime>0)
		{
			Sleep(restTime);
		}
		
		FrameStartTime = glfwGetTime() + FrameTime;
		Time::Update();
		Input::Update();
		//清除颜色缓冲和深度缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		

		
		Scene::GetCurrentScene()->Run();

		ReleasePool::GetInstance()->Clear();

		if (Input::GetKey(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, true);
		}
		Input::ClearFrameInput();
		SphereCollider::Detected();
		//交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
		glfwSwapBuffers(window);
		//是否触发事件，键盘输入、鼠标移动等、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
		glfwPollEvents();

		if (FrogEngine::Time::GetTime() - lastPrint >= 1)
		{
			lastPrint = FrogEngine::Time::GetTime();
			glfwSetWindowTitle(window, (std::string("Endless War    Fps: ") + std::to_string(1/Time::GetDeltaTime())).c_str());
		}
	}
	//释放/删除之前的分配的所有资源
	glfwTerminate();
	return 0;
}
