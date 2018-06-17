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
	void draw();
private:
	void generateVertices();
	void init();
	unsigned int loadCubemap(const char * faces[]);

	float* vertices;
	unsigned int* indices;
	unsigned int VAO, VBO;
	unsigned int texture;
};