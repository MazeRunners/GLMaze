#version 330 core
layout (location = 0) in vec3 posAttr;
layout (location = 1) in vec3 colorAttr;

uniform mat4 transformation;

out vec3 vertColor;

void main() {
    gl_Position = transformation * vec4(posAttr, 1.0);
    vertColor = colorAttr;
}       