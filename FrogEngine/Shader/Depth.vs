#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform mat4 geometry;
void main()
{
    gl_Position = lightSpaceMatrix * model * geometry * vec4(position, 1.0f);
}