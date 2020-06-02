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

#include "FBXSDK/fbxsdk.h"

#include "stb_image.h"
#include "stb_image_write.h"

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
	float Clamp(float value, float min, float max);
	class Random
	{
	public:
		static float Randf(float min, float max);
	private:
		Random() {};
		~Random() {};
	};
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
		Vector3(FbxVector4 vec);
		~Vector3();
		float* ValuePtr();
		Vector3 Normalized() const;
		bool operator==(const Vector3& other) const;
		Vector3 operator+(const Vector3& other) const;
		Vector3 operator-(const Vector3& other) const;
		Vector3 operator*(float scale) const;
		Vector3 operator*(const Matrix4& mat) const;
		Vector3 operator/(float scale) const;
		Vector3 Cross(const Vector3& other) const;
		static float Dot(const Vector3& v1, const Vector3& v2);
		float Length() const;
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
		Matrix4 Inverse();
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
	class Ref
	{
	public:
		void Release();
		void Retain();
		int GetReferenceCount() const;
	protected:
		Ref();
		virtual ~Ref();
		friend class ReleasePool;
	private:
		int _referenceCount;
	};
	class ReleasePool
	{
	public:
		static ReleasePool* GetInstance();
		void AddObject(Ref* object);
		void RemoveObject(Ref* object);
		void Clear();
	private:
		ReleasePool();
		~ReleasePool();

		static ReleasePool* _instance;
		std::set<Ref*> _objects;
	};
	class Mesh;
	class Component;
	class SphereCollider;
	class Node :public Ref
	{
	private:
		void InitByEulerAngles();
	public:
		Node();
		~Node();
		Node* Clone() const;

		void AddComponent(Component* component);
		Component* GetComponent(const char* name);

		virtual void OnCollision(SphereCollider* collider);
		virtual void OnEnter(SphereCollider* collider);
		virtual void OnExit(SphereCollider* collider);
		void SetCollider(SphereCollider* collider);

		void SetLocalForward(const Vector3& forward);
		void SetForward(const Vector3& forward);

		void SetScale(Vector3 scale);

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

		int GetChildNum();
		void SetParent(Node* parent);
		Node* GetParent() const;
		void AddChild(Node* child);
		void RemoveChild(Node* child);
		void RemoveFromParent();
		bool IsDescendantOf(Node* node);

		void Rendering();
		void DepthRendering(Matrix4 lightSpaceMatrix);

		void ComponentsUpdate();
		void ComponentsLateUpdate();

		Vector3 GetPosition() const;
		Vector3 GetForward() const;
		Vector3 GetUp() const;
		Vector3 GetRight() const;

		void AddMesh(Mesh* mesh);
		int GetMeshNum();
		Mesh* GetMesh(int index);

		Matrix4 GetModelMatrix();

	public:
		std::string name;
		Vector3 LocalScale;
		Vector3 LocalPosition;
		bool AutoRendering;
		bool Shadow;
		Matrix4 geomerty;
	protected:
		std::vector<Mesh*> meshs;
		Node* _parent;
		std::set<Node*> _childs;
		Vector3 _eulerAngles;
		Vector3 _up;
		Vector3 _front;
		Vector3 _right;
		std::map<const char*, Component*> _components;
		SphereCollider* _collider;
	};
	class SkyBox;
	class Camera : public Node
	{
	public:
		Camera();
		~Camera();
		Matrix4 GetProjectionMatrix() const;
		Matrix4 GetLookAtMatrix() const;
		void SetSkyBox(SkyBox* skyBox);
		SkyBox* GetSkyBox() const;
	public:
		float AspectRatio;
	private:
		SkyBox* _skyBox;
	};
	class Texture2D :public Ref
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
		void SetDiffuseTexture(Texture2D* texture);
		Texture2D* GetDiffuseTexture();
		void SetSpecularTexture(Texture2D* texture);
		Texture2D* GetSpecularTexture();
		void SetNormalTexture(Texture2D* texture);
		Texture2D* GetNormalTexture();
	private:
		Material();
		Material(Texture2D* diffuseTexure, Texture2D* specularTexture, float shininess);
		Texture2D* diffuseTexture;
		Texture2D* specularTexture;
		Texture2D* normalTexture;
	public:
		float shininess;
	};

	struct Vertex
	{
		Vector3 Position;
		Vector3 Normal;
		Vector3 Tangent;
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
		void Use();
		void SetBool(const char* valueName, bool value) const;
		void SetFloat(const char* valueName, float value) const;
		void SetInt(const char* valueName, int value) const;
		void SetMat4(const char* valueName, Matrix4 value) const;
		void SetVector3(const char* valueName, Vector3 value) const;

		static Shader* LoadShader(const char* name, const char* vertexShaderPath, const char* fragmentShaderPath);
		static Shader* GetShader(const char* name);
		~Shader();
		virtual void RenderingNodes(Mesh* mesh, std::vector<Node*> nodes) {};
	};
	class Mesh :public Ref
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
		friend class ParticleEmitter; std::vector<Vertex> _vertices;
	private:

		std::vector<unsigned int> _indices;
		unsigned int _vao;
		unsigned int _vbo;
		unsigned int _ebo;
		static std::map<const char*, Mesh*> _geometryMeshes;
	};
	class Model
	{
	private:
		static FbxManager* pManager;
	public:
		static Node* LoadModel(std::string path);
	private:
		static Node* ProcessNode(std::string directory, FbxNode* pNode);
		static Mesh* ProcessMesh(std::string directory, FbxMesh* pMesh);
		static Vertex GetVertex(FbxMesh* pMesh, int polygonIndex, int polygonVertexIndex);
		static Texture2D* LoadMaterialTexture(std::string directory, FbxSurfaceMaterial* pMaterial, const char* type);
	};
	class DirectionalLight
	{
	public:
		static DirectionalLight* Create(Vector3 direction, Vector3 color);
		~DirectionalLight();
		void GenerateDepthMap();
	public:
		Vector3 direction;
		Vector3 color;

		GLuint depthMapFBO;

		const GLuint SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
		GLuint depthMap;
		Matrix4 lightSpaceMatrix;
		Node* center;
	private:
		DirectionalLight();
	};
	class PointLight
	{
	public:
		static PointLight* Create(Vector3 position, Vector3 color, float range);
		~PointLight();
	public:
		Vector3 position;
		Vector3 color;

		float constant;
		float linear;
		float quadratic;
	private:
		PointLight();
	};
	class FlashLight
	{
	public:
		static FlashLight* Create(Vector3 position, Vector3 direction, Vector3 color, float innerCone, float outerCone);
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
		static float GetTime();
		static float GetDeltaTime();
		static void Update();
	private:
		static float CurrentTime;
		static float DeltaTime;
	};
	class Input
	{
	public:
		static void Update();
		static void SetWindow(GLFWwindow* window);
		static void UpdateMousePos(GLFWwindow* window, double xpos, double ypos);
		static void MouseClickCallBack(GLFWwindow* window, int button, int action, int mods);
		static void ClearFrameInput();
		static double GetMousePosX();
		static double GetMousePosY();
		static double GetMousePosDeltaX();
		static double GetMousePosDeltaY();
		static Vector2 GetAxis();
		static bool GetKey(int key);
	private:
		static GLFWwindow* _window;
		static double _mousePosX;
		static double _mousePosY;
		static double _mousePosDeltaX;
		static double _mousePosDeltaY;

		static bool _mouseLeftClick;
		static bool _mouseMiddleClick;
		static bool _mouseRightClick;

		static Vector2 _axis;
	};
	class Component
	{
	public:
		Component(const char* name);
		virtual ~Component();
		virtual void Awake() = 0;
		virtual void Update() = 0;
		virtual void LateUpdate() = 0;
	public:
		Node* _node;
		const char* _name;
	};
	class Scene
	{
	public:
		static void SetCurrentScene(Scene* scene);
		static Scene* GetCurrentScene();
		Scene();
		~Scene();
		void Run();
		void RenderingScene();
		void SetCurrentCamera(Camera* camera);
		Camera* GetCurrentCamera();
		void AddDirectionalLight(DirectionalLight* light);
		std::vector<DirectionalLight*>& GetDirectionalLights();
		void AddPointLight(PointLight* light);
		std::vector<PointLight*>& GetPointLights();
		void AddFlashLight(FlashLight* light);
		std::vector<FlashLight*>& GetFlashLights();
		void AddObject(Node* node);
		Node* _root;
	protected:

		Camera* _currentCamera;
		std::vector<DirectionalLight*> _directionalLights;
		std::vector<PointLight*> _pointLights;
		std::vector<FlashLight*> _flashLights;
	private:
		static Scene* CurrentScene;
	};
	class Particle :public Node
	{
	public:
		Particle();
		~Particle();
	public:
		float BornTime;
		float LifeTime;
		Vector3 EmitDirection;

		bool ConstantSpeed;
		float StartSpeed;
		float EndSpeed;

		bool ConstanColor;
		Vector3 StartColor;
		Vector3 EndColor;

		float Size;
	};
	class ParticleEmitter :public Component
	{
	public:
		static Node* Create();
		ParticleEmitter();
		~ParticleEmitter();
		void Awake();
		void Update();
		void LateUpdate();
		void DrawAllParticles();

		void SetParticleLife(float life);
		void SetParticleLife(float minLife, float maxLife);

		void SetParticleColor(Vector3 color);
		void SetParticleColor(Vector3 startColor, Vector3 endColor);

		void SetParticleSpeed(float speed);
		void SetParticleSpeed(float startSpeed, float endSpeed);

		void SetParticleSize(float size);
		void SetParticleSize(float minSize, float maxSize);

		void Start();
		void Stop();
		void ReStart();
	private:
		struct ParticleVertex
		{
			Vector3 Position;
			Vector3 Color;
		};
		unsigned int vao, vbo;
		std::vector<ParticleVertex> vertices;
	public:
		float EmitAngle;
		int MaxParticles;
		int EmitSpeed;
		bool Loop;
		bool ReleaseAfterEmit;
		static std::set<ParticleEmitter*> ParticleEmitters;
	private:
		std::vector<Particle*> _particles;
		float _startTime;
		float _emittedParticleNum;
		float _ExistingParticleNum;

		bool _start;

		bool _constantLife;
		float _minParticleLife;
		float _maxParticleLife;

		bool _constantColor;
		Vector3 _startParticleColor;
		Vector3 _endParticleColor;

		bool _constantSpeed;
		float _startParticleSpeed;
		float _endParticleSpeed;

		bool _constantSize;
		float _minParticleSize;
		float _maxParticleSize;
	};
	class SphereCollider
	{
		friend Node;
	public:
		enum Layer
		{
			Player,
			Enemy,
			PlayerMissile,
			EnemyMissile,
			PlayerExplosion,
			EnemyExplosion
		};
	public:
		SphereCollider(Layer layer, float size);
		static void Detected();
		static void SetLayerCollsion(Layer layer1, Layer layer2, bool collision);
		bool CanMove(Vector3 direction) const;
		Layer GetLayer() const;
		Node* GetNode() const;
	private:
		virtual ~SphereCollider();
	private:
		static std::set<SphereCollider*> _colliders;
		static std::map<Layer, std::set<Layer>> _layerCollision;
		Layer _layer;

		Node* _node;
		float _size;
		std::set<SphereCollider*> _collisionColliders;
	};
}
