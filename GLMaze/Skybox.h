#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GLShader.h"

class Skybox {
public:
	Skybox();
<<<<<<< HEAD
	Skybox(const char* up,
		const char* down,
		const char* front,
		const char* back,
		const char* left,
		const char* right);
=======
	Skybox(const char* path[6]);
>>>>>>> feature/lun
	~Skybox();
	void draw(GLShader* shader);
private:
	void generateVertices();
	void init();
<<<<<<< HEAD
	void loadTexture(const char* path);
=======
	void loadTexture(const char* path[6]);
>>>>>>> feature/lun

	float* vertices;
	unsigned int* indices;
	unsigned int VAO, VBO, EBO;
<<<<<<< HEAD
	unsigned int texture;
=======
	unsigned int texture[6];
>>>>>>> feature/lun


};