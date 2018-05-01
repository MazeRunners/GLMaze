#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceTransformation;

void main()
{
    gl_Position = lightSpaceTransformation *  vec4(aPos, 1.0);
}