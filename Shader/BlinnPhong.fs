#version 330 core
out vec4 FragColor;

float gamma = 2.2;

in vec3 Normal;  
in vec3 FragPos;  
in mat3 TBN;
in vec2 TexCoord;

#define NR_DIRECTIONAL_LIGHTS 1
#define NR_POINT_LIGHTS 4
#define NR_FLASH_LIGHTS 4

float ambientStrength = 0.01;
float diffuseStrength = 0.2;
float specularStrength = 0.3;

struct Material {
	bool haveDiffuse;
    sampler2D diffuse;
    bool alpha;
	bool haveSpecular;
    sampler2D specular;
    float shininess;
	bool haveNormal;
    sampler2D normal;
}; 

struct DirectionalLight {
    vec3 direction;
	vec3 color;
	mat4 lighgtSpaceMat;
	sampler2D depthMap;
};

struct PointLight {
    vec3 position;
	vec3 color;
	float constant;
    float linear;
    float quadratic;
};

struct Flashlight {
	vec3 position;
	vec3 direction;
	vec3 color;
	float innerCone;
	float outerCone;

	float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;
uniform Material material;

uniform int directionalLightNum;
uniform DirectionalLight directionalLights[NR_DIRECTIONAL_LIGHTS];

uniform int pointLightNum;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform int flashLightNum;
uniform Flashlight flashLights[NR_FLASH_LIGHTS];

vec3 CalcDirectionalLight(DirectionalLight directionalLight, vec4 diffuseTex, vec4 specularTex, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight pointLight, vec4 diffuseTex, vec4 specularTex, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcFlashLight(Flashlight flashLight, vec4 diffuseTex, vec4 specularTex, vec3 normal, vec3 fragPos, vec3 viewDir);

bool blinn = true;

void main()
{
	vec4 diffuseTex;
	if (material.haveDiffuse)
	{
		diffuseTex = vec4(pow(texture(material.diffuse, TexCoord).rgb, vec3(gamma)), 1);
	}
    
	vec4 specularTex;
	if (material.haveSpecular)
	{
		specularTex = vec4(pow(texture(material.specular, TexCoord).rgb, vec3(gamma)), 1);
	}

    vec3 viewDir = normalize(FragPos-viewPos);
	vec3 result;
	for(int i = 0; i<directionalLightNum; i++)
	{
		result += CalcDirectionalLight(directionalLights[i],diffuseTex,specularTex,Normal,viewDir);
	}
	for(int i = 0; i<pointLightNum; i++)
	{
		result += CalcPointLight(pointLights[i],diffuseTex,specularTex,Normal,FragPos,viewDir);
	}
	for(int i = 0; i<flashLightNum; i++)
	{
		result += CalcFlashLight(flashLights[i],diffuseTex,specularTex,Normal,FragPos,viewDir);
	}
    FragColor = vec4(pow(result, vec3(1.0/gamma)), 1);
}

vec3 CalcDirectionalLight(DirectionalLight directionalLight, vec4 diffuseTex, vec4 specularTex, vec3 normal, vec3 viewDir)
{
	vec4 fragPosLightSpace = directionalLight.lighgtSpaceMat*vec4(FragPos,1);
	// 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
	// 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(directionalLight.depthMap, projCoords.xy).r; 
    // 取得当前片段在光源视角下的深度
    float currentDepth = projCoords.z;
	if (projCoords.x<=1&&projCoords.x>=0&&projCoords.y<=1&&projCoords.y>=0)
	{
		if (currentDepth-0.005 >closestDepth)
		{
			return vec3(0,0,0);
		}
	}
	vec3 lightDir = normalize(directionalLight.direction);
    // 漫反射
    vec3 diffuse = vec3(max(dot(normal, -lightDir), 0.0) * diffuseTex.rgb * diffuseStrength);
    // 镜面反射
	vec3 specular;
	if (blinn)
	{
		vec3 halfwayDir = -normalize(lightDir + viewDir);
		specular = pow(max(dot(normal, halfwayDir), 0.0), material.shininess) * specularTex.rbg * specularStrength;
	}
    else
	{
		vec3 reflectDir = reflect(normal,-lightDir);
		specular = pow(max(dot(reflectDir, -viewDir), 0.0), material.shininess) * specularTex.rbg * specularStrength;
	}
    // 合并结果
    return (diffuse + specular) * directionalLight.color;
}

vec3 CalcPointLight(PointLight pointLight, vec4 diffuseTex, vec4 specularTex, vec3 normal, vec3 fragPos, vec3 viewDir)
{  	

    vec3 lightDir = normalize(fragPos-pointLight.position);

    // 漫反射 
	vec3 diffuse = vec3(max(dot(normal, -lightDir), 0.0)* diffuseTex.rgb * diffuseStrength);
    
    // 镜面反射
	vec3 specular;
	if (blinn)
	{
		vec3 halfwayDir = -normalize(lightDir + viewDir);
		specular = pow(max(dot(normal, halfwayDir), 0.0), material.shininess) * specularTex.rbg * specularStrength;
	}
    else
	{
		vec3 reflectDir = reflect(normal,-lightDir);
		specular = pow(max(dot(reflectDir, -viewDir), 0.0), material.shininess) * specularTex.rbg * specularStrength;
	}
        
    vec3 result = diffuse + specular;
	//衰减
	float distance = length(pointLight.position-fragPos);
	float attenuation = 1.0/(pointLight.constant+pointLight.linear*distance+pointLight.quadratic*distance*distance);
	return result*attenuation*pointLight.color;
}

vec3 CalcFlashLight(Flashlight flashLight, vec4 diffuseTex, vec4 specularTex, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(fragPos-flashLight.position);
	float theta = dot(lightDir, normalize(flashLight.direction));
	if(theta > flashLight.outerCone) 
	{ 
		// 执行光照计算
		 // diffuse 
        vec3 diffuse = vec3(max(dot(normal, -lightDir), 0.0)* diffuseTex.rgb * diffuseStrength);  
        
        // specular
		vec3 specular;
		if (blinn)
		{
			vec3 halfwayDir = -normalize(lightDir + viewDir);
			specular = pow(max(dot(normal, halfwayDir), 0.0), material.shininess) * specularTex.rbg * specularStrength;
		}
		else
		{
			vec3 reflectDir = reflect(normal,-lightDir);
			specular = pow(max(dot(reflectDir, -viewDir), 0.0), material.shininess) * specularTex.rbg * specularStrength;
		}
        
        // attenuation
        float distance    = length(flashLight.position - fragPos);
        float attenuation = 1.0 / (flashLight.constant + flashLight.linear * distance + flashLight.quadratic * (distance * distance));    		

		if(theta < flashLight.innerCone)
		{
			float strength = (theta-flashLight.outerCone)/(flashLight.innerCone-flashLight.outerCone);
			return (diffuse + specular) * strength * attenuation * flashLight.color;
		}
		else
		{
			return (diffuse + specular) * attenuation * flashLight.color;
		}
	}
	else 
	{
		return vec3(0,0,0);
	}
}