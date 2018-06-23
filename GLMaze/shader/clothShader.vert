#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 viewTransformation;

void main()
{
    gl_Position = viewTransformation * vec4(aPos, 1.0);
}