#include "FrogEngine.h"
using namespace FrogEngine;

Node* Model::LoadModel(std::string path)
{
	std::string directory = path.substr(0, path.find_last_of('/')) + "/";
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return nullptr;
	}
	return ProcessNode(directory, scene->mRootNode, scene);
}

Node* Model::ProcessNode(std::string directory, aiNode* node, const aiScene* scene)
{
	Node* ret = new Node();
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ret->mesh = ProcessMesh(directory, mesh, scene);
	}
	// 接下来对它的子节点重复这一过程
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ret->AddChild(ProcessNode(directory, node->mChildren[i], scene));
	}
	return ret;
}

Mesh* Model::ProcessMesh(std::string directory, aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// 处理顶点位置、法线和纹理坐标
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
	// 处理索引
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	Mesh* result = Mesh::Create(vertices, indices);
	// 处理材质
	if (mesh->mMaterialIndex >= 0)
	{	
		Material* material = Material::Create();
		aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];
		material->diffuseTexture = LoadMaterialTextures(directory, ai_material, aiTextureType_DIFFUSE);
		material->specularTexture = LoadMaterialTextures(directory, ai_material, aiTextureType_SPECULAR);
		result->material = material;
	}
	return result;
}

Texture2D* Model::LoadMaterialTextures(std::string directory, aiMaterial* mat, aiTextureType type)
{
	Texture2D* texture = nullptr;
	if (mat->GetTextureCount(type) != 0)
	{
		aiString str;
		mat->GetTexture(type, 0, &str);
		texture = Texture2D::Create((directory + str.C_Str()).c_str(), false);
	}
	return texture;
}