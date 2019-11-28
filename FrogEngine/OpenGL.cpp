#include <iostream>
#include <glad/glad.h> 
#include <glfw3.h>
#include "FrogEngine.h"
using namespace FrogEngine;

float window_width = 800;
float window_height = 600;

float lastX = 400, lastY = 300;
bool firstMouse = true;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

Camera camera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	window_width = width;
	window_height = height;
	std::cout << "Window Size Changed:" << width << "*" << height << std::endl;
	Camera::GetCurrentCamera()->AspectRatio = (window_width / window_height);
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
		return;
	}
	float xoffset = (xpos - lastX) / window_width;
	float yoffset = (lastY - ypos) / window_height;
	camera.ProcessMouseMovement(xoffset, yoffset);

	lastX = xpos;
	lastY = ypos;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.Move(Direction::FRONT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.Move(Direction::BACK, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.Move(Direction::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.Move(Direction::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.Move(Direction::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.Move(Direction::DOWN, deltaTime);
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
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//将已创建窗口的上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(window);
	//GLAD用来管理OpenGL的函数指针，在调用任何OpenGL的函数之前我们需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//OpenGL渲染窗口的尺寸大小，左下角，长，宽
	glViewport(0, 0, window_width, window_height);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//当窗口大小改变时，可能需要一些相应的操作，例如改变视口大小
	//窗口注册一个回调函数，函数原型：void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//鼠标输入回调函数
	glfwSetCursorPosCallback(window, mouse_callback);
	//深度测试
	glEnable(GL_DEPTH_TEST);
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera::SetCurrentCamera(&camera);

	auto directionalLight = DirectionalLight::Create(Vector3(0, 0, -1), Vector3(1, 1, 1));
//	auto pointLight = PointLight::Create(Vector3(0, 0, 3), Vector3(1, 1, 1), 50);
	//	auto flashLight = FlashLight::Create(Vector3(0, 0, 0), Vector3(0, 0, -1), Vector3(1, 1, 1), 15, 20);
	Shader::LoadShader("Phong", "../Shader/Common.vs", "../Shader/BlinnPhong.fs");
	Shader::LoadShader("Specular", "../Shader/Common.vs", "../Shader/Specular.fs");
	Shader::LoadShader("SkyBox", "../Shader/SkyBox.vs", "../Shader/SkyBox.fs");

	Model model("../Resource/Model/nanosuit.obj");
//	Model model("Tank.obj");

	Mesh mesh(Mesh::Geometry::Quad);
	mesh.material = Material::Create();
	mesh.material->diffuseTexture = Texture2D::Create("../Resource/container2.jpg", false);
	mesh.material->specularTexture = Texture2D::Create("../Resource/container2_specular.jpg", false);
	mesh.material->shininess = 1;
	mesh.shader = Shader::GetShader("Phong");


	auto skyBox = SkyBox("../Resource/skybox/front.jpg", "../Resource/skybox/back.jpg", "../Resource/skybox/left.jpg", "../Resource/skybox/right.jpg", "../Resource/skybox/top.jpg", "../Resource/skybox/bottom.jpg");
	//Loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//处理输入
		processInput(window);

		//渲染，屏幕纯色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//清除颜色缓冲和深度缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//		flashLight->position = Camera::GetCurrentCamera()->Position();
		//		flashLight->direction = Camera::GetCurrentCamera()->Forward();

		skyBox.Draw();
		mesh.transform.SetScale(10, 10, 0.2);
		mesh.transform.SetPosition(cos(glfwGetTime()) * 10, 0, -5);
		mesh.Draw();
		model.Rendering();


		//交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
		glfwSwapBuffers(window);
		//是否触发事件，键盘输入、鼠标移动等、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
		glfwPollEvents();
	}
	//释放/删除之前的分配的所有资源
	glfwTerminate();
	return 0;
}
