#pragma once
#include <glm/glm.hpp>

#include <string>

#include "GLShader.h"

class Skybox {
public:
	Skybox();
	~Skybox();
	void render(glm::mat4 viewTransformation);

private:
	void generateVertices();
	void init();
	unsigned int loadCubemap(const char * faces[]);

	float* vertices;
	unsigned int* indices;
	unsigned int VAO, VBO;
	unsigned int texture;

	GLShader skyShader = GLShader("./shader/skyshader.vert", "./shader/skyshader.frag");
};