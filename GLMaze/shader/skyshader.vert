#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 viewTransformation;

void main()
{
    gl_Position = viewTransformation * vec4(aPos, 1.0);
    TexCoords = aPos;

}