#include "FrogEngine.h"
using namespace FrogEngine;

Material* Material::Create()
{
	return new Material();
}

Material* Material::Create(Texture2D* diffuseTexure, Texture2D* specularTexture, float shininess)
{
	return new Material(diffuseTexure, specularTexture, shininess);
}

Material::Material()
{
	this->diffuseTexture = nullptr;
	this->specularTexture = nullptr;
	this->shininess = 32;
}

Material::Material(Texture2D* diffuseTexure, Texture2D* specularTexture, float shininess)
{
	this->diffuseTexture = diffuseTexure;
	diffuseTexure->Retain();
	this->specularTexture = specularTexture;
	specularTexture->Retain();
	this->shininess = shininess;
}

void Material::SetDiffuseTexture(Texture2D* texture)
{
	if (!texture)
	{
		return;
	}
	texture->Retain();
	diffuseTexture = texture;
}

Texture2D* Material::GetDiffuseTexture()
{
	return diffuseTexture;
}

void Material::SetSpecularTexture(Texture2D* texture)
{
	if (!texture)
	{
		return;
	}
	texture->Retain();
	specularTexture = texture;
}

Texture2D* Material::GetSpecularTexture()
{
	return specularTexture;
}

void Material::SetNormalTexture(Texture2D* texture)
{
	if (!texture)
	{
		return;
	}
	texture->Retain();
	normalTexture = texture;
}

Texture2D* Material::GetNormalTexture()
{
	return normalTexture;
}

Material::~Material()
{
	if (diffuseTexture)
	{
		diffuseTexture->Release();
	}
	if (specularTexture)
	{
		specularTexture->Release();
	}
	if (normalTexture)
	{
		normalTexture->Release();
	}
}