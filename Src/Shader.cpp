#include <glad/glad.h>
#include "FrogEngine.h"
using namespace FrogEngine;
#include <iostream>
#include <fstream>
#include <sstream>

std::map<const char*, Shader*> Shader::_shaders;

Shader* Shader::LoadShader(const char* name, const char* vertexShaderPath, const char* fragmentShaderPath)
{
	_shaders[name] = new Shader(name, vertexShaderPath, fragmentShaderPath);
	return _shaders[name];
}

Shader* Shader::GetShader(const char* name)
{
	return _shaders[name];
}

Shader::Shader(const char* name, const char* vertexShaderPath, const char* fragmentShaderPath)
{
	name = name;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// 打开文件
		vShaderFile.open(vertexShaderPath);
		fShaderFile.open(fragmentShaderPath);
		std::stringstream vShaderStream, fShaderStream;
		// 读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// 关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		// 转换数据流到string
		std::string vertexCode = vShaderStream.str();
		std::string fragmentCode = fShaderStream.str();

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//顶点着色器对象
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		//把着色器源码附加到着色器对象上，然后编译它
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);
		//检查着色器程序是否编译成功
		int  success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR : VERTEX SHADER ("<< fragmentShaderPath <<") COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		//片段着色器对象
		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		//编译片段着色器对象
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);
		//检查着色器程序是否编译成功
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR : FRAGMENT SHADER (" << vertexShaderPath << ") COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		//创建着色器程序
		_shaderProgram = glCreateProgram();
		//着色器链接到程序上
		glAttachShader(_shaderProgram, vertexShader);
		glAttachShader(_shaderProgram, fragmentShader);
		//链接
		glLinkProgram(_shaderProgram);
		//检查着色器程序状态
		glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR : SHADER (" << name << ") COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		//链接结束后，Shader可以删除了
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR : SHADER FILE(" << vertexShaderPath << "," << fragmentShaderPath << ") NOT_SUCCESFULLY_READ\n" << std::endl;
	}
}

Shader::~Shader()
{

}

void Shader::Use()
{
	glUseProgram(_shaderProgram);
}

void Shader::SetBool(const char* valueName, bool value) const
{
	int valueLocation = glGetUniformLocation(_shaderProgram, valueName);
	glUniform1i(valueLocation, (int)value);
}

void Shader::SetInt(const char* valueName, int value) const
{
	int valueLocation = glGetUniformLocation(_shaderProgram, valueName);
	glUniform1i(valueLocation, value);
}

void Shader::SetFloat(const char* valueName, float value) const
{
	int valueLocation = glGetUniformLocation(_shaderProgram, valueName);
	glUniform1f(valueLocation, value);
}

void Shader::SetMat4(const char* valueName, Matrix4 value) const
{
	unsigned int valueLocation = glGetUniformLocation(_shaderProgram, valueName);
	glUniformMatrix4fv(valueLocation, 1, GL_FALSE, value.ValuePtr());
}

void Shader::SetVector3(const char* valueName, Vector3 value) const
{
	unsigned int valueLocation = glGetUniformLocation(_shaderProgram, valueName);
	glUniform3f(valueLocation, value.GetX(), value.GetY(), value.GetZ());
}