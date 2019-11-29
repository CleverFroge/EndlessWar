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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"

namespace FrogEngine
{
	static const float PI = atan(1) * 4;
	static float Attenuation[] =
	{
		//dis, constant, linear, quadratic
		7,	1.0,	0.7,	1.8,		
		13,	1.0,	0.35,	0.44,	
		20,	1.0,	0.22,	0.20,	
		32,	1.0,	0.14,	0.07,	
		50,	1.0,	0.09,	0.032,	
		65,	1.0,	0.07,	0.017,	
		100,	1.0,	0.045,	0.0075,	
		160,	1.0,	0.027,	0.0028,	
		200,	1.0,	0.022,	0.0019,	
		325,	1.0,	0.014,	0.0007,	
		600,	1.0,	0.007,	0.0002,	
		3250,	1.0,	0.0014,	0.000007,	
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
	class Transform
	{
	private:
		void InitByEulerAngles();
	public:
		Transform();
		~Transform();
		void SetScale(float x, float y, float z);
		void SetPosition(const Vector3& position);
		void SetPosition(float x, float y, float z);
		void SetEulerAngle(const Vector3& eulerAngles);
		void SetEulerAngle(float x, float y, float z);
		void SetEulerAngleX(float eularAngleX);
		void SetEulerAngleY(float eularAngleY);
		void SetEulerAngleZ(float eularAngleZ);

		const Vector3& Scale() const;
		const Vector3& Position() const;
		const Vector3& EulerAngle() const;
		const Vector3& Forward() const;
		const Vector3& Up() const;
		const Vector3& Right() const;

		void Print() const;

		void SetParent(Transform* parent);
		void AddChild(Transform* child);
		void RemoveChild(Transform* child);
	public:
		static Transform* root;
	private:
		Transform* _parent;
		std::set<Transform*> _childs;
		Vector3 _scale;
		Vector3 _position;

		Vector3 _eulerAngle;
		Vector3 _up;
		Vector3 _front;
		Vector3 _right;
	};
	class Camera : public Transform
	{
	public:
		static void SetCurrentCamera(Camera* camera);
		static Camera* GetCurrentCamera();
		Camera();
		~Camera();
		void ProcessMouseMovement(float deltaX, float deltaY);
		void Move(Direction direction, float deltaTime);
		void ProcessMouseScroll(float scroll);
		Matrix4 GetProjectionMatrix() const;
		Matrix4 GetLookAtMatrix() const;
	public:
		float AspectRatio;
	private:
		static Camera* _currentCamera;
		float _movementSpeed;
		float _mouseSensitivity;
		float _zoom;
	};
	class Texture2D
	{
	public:
		unsigned int ID;
		bool Alpha;
	public:
		static Texture2D* Create();
		static Texture2D* Create(const char* path, bool alpha);
		~Texture2D();
	private:
		Texture2D();
		Texture2D(const char* path, bool Alpha);
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
		void Draw(Shader* shader);
		void SimpleDraw(Shader* shader);
	private:
		Mesh();
		void TransmitData();
	public:
		Transform transform;
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
		Model(std::string path);
		Model(Mesh* mesh);
		~Model();
		Transform& GetTransform();
		void Rendering();
	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
		Texture2D* LoadMaterialTextures(aiMaterial* mat, aiTextureType type);
	private:
		std::string _directory;
		Transform _transform;
		std::vector<Mesh*> _meshs;
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
}
