#include "FrogEngine.h"
using namespace FrogEngine;

Model::Model(std::string path)
{
	_directory = path.substr(0, path.find_last_of('/')) + "/";
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	ProcessNode(scene->mRootNode, scene);
}

Model::Model(Mesh* mesh)
{
	_meshs.push_back(mesh);
}

Model::~Model()
{
}

void Model::Rendering()
{
	for (size_t i = 0; i < _meshs.size(); i++)
	{
		_meshs[i]->Draw();
	}
}

Transform& Model::GetTransform()
{
	return _transform;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// ����ڵ����е���������еĻ���
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshs.push_back(ProcessMesh(mesh, scene));
	}
	// �������������ӽڵ��ظ���һ����
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// ������λ�á����ߺ���������
		vertex.Position = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoord = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertex.TexCoord = Vector2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	// ��������
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	Mesh* result = Mesh::Create(vertices, indices);
	// �������
	if (mesh->mMaterialIndex >= 0)
	{	
		Material* material = Material::Create();
		aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];
		material->diffuseTexture = LoadMaterialTextures(ai_material, aiTextureType_DIFFUSE);
		material->specularTexture = LoadMaterialTextures(ai_material, aiTextureType_SPECULAR);
		result->material = material;
	}
	return result;
}

Texture2D* Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
	Texture2D* texture = nullptr;
	if (mat->GetTextureCount(type) != 0)
	{
		aiString str;
		mat->GetTexture(type, 0, &str);
		texture = Texture2D::Create((_directory + str.C_Str()).c_str(), false);
	}
	return texture;
}