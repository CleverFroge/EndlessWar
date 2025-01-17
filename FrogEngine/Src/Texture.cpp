#pragma once
#include <glad/glad.h>
#include "FrogEngine.h"
using namespace FrogEngine;
#include <iostream>

std::map<std::string, Texture2D*> Texture2D::_textures;


Texture2D* Texture2D::Create(const char* path, bool alpha)
{
	if (_textures.find(path) != _textures.end())
	{
		return _textures[path];
	}
	Texture2D* ret = new Texture2D(path, alpha);
	_textures[path] = ret;
	return ret;
}

Texture2D::Texture2D(const char* path, bool alpha)
{
	Alpha = alpha;
	glGenTextures(1, &ID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &ID);
}