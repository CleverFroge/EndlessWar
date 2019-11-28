#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoord;

#define NR_DIRECTIONAL_LIGHTS 1
#define NR_POINT_LIGHTS 4
#define NR_FLASH_LIGHTS 4

float ambientStrength = 0.2;
float diffuseStrength = 0.3;
float specularStrength = 0.5;

struct Material {
    sampler2D diffuse;
    bool alpha;
    sampler2D specular;
    float shininess;
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

vec3 CalcDirectionalLight(DirectionalLight directionalLight, vec4 diffuseTex, vec4 specularTex, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight pointLight, vec4 diffuseTex, vec4 specularTex, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcFlashLight(Flashlight flashLight, vec4 diffuseTex, vec4 specularTex, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec4 diffuseTex = vec4(0.2,0.2,0.2,0);
    vec4 specularTex = vec4(1,1,1,1);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = vec3(diffuseTex) * ambientStrength;
	for(int i = 0; i<directionalLightNum; i++)
	{
		result += CalcDirectionalLight(directionalLights[i],diffuseTex,specularTex,norm,viewDir);
	}
	for(int i = 0; i<pointLightNum; i++)
	{
		result += CalcPointLight(pointLights[i],diffuseTex,specularTex,norm,FragPos,viewDir);
	}
	for(int i = 0; i<flashLightNum; i++)
	{
		result += CalcFlashLight(flashLights[i],diffuseTex,specularTex,norm,FragPos,viewDir);
	}
    FragColor = vec4(result, 1);
} 

vec3 CalcDirectionalLight(DirectionalLight directionalLight, vec4 diffuseTex, vec4 specularTex, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-directionalLight.direction);
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面反射
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // 合并结果
    vec3 diffuse  = diff * vec3(diffuseTex) * diffuseStrength;
    vec3 specular = spec * vec3(specularTex) * specularStrength;
    return (diffuse + specular) * directionalLight.color;
}

vec3 CalcPointLight(PointLight pointLight, vec4 diffuseTex, vec4 specularTex, vec3 normal, vec3 fragPos, vec3 viewDir)
{  	
    vec3 lightDir = normalize(pointLight.position - fragPos);

    // 漫反射 
    float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * vec3(diffuseTex) * diffuseStrength;
    
    // 镜面反射
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = vec3(specularTex) * spec * specularStrength;  
        
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
        float diff = max(dot(normal, -lightDir), 0.0);
        vec3 diffuse = diff * vec3(diffuseTex) * diffuseStrength;  
        
        // specular
        vec3 halfwayDir = normalize(-lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
        vec3 specular = spec * vec3(specularTex) * specularStrength;  
        
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