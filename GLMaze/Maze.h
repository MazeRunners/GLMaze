#pragma once

#include <glm/glm.hpp>

#include "GLModel.h"
#include "GLShader.h"
#include "Lighting.h"

class Maze {
public:
	Maze();
	~Maze();
	void drawForShadow(Lighting* lighting);
	void render(glm::vec3 cameraPos, glm::mat4 viewTransformation, Lighting* lighting);

private:
	GLModel model = GLModel("./resource/maze/maze.obj");
	GLShader viewShader = GLShader("./shader/view.vert", "./shader/view.frag");
};

