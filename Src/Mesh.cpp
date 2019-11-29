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