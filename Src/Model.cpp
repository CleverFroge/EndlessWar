#include <stack>
#include "FrogEngine.h"
using namespace FrogEngine;

Node* Model::LoadModel(std::string path, bool simplified)
{
	std::string directory = path.substr(0, path.find_last_of('/')) + "/";
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return nullptr;
	}
	return ProcessNode(directory, scene->mRootNode, scene, nullptr, simplified);
}

Node* Model::ProcessNode(std::string directory, aiNode* node, const aiScene* scene, Node* parent, bool simplified)
{
	Node* ret = new Node();
	ret->name = node->mName.C_Str();
	if (simplified)
	{
		aiNode* it = node->mParent;
		while (it)
		{
			if (it->mName == node->mName)
			{
				break;
			}
			it = it->mParent;
		}
		aiVector3D ori(0, 0, 0);
		std::stack<aiMatrix4x4> mats;
		std::string parentName;
		bool parent = false;
		while (it)
		{
			std::string nodeName = it->mName.C_Str();
			if (parent && nodeName == parentName)
			{
				break;
			}
			if (it->mMeshes)
			{
				parent = true;
				parentName = nodeName;
			}
			mats.push(it->mTransformation);
			it = it->mParent;
		}
		while (!mats.empty())
		{
			ori *= mats.top();
			mats.pop();
		}
		if (node->mNumMeshes)
		{
			ret->SetLocalPosition(ori.x, ori.z, -ori.y);
		}
	}
	ret->parentTransform = node->mTransformation;
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ret->mesh = ProcessMesh(directory, mesh, node, scene, simplified);
	}
	// 接下来对它的子节点重复这一过程
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		if (node->mNumMeshes)
		{
			ret->AddChild(ProcessNode(directory, node->mChildren[i], scene, ret, simplified));
		}
		else
		{
			ret->AddChild(ProcessNode(directory, node->mChildren[i], scene, parent, simplified));
		}
	}
	return ret;
}

Mesh* Model::ProcessMesh(std::string directory, aiMesh* mesh, aiNode* node, const aiScene* scene, bool simplified)
{
	std::vector<Vertex> vertices;
	aiMatrix4x4 model;
	aiNode* it = node->mParent;
	while (it)
	{
		if (it->mName == node->mName)
		{
			break;
		}
		model *= it->mTransformation;
		it = it->mParent;
	}
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// 处理顶点位置、法线和纹理坐标
		aiVector3D pos = mesh->mVertices[i];
		if (simplified)
		{
			pos *= model;
		}
		vertex.Position = Vector3(pos.x, pos.z, -pos.y);
		aiVector3D normal = mesh->mNormals[i];
		normal *= node->mTransformation;
		vertex.Normal = Vector3(normal.x, normal.y, normal.z);
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
	std::vector<unsigned int> indices;
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
		material->normalTexture = LoadMaterialTextures(directory, ai_material, aiTextureType_NORMALS);
		material->displacementTexture = LoadMaterialTextures(directory, ai_material, aiTextureType_DISPLACEMENT);

		aiGetMaterialFloat(ai_material, AI_MATKEY_SHININESS, &material->shininess);
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