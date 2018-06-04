#pragma once

#include <string>
#include <vector>

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
	void draw(GLShader* shader);
private:
	void generateVertices();
	void init();
	void loadTexture(const char* path);

	std::vector<float> vertices;
	unsigned int VAO, VBO;
	unsigned int texture;


};