#pragma once

#include <glm/glm.hpp>
#include "GLShader.h"

class Floor {
public:
	Floor();
	~Floor();
	void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

private:
	unsigned int VBO, VAO;
	GLShader* shader;
};