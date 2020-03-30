#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <type_traits>

#include "stb_image.h"

#include <algorithm>

namespace FrogEngine
{
	static const float PI = (float)atan(1) * 4;
	static float Attenuation[] =
	{
		//dis,		constant,	linear,		quadratic
		7.0f,		1.0f,		0.7f,		1.8f,
		13.0f,		1.0f,		0.35f,		0.44f,
		20.0f,		1.0f,		0.22f,		0.20f,
		32.0f,		1.0f,		0.14f,		0.07f,
		50.0f,		1.0f,		0.09f,		0.032f,
		65.0f,		1.0f,		0.07f,		0.017f,
		100.0f,		1.0f,		0.045f,		0.0075f,
		160.0f,		1.0f,		0.027f,		0.0028f,
		200.0f,		1.0f,		0.022f,		0.0019f,
		325.0f,		1.0f,		0.014f,		0.0007f,
		600.0f,		1.0f,		0.007f,		0.0002f,
		3250.0f,	1.0f,		0.0014f,	0.000007f,
	};
	enum Direction
	{
		FRONT,
		BACK,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};
	float NormalizedAngle(float angle);
	float Angle(float radians);
	float Radians(float angle);
	float GetAttenuationLinear(float distant);
	float GetAttenuationQuadratic(float distant);
	struct Vector2
	{
	public:
		Vector2();
		Vector2(float x, float y);
		~Vector2();
		float* ValuePtr();
		Vector2 Normalized();
		bool operator==(const Vector2& other) const;
		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
		Vector2 operator*(float scale) const;
		Vector2 operator/(float scale) const;
		void Print() const { std::cout << "(" << _vec.x << "," << _vec.y << ")" << std::endl; };
		float GetX() const;
		float GetY() const;
		void SetX(float x);
		void SetY(float y);
		glm::vec2 Value();
	private:
		glm::vec2 _vec;
	};
	class Matrix4;
	struct Vector3
	{
	public:
		static Vector3 FRONT;
		static Vector3 BEHIND;
		static Vector3 LEFT;
		static Vector3 RIGHT;
		static Vector3 UP;
		static Vector3 DOWN;
	public:
		Vector3();
		Vector3(glm::vec3 vec);
		Vector3(float x, float y, float z);
		~Vector3();
		float* ValuePtr();
		Vector3 Normalized();
		bool operator==(const Vector3& other) const;
		Vector3 operator+(const Vector3& other) const;
		Vector3 operator-(const Vector3& other) const;
		Vector3 operator*(float scale) const;
		Vector3 operator*(const Matrix4& mat) const;
		Vector3 operator/(float scale) const;
		Vector3 Cross(const Vector3& other) const;
		static float Angle(Vector3 from, Vector3 to);
		void Print() const { std::cout << "(" << _vec.x << "," << _vec.y << "," << _vec.z << ")" << std::endl; };
		float GetX() const;
		float GetY() const;
		float GetZ() const;
		void SetX(float x);
		void SetY(float y);
		void SetZ(float z);
		glm::vec3 Value();
	private:
		glm::vec3 _vec;
	};
	class Matrix4
	{
	public:
		Matrix4();
		Matrix4(glm::mat4 value);
		Matrix4(aiMatrix4x4 mat);
		~Matrix4();
		float* ValuePtr();
		void Translate(const Vector3& offset);
		void Rotate(const Vector3& axis, float angle);
		void Scale(const Vector3& scale);
		void Scale(float scaleX, float scaleY, float scaleZ);
		void LookAt(Vector3 cameraPos, Vector3 targetPos, Vector3 upDirection);
		void Perspective(float portAngle, float scale, float near, float far);
		glm::mat4 Value();
	private:
		glm::mat4 _mat;
		friend Vector3;
	};
	class Mesh;
	class Component;
	class Node
	{
	private:
		void InitByEulerAngles();
	public:
		Node();
		~Node();
		void AddComponent(const char * name,Component* component);

		void SetLocalForward(const Vector3& forward);

		void SetLocalPosition(const Vector3& pos);
		void SetLocalPosition(float x, float y, float z);
		void SetLocalEulerAngles(const Vector3& eulerAngles);
		void SetLocalEulerAngles(float x, float y, float z);
		void SetLocalEulerAngleX(float eularAngleX);
		void SetLocalEulerAngleY(float eularAngleY);
		void SetLocalEulerAngleZ(float eularAngleZ);

		const Vector3& GetLocalEulerAngles() const;
		const Vector3& GetLocalForward() const;
		const Vector3& GetLocalUp() const;
		const Vector3& GetLocalRight() const;

		Node* Find(const char* name) const;

		void Print() const;

		void SetParent(Node* parent);
		Node* GetParent() const;
		void AddChild(Node* child);
		void RemoveChild(Node* child);

		void Rendering();

		Vector3 GetPosition() const;
		Vector3 GetForward() const;
		Vector3 GetUp() const;
		Vector3 GetRight() const;
	public:
		static Node* ROOT;
		Mesh* mesh;
		std::string name;
		Vector3 LocalScale;
		Vector3 LocalPosition;
		bool AutoRendering;
	protected:
		Node* _parent;
		std::set<Node*> _childs;
		Vector3 _eulerAngles;
		Vector3 _up;
		Vector3 _front;
		Vector3 _right;
		static std::map<const char*, Component*> _components;
		
	};
	class Camera : public Node
	{
	public:
		static void SetCurrentCamera(Camera* camera);
		static Camera* GetCurrentCamera();
		Camera();
		~Camera();
		Matrix4 GetProjectionMatrix() const;
		Matrix4 GetLookAtMatrix() const;
	public:
		float AspectRatio;
	private:
		static Camera* _currentCamera;
		
	};
	class Texture2D
	{
	public:
		unsigned int ID;
		bool Alpha;
	public:
		static Texture2D* Create(const char* path, bool alpha);
		~Texture2D();
	private:
		Texture2D(const char* path, bool Alpha);
		static std::map<std::string, Texture2D*> _textures;
	};
	class Material
	{
	public:
		static Material* Create();
		static Material* Create(Texture2D* diffuseTexure, Texture2D* specularTexture, float shininess);
		~Material();
	private:
		Material();
		Material(Texture2D* diffuseTexure, Texture2D* specularTexture, float shininess);
	public:
		Texture2D* diffuseTexture;
		Texture2D* specularTexture;
		Texture2D* normalTexture;
		float shininess;
	};

	struct Vertex
	{
		Vector3 Position;
		Vector3 Normal;
		Vector2 TexCoord;
	};
	class Shader
	{
	private:
		static std::map<const char*, Shader*> _shaders;
		const char* _name;
		unsigned int _shaderProgram;
	private:
		Shader(const char* name, const char* vertexShaderPath, const char* fragmentShaderPath);
	public:
		static Shader* LoadShader(const char* name, const char* vertexShaderPath, const char* fragmentShaderPath);
		static Shader* GetShader(const char* name);
		~Shader();
		void Use();
		void SetBool(const char* valueName, bool value) const;
		void SetFloat(const char* valueName, float value) const;
		void SetInt(const char* valueName, int value) const;
		void SetMat4(const char* valueName, Matrix4 value) const;
		void SetVector3(const char* valueName, Vector3 value) const;
	};
	class Mesh
	{
	public:
		enum Geometry
		{
			Quad,
			Cube
		};
	public:
		static Mesh* Create(Geometry geometry);
		static Mesh* Create(std::vector<Vertex>& vertices);
		static Mesh* Create(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
		~Mesh();
		void Draw();
	private:
		Mesh();
		void TransmitData();
	public:
		Material* material;
		Shader* shader;
	private:
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		unsigned int _vao;
		unsigned int _vbo;
		unsigned int _ebo;
	};
	class Model
	{
	public:
		static Node* LoadModel(std::string path, bool simplified);
	private:
		static Node* ProcessNode(std::string directory, aiNode* node, const aiScene* scene, bool simplified);
		static Mesh* ProcessMesh(std::string directory, aiMesh* mesh, aiNode* node, const aiScene* scene, bool simplified);
		static Texture2D* LoadMaterialTextures(std::string directory, aiMaterial* mat, aiTextureType type);
	};
	class DirectionalLight
	{
		static const int MaxInstance = 1;
	public:
		static DirectionalLight* Create(Vector3 direction, Vector3 color);
		static const std::vector<DirectionalLight*>& GetLights();
		static void Release(DirectionalLight* light);
		~DirectionalLight();
	public:
		Vector3 direction;
		Vector3 color;
	private:
		DirectionalLight();
	private:
		static std::vector<DirectionalLight*> _lights;
	};
	class PointLight
	{
		static const int MaxInstance = 4;
	public:
		static PointLight* Create(Vector3 position, Vector3 color, float range);
		static const std::vector<PointLight*>& GetLights();
		static void Release(PointLight* light);
		~PointLight();
	public:
		Vector3 position;
		Vector3 color;

		float constant;
		float linear;
		float quadratic;
	private:
		PointLight();
	private:
		static std::vector<PointLight*> _lights;
	};
	class FlashLight
	{
		static const int MaxInstance = 4;
	public:
		static FlashLight* Create(Vector3 position, Vector3 direction, Vector3 color, float innerCone, float outerCone);
		static const std::vector<FlashLight*>& GetLights();
		static void Release(FlashLight* light);
		~FlashLight();
	public:
		Vector3 position;
		Vector3 direction;
		Vector3 color;

		float innerCone;
		float outerCone;

		float constant;
		float linear;
		float quadratic;
	private:
		FlashLight();
	private:
		static std::vector<FlashLight*> _lights;
	};
	class SkyBox
	{
	public:
		SkyBox(const char* front, const char* back, const char* left, const char* right, const char* up, const char* down);
		~SkyBox();
		void Draw();
	private:
		static float _skyboxVertices[];
		unsigned int _vbo;
		unsigned int _vao;
		unsigned int _textureID;
	};
	class Screen
	{
	public:
		static void ScreenSizeChanged(GLFWwindow* window, int width, int height);
		static int GetWidth();
		static int GetHeight();
	private:
		static int Width;
		static int Height;
	};
	class Time
	{
	public:
		static float GetCurrentTime();
		static float GetDeltaTime();
		static void Update();
	private:
		static float CurrentTime;
		static float DeltaTime;
	};
	class Input
	{
	public:
		static void SetWindow(GLFWwindow* window);
		static void UpdateMousePos(GLFWwindow* window, double xpos, double ypos);
		static void ClearFrameInput();
		static double GetMousePosX();
		static double GetMousePosY();
		static double GetMousePosDeltaX();
		static double GetMousePosDeltaY();
		static bool GetKey(int key);
	private:
		static GLFWwindow* _window;
		static double _mousePosX;
		static double _mousePosY;
		static double _mousePosDeltaX;
		static double _mousePosDeltaY;
	};
	class Component
	{
	public:
		Component();
		~Component();
		virtual void Awake() = 0;
		virtual void Update() = 0;
		static void UpdateAllComponents();
		void AddToUpdatePool();
		void RemoveFromUpdatePool();
	public:
		Node* _node;
	private:
		static std::set<Component*> components;

	};
}
