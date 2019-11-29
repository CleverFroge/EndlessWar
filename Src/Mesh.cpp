#include <glad/glad.h>
#include "FrogEngine.h"
using namespace FrogEngine;

Mesh::Mesh()
{
}

Mesh* Mesh::Create(Geometry geometry)
{
	Mesh* mesh = new Mesh();
	switch (geometry)
	{
	case FrogEngine::Mesh::Quad:
	{
		float vertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f
		};
		for (size_t i = 0; i < 6; i++)
		{
			Vertex vertex;
			vertex.Position = Vector3(vertices[i * 8], vertices[i * 8 + 1], vertices[i * 8 + 2]);
			vertex.Normal = Vector3(vertices[i * 8 + 3], vertices[i * 8 + 4], vertices[i * 8 + 5]);
			vertex.TexCoord = Vector2(vertices[i * 8 + 6], vertices[i * 8 + 7]);
			mesh->_vertices.push_back(vertex);
		}
	}
	break;
	case FrogEngine::Mesh::Cube:
	{
		float vertices[] = {
			// positions          // normals           // texture coords
			// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
		};
		for (size_t i = 0; i < 36; i++)
		{
			Vertex vertex;
			vertex.Position = Vector3(vertices[i * 8], vertices[i * 8 + 1], vertices[i * 8 + 2]);
			vertex.Normal = Vector3(vertices[i * 8 + 3], vertices[i * 8 + 4], vertices[i * 8 + 5]);
			vertex.TexCoord = Vector2(vertices[i * 8 + 6], vertices[i * 8 + 7]);
			mesh->_vertices.push_back(vertex);
		}
	}
	break;
	default:
		break;
	}
	mesh->TransmitData();
	return mesh;
}

Mesh* Mesh::Create(std::vector<Vertex>& vertices)
{
	Mesh* mesh = new Mesh();
	mesh->_vertices = vertices;
	mesh->TransmitData();
	return mesh;
}

Mesh* Mesh::Create(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	Mesh* mesh = new Mesh();
	mesh->_vertices = vertices;
	mesh->_indices = indices;
	mesh->TransmitData();
	return mesh;
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &_vao);
	if (_indices.size()!=0)
	{
		glDeleteBuffers(1, &_ebo);
	}
	glDeleteBuffers(1, &_vbo);
}

void Mesh::Draw()
{
	if (!shader)
	{
		shader = Shader::GetShader("Phong");
	}
	//使用该着色器程序
	shader->Use();
	//向shader发送材质数据
	if (material)
	{
		unsigned int textureIndex = 0;
		if (material->diffuseTexture)
		{
			shader->SetBool("material.alpha", material->diffuseTexture->Alpha);
			shader->SetInt("material.diffuse", textureIndex);
			glActiveTexture(GL_TEXTURE0 + textureIndex);
			glBindTexture(GL_TEXTURE_2D, material->diffuseTexture->ID);
			textureIndex++;
		}
		else
		{
			shader->SetInt("material.diffuse", -1);
		}
		if (material->specularTexture)
		{
			shader->SetInt("material.specular", textureIndex);
			glActiveTexture(GL_TEXTURE0 + textureIndex);
			glBindTexture(GL_TEXTURE_2D, material->specularTexture->ID);
		}
		else
		{
			shader->SetInt("material.specular", -1);
		}
		shader->SetFloat("material.shininess", material->shininess);
	}
	//向shader发送model矩阵
	Matrix4 model;
	model.Translate(transform.Position());
	Vector3 eulerAngles = transform.EulerAngle();
	model.Rotate(Vector3(1, 0, 0), -eulerAngles.GetX());
	model.Rotate(Vector3(0, 1, 0), eulerAngles.GetY());
	model.Rotate(Vector3(0, 0, 1), eulerAngles.GetZ());
	model.Scale(transform.Scale());
	shader->SetMat4("model", model);

	//向shader发送相机相关数据
	Camera* currentCamera = Camera::GetCurrentCamera();
	shader->SetVector3("viewPos", currentCamera->Position());
	shader->SetMat4("view", currentCamera->GetLookAtMatrix());
	shader->SetMat4("projection", currentCamera->GetProjectionMatrix());

	//向shader发送光源相关数据
	//方向光
	auto directionalLights = DirectionalLight::GetLights();
	shader->SetInt("directionalLightNum", directionalLights.size());
	for (size_t i = 0; i < directionalLights.size(); i++)
	{
		std::string name = std::string("directionalLights[") + std::to_string(i) + std::string("]");
		shader->SetVector3((name + std::string(".direction")).c_str(), directionalLights[i]->direction);
		shader->SetVector3((name + std::string(".color")).c_str(), directionalLights[i]->color);
	}
	//点光源
	auto pointLights = PointLight::GetLights();
	shader->SetInt("pointLightNum", pointLights.size());
	for (size_t i = 0; i < pointLights.size(); i++)
	{
		std::string name = std::string("pointLights[") + std::to_string(i) + std::string("]");
		shader->SetVector3((name + std::string(".position")).c_str(), pointLights[i]->position);
		shader->SetVector3((name + std::string(".color")).c_str(), pointLights[i]->color);
		shader->SetFloat((name + std::string(".constant")).c_str(), pointLights[i]->constant);
		shader->SetFloat((name + std::string(".linear")).c_str(), pointLights[i]->linear);
		shader->SetFloat((name + std::string(".quadratic")).c_str(), pointLights[i]->quadratic);
	}
	//聚光
	auto flashLights = FlashLight::GetLights();
	shader->SetInt("flashLightNum", flashLights.size());
	for (size_t i = 0; i < flashLights.size(); i++)
	{
		std::string name = std::string("flashLights[") + std::to_string(i) + std::string("]");
		shader->SetVector3((name + std::string(".position")).c_str(), flashLights[i]->position);
		shader->SetVector3((name + std::string(".direction")).c_str(), flashLights[i]->direction);
		shader->SetVector3((name + std::string(".color")).c_str(), flashLights[i]->color);
		shader->SetFloat((name + std::string(".innerCone")).c_str(), flashLights[i]->innerCone);
		shader->SetFloat((name + std::string(".outerCone")).c_str(), flashLights[i]->outerCone);
		shader->SetFloat((name + std::string(".constant")).c_str(), flashLights[i]->constant);
		shader->SetFloat((name + std::string(".linear")).c_str(), flashLights[i]->linear);
		shader->SetFloat((name + std::string(".quadratic")).c_str(), flashLights[i]->quadratic);
	}

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	if (_indices.size() == 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	}
}

void Mesh::Draw(Shader* shader)
{
	//使用该着色器程序
	shader->Use();
	//向shader发送model矩阵
	Matrix4 model;
	model.Translate(transform.Position());
	Vector3 eulerAngles = transform.EulerAngle();
	model.Rotate(Vector3(1, 0, 0), -eulerAngles.GetX());
	model.Rotate(Vector3(0, 1, 0), eulerAngles.GetY());
	model.Rotate(Vector3(0, 0, 1), eulerAngles.GetZ());
	model.Scale(transform.Scale());
	shader->SetMat4("model", model);	
	
	//绘制
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	if (_indices.size() == 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	}
}

void Mesh::SimpleDraw(Shader* shader)
{
	//使用该着色器程序
	shader->Use();
	//向shader发送model矩阵
	Matrix4 model;
	model.Translate(transform.Position());
	Vector3 eulerAngles = transform.EulerAngle();
	model.Rotate(Vector3(1, 0, 0), -eulerAngles.GetX());
	model.Rotate(Vector3(0, 1, 0), eulerAngles.GetY());
	model.Rotate(Vector3(0, 0, 1), eulerAngles.GetZ());
	model.Scale(transform.Scale());
	shader->SetMat4("model", model);

	//绘制
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	if (_indices.size() == 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	}
}

void Mesh::TransmitData()
{
	//顶点缓冲对象
	glGenBuffers(1, &_vbo);
	//创建顶点数组对象
	glGenVertexArrays(1, &_vao);

	//绑定顶点数组对象
	glBindVertexArray(_vao);
	//OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。我们可以使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

	if (_indices.size() != 0)
	{
		//创建顶点索引对象
		glGenBuffers(1, &_ebo);
		//绑定顶点索引对象，传输数据
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), &_indices[0], GL_STATIC_DRAW);
	}

	//如何解析顶点数据
	//glVertexAttribPointer参数：位置值，顶点属性大小，数据类型，标准化（0~1），步长，起始位置偏移量
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TexCoord)));
	glEnableVertexAttribArray(2);
}