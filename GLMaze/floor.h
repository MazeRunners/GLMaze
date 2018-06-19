#pragma once

//#include <GL\glew.h>
#include <glad/glad.h>
#include <iostream>
#include <ctime>
#include <math.h>
#include <glm/glm.hpp>
#include "GLShader.h"

class Floor {
private:

	GLuint VBO, VAO;
	GLShader *shader;

public:
	Floor();

	~Floor();

	void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

};