#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct Material {
    sampler2D diffuse;
    bool alpha;
    sampler2D specular;
    float shininess;
    sampler2D normal;
	sampler2D displacement;
}; 
uniform Material material;

void main()
{
    TexCoord = aTexCoords;
	vec3 displacement = texture(material.displacement, TexCoord).rgb;
	Normal = texture(material.normal, TexCoord).rgb;
	if (Normal==vec3(0,0,0))
	{
		Normal = aNormal;
	}
	Normal = aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
    
    gl_Position = projection * view * vec4(FragPos, 1.0);

	Normal = mat3(transpose(inverse(model))) * Normal;  
}
/*
void main()
{
    TexCoord = aTexCoords;
	vec3 displacement = texture(material.displacement, TexCoord).rgb;
	Normal = texture(material.normal, TexCoord).rgb;
	if (Normal==vec3(0,0,0))
	{
		Normal = aNormal;
	}
	FragPos = vec3(model * vec4(aPos+Normal*displacement, 1.0));
    
    gl_Position = projection * view * vec4(FragPos, 1.0);

	Normal = mat3(transpose(inverse(model))) * Normal;  
}
*/