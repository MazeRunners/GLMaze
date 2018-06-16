#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GLShader.h"

class Skybox {
public:
	Skybox();
	Skybox(const char* up,
		const char* down,
		const char* front,
		const char* back,
		const char* left,
		const char* right);
	~Skybox();
	void draw(GLShader* shader);
private:
	void generateVertices();
	void init();
	void loadTexture(const char* path);

	float* vertices;
	unsigned int* indices;
	unsigned int VAO, VBO, EBO;
	unsigned int texture;


};