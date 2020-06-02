#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoords;

struct VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	mat3 TBN;
	vec2 TexCoord;
};

out VS_OUT vs_out;

uniform mat4 geometry;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main()
{
    vs_out.TexCoord = aTexCoords;
	
	vs_out.Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
	vs_out.FragPos = vec3(model * geometry * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}