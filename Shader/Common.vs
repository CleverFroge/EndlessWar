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
	bool haveDiffuse;
    sampler2D diffuse;
    bool alpha;
	bool haveSpecular;
    sampler2D specular;
    float shininess;
	bool haveNormal;
    sampler2D normal;
}; 
uniform Material material;

void main()
{
    TexCoord = aTexCoords;
	Normal = aNormal;
	if (material.haveNormal)
	{
		Normal = texture(material.normal, TexCoord).rgb;
	}
	Normal = normalize(mat3(transpose(inverse(model))) * Normal);

	FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
}