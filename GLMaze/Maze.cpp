#include "Maze.h"

#include <glad/glad.h>

Maze::Maze() {
}

Maze::~Maze() {
}

void Maze::drawForShadow(Lighting* lighting) {
	lighting->beginShadowCalc();
	model.draw(lighting->getShadowShader(), false);
	lighting->endShadowCalc();
}

void Maze::render(glm::vec3 cameraPos, glm::mat4 viewTransformation, Lighting* lighting) {
	viewShader.use();
	viewShader.setInt("shadowMap", 8);
	viewShader.setVec3("viewPos", cameraPos);
	viewShader.setVec3("lightPos", lighting->getLightSpace().position);

	viewShader.setMat4("viewTransformation", viewTransformation);
	viewShader.setMat4("lightSpaceTransformation", lighting->getLightSpace().transformation);

	glActiveTexture(GL_TEXTURE0 + 8);
	glBindTexture(GL_TEXTURE_2D, lighting->getShadowMap());

	model.draw(&viewShader, true);
}
