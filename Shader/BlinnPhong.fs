#version 330 core
out vec4 FragColor;

float gamma = 2.2;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoord;

#define NR_DIRECTIONAL_LIGHTS 1
#define NR_POINT_LIGHTS 4
#define NR_FLASH_LIGHTS 4

float ambientStrength = 0;
float diffuseStrength = 0;
float specularStrength = 1;

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

vec3 CalcDirectionalLight(DirectionalLight directionalLight, vec4 specularTex, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight pointLight, vec4 specularTex, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcFlashLight(Flashlight flashLight, vec4 specularTex, vec3 normal, vec3 fragPos, vec3 viewDir);

bool blinn = true;

void main()
{
    vec4 diffuseTex = vec4(0,0,0,0);
	if (material.haveDiffuse)
	{
		diffuseTex = vec4(pow(texture(material.diffuse, TexCoord).rgb, vec3(gamma)), 1);
	}
	vec4 specularTex = vec4(0.5,0.5,0.5,0);
	if (material.haveSpecular)
	{
		vec4 specularTex = texture(material.specular, TexCoord);
	}
    vec3 viewDir = normalize(FragPos-viewPos);
	vec3 result = vec3(0,0,0);
	for(int i = 0; i<directionalLightNum; i++)
	{
		result += CalcDirectionalLight(directionalLights[i],specularTex,Normal,viewDir);
	}
	for(int i = 0; i<pointLightNum; i++)
	{
		result += CalcPointLight(pointLights[i],specularTex,Normal,FragPos,viewDir);
	}
	for(int i = 0; i<flashLightNum; i++)
	{
		result += CalcFlashLight(flashLights[i],specularTex,Normal,FragPos,viewDir);
	}
	result = result *diffuseTex.rgb+ vec3(diffuseTex) * ambientStrength;
    FragColor = vec4(pow(result, vec3(1.0/gamma)), 1);
} 

vec3 CalcDirectionalLight(DirectionalLight directionalLight, vec4 specularTex, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(directionalLight.direction);
    // 漫反射
    vec3 diffuse = vec3(max(dot(normal, -lightDir), 0.0) * diffuseStrength);
    // 镜面反射
	vec3 specular;
	if (blinn)
	{
		vec3 halfwayDir = -normalize(lightDir + viewDir);
		specular = pow(max(dot(normal, halfwayDir), 0.0), material.shininess)*vec3(specularTex) * specularStrength;
	}
    else
	{
		vec3 reflectDir = reflect(normal,-lightDir);
		specular = pow(max(dot(reflectDir, -viewDir), 0.0), material.shininess)*vec3(specularTex) * specularStrength;
	}
    // 合并结果
    return (diffuse + specular) * directionalLight.color;
}

vec3 CalcPointLight(PointLight pointLight, vec4 specularTex, vec3 normal, vec3 fragPos, vec3 viewDir)
{  	

    vec3 lightDir = normalize(fragPos-pointLight.position);

    // 漫反射 
	vec3 diffuse = vec3(max(dot(normal, lightDir), 0.0) * diffuseStrength);
    
    // 镜面反射
	vec3 specular;
	if (blinn)
	{
		vec3 halfwayDir = -normalize(lightDir + viewDir);
		specular = pow(max(dot(normal, halfwayDir), 0.0), material.shininess)*vec3(specularTex) * specularStrength;
	}
    else
	{
		vec3 reflectDir = reflect(normal,-lightDir);
		specular = pow(max(dot(reflectDir, -viewDir), 0.0), material.shininess)*vec3(specularTex) * specularStrength;
	}
        
    vec3 result = diffuse + specular;
	//衰减
	float distance = length(pointLight.position-fragPos);
	float attenuation = 1.0/(pointLight.constant+pointLight.linear*distance+pointLight.quadratic*distance*distance);
	return result*attenuation*pointLight.color;
}

vec3 CalcFlashLight(Flashlight flashLight, vec4 specularTex, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(fragPos-flashLight.position);
	float theta = dot(lightDir, normalize(flashLight.direction));
	if(theta > flashLight.outerCone) 
	{ 
		// 执行光照计算
		 // diffuse 
        vec3 diffuse = vec3(max(dot(normal, -lightDir), 0.0) * diffuseStrength);  
        
        // specular
		vec3 specular;
		if (blinn)
		{
			vec3 halfwayDir = -normalize(lightDir + viewDir);
			specular = pow(max(dot(normal, halfwayDir), 0.0), material.shininess)*vec3(specularTex) * specularStrength;
		}
		else
		{
			vec3 reflectDir = reflect(normal,-lightDir);
			specular = pow(max(dot(reflectDir, -viewDir), 0.0), material.shininess)*vec3(specularTex) * specularStrength;
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