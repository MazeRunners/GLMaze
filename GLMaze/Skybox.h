#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GLShader.h"

class Skybox {
public:
	Skybox();
	Skybox(const char* path[6]);
	~Skybox();
	void draw(GLShader* shader);
private:
	void generateVertices();
	void init();
	void loadTexture(const char* path[6]);

	float* vertices;
	unsigned int* indices;
	unsigned int VAO, VBO, EBO;
	unsigned int texture[6];


};