#version 330 core

layout (location = 0) in vec3 posAttr;
layout (location = 1) in vec3 normalAttr;
layout (location = 2) in vec2 texCoordAttr;

out vec3 fragPos;
out vec3 normal;
out vec4 fragLightSpacePos;
out vec2 texCoord;

uniform mat4 lightSpaceTransformation;
uniform mat4 viewTransformation;

void main()
{
  fragPos = posAttr;
  normal = normalAttr;
  texCoord = texCoordAttr;
  gl_Position = viewTransformation * vec4(posAttr, 1.0);
  fragLightSpacePos = lightSpaceTransformation * vec4(posAttr, 1.0);
}