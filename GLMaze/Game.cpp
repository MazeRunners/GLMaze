#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

Game::Game(GLPlatform* platform) :GUIManager(platform->getWindow()) {
	this->window = platform->getWindow();
}

Game::~Game() {
}

void Game::start() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		GUIManager.recordUserInput();

		moveCameraWithCollision();
		renderScene();

		GUIManager.draw();
		GUIManager.render();
		glfwSwapBuffers(window);
	}
}

void Game::moveCameraWithCollision() {
	Camera::Parameters camera_args = camera.calcNextParameter(GUIManager.getUserInput());
	collision.updateCameraBody(camera_args.position.x, camera_args.position.y, camera_args.position.z);
	if (!collision.testCollision()) {
		camera.setTo(camera_args);
	}
}

void Game::renderScene() {
	Camera::Parameters cameraParameter = camera.getParameter();
	Camera::Matrices cameraMat = camera.getMatrices();
	glm::vec3 cameraPos = cameraParameter.position;
	glm::mat4 viewTransformation = cameraMat.transformation;

	lighting.calculateLightSpaceTransformation();

	glViewport(0, 0, 1280, 720);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	maze.drawForShadow(&lighting);
	maze.render(cameraPos, viewTransformation, &lighting);
	skybox.render(viewTransformation);
	cloth.render(cameraPos, viewTransformation, &lighting);
	text.renderText();
	particles.render(cameraParameter, cameraMat);
}