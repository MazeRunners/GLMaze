#include "Game.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

Game::Game() {
	initShader();
	GUIManager = new GUI(platfrom.getContext().window);
	initCamera();
	model = new Model("C:/Users/Koishi/Documents/CodeStudy/LearnOpenGL-master/resources/objects/nanosuit/nanosuit.obj");
}

Game::~Game() {
	delete shadowShader;
	delete viewShader;
	delete GUIManager;
	delete camera;
	delete model;
}

void Game::start() {
	const GLPlatform::Context context = platfrom.getContext();

	calculateLightSpaceTransformation();

	while (!glfwWindowShouldClose(context.window)) {
		glfwPollEvents();
		GUIManager->recordUserInput();
		camera->moveCamera(GUIManager->getUserInput());

		GUIManager->toNextFrame();
		renderScene();
		GUIManager->render();

		glfwSwapBuffers(context.window);
	}

}

void Game::initShader() {
	shadowShader = new GLShader("shadow.vert", "shadow.frag");
	viewShader = new GLShader("shader.vert", "shader.frag");
	viewShader->use();
	viewShader->setInt("shadowMap", 0);
}

void Game::initCamera() {
	Camera::Parameter cameraParameter;
	cameraParameter.position = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraParameter.front = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraParameter.up = glm::vec3(0.0f, 1.0f, 0.0f);
	camera = new Camera(cameraParameter);
}

void Game::calculateLightSpaceTransformation() {
	lightSpace.position = glm::vec3(-2.0f, 4.0f, 1.0f);

	lightSpace.projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);

	lightSpace.view = glm::lookAt(lightSpace.position,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	lightSpace.transformation = lightSpace.projection * lightSpace.view;
}

void Game::renderScene() {
	calculateShadowDepth();

	viewShader->use();
	viewShader->setVec3("viewPos", camera->getParameter().position);
	viewShader->setVec3("lightPos", lightSpace.position);

	glm::mat4 viewTransformation = camera->getViewTransformation();
	viewShader->setMat4("viewTransformation", viewTransformation);
	viewShader->setMat4("lightSpaceTransformation", lightSpace.transformation);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, platfrom.getContext().shadowDepthMap);

	drawObjects();
}

void Game::drawObjects() {
	model->draw(viewShader);
}

void Game::calculateShadowDepth() {
	shadowShader->use();
	shadowShader->setMat4("lightSpaceTransformation", lightSpace.transformation);

	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, platfrom.getContext().shadowDepthFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	drawObjects();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, 800, 600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}