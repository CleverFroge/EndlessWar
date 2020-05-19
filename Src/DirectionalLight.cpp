#include "FrogEngine.h"
using namespace FrogEngine;


DirectionalLight* DirectionalLight::Create(Vector3 direction, Vector3 color)
{
	DirectionalLight* light = new DirectionalLight();
	light->direction = direction.Normalized();
	light->color = color.Normalized();
	return light;
}

DirectionalLight::DirectionalLight()
{
	center = nullptr;

	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::GenerateDepthMap()
{
	// 1. 首选渲染深度贴图
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	float size = 20;
	GLfloat near_plane = 0.0, far_plane = size * 2;
	glm::mat4 lightProjection = glm::ortho(-size / 2, size / 2, -size / 2, size / 2, near_plane, far_plane);

	Vector3 centerPos = Vector3(0,0,0);
	if (center)
	{
		try
		{
			centerPos = center->GetPosition();
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	Vector3 eyePos = centerPos - direction * size / 2;
	glm::mat4 lightView = glm::lookAt(eyePos.Value(), centerPos.Value(), glm::vec3(0,1,0));
	lightSpaceMatrix = lightProjection * lightView;
	Scene::GetCurrentScene()->_root->DepthRendering(lightSpaceMatrix);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}