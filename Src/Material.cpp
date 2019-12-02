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
	this->specularTexture = specularTexture;
	this->shininess = shininess;
}

Material::~Material()
{
}