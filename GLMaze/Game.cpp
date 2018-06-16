#include "Game.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

Game::Game() {
	initShader();

	GameConfig::Parameters configuration = config.getParameters();
	GUIManager = new GUI(platfrom.getContext().window, configuration);
	initCamera(configuration);

	model = new Model("./resource/maze.obj");
	skybox = new Skybox("./resource/skybox/miramar_up.png",
		"./resource/skybox/miramar_dn.png",
		"./resource/skybox/miramar_ft.png",
		"./resource/skybox/miramar_bk.png",
		"./resource/skybox/miramar_lf.png",
		"./resource/skybox/miramar_rt.png");
}

Game::~Game() {
	delete shadowShader;
	delete viewShader;
	delete GUIManager;
	delete camera;
	delete model;
	delete skybox;
}

void Game::start() {
	const GLPlatform::Context context = platfrom.getContext();

	calculateLightSpaceTransformation();

	while (!glfwWindowShouldClose(context.window)) {
		glfwPollEvents();
		GUIManager->recordUserInput();

		camera->moveWithUser(GUIManager->getUserInput());

		renderScene();
		GUIManager->draw();
		GUIManager->render();

		glfwSwapBuffers(context.window);
	}

}

void Game::initShader() {
	shadowShader = new GLShader("./shader/shadow.vert", "./shader/shadow.frag");
	viewShader = new GLShader("./shader/shader.vert", "./shader/shader.frag");
	skyShader = new GLShader("./shader/skyshader.vert", "./shader/skyshader.frag");
	viewShader->use();
	viewShader->setInt("shadowMap", 0);
}

void Game::initCamera(GameConfig::Parameters config) {
	Camera::Parameters cameraParameter;
	cameraParameter.position = config.cameraPos;
	cameraParameter.front = config.cameraFront;
	cameraParameter.up = config.cameraUp;

	cameraParameter.fovy = config.fovy;
	cameraParameter.aspect = config.aspect;
	cameraParameter.z_near = config.z_near;
	cameraParameter.z_far = config.z_far;

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
	renderSkybox();
	renderMaze();
}

void Game::drawObjects(GLShader* shader, bool no_texture) {
	model->draw(shader, no_texture);
}

void Game::calculateShadowDepth() {
	shadowShader->use();
	shadowShader->setMat4("lightSpaceTransformation", lightSpace.transformation);

	glViewport(0, 0, 1280, 720);
	glBindFramebuffer(GL_FRAMEBUFFER, platfrom.getContext().shadowDepthFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	// disable texture when shadow mapping
	drawObjects(shadowShader, false);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::renderMaze() {
	viewShader->use();
	viewShader->setVec3("viewPos", camera->getParameter().position);
	viewShader->setVec3("lightPos", lightSpace.position);

	glm::mat4 viewTransformation = camera->getViewTransformation();
	viewShader->setMat4("viewTransformation", viewTransformation);
	viewShader->setMat4("lightSpaceTransformation", lightSpace.transformation);
	

	glViewport(0, 0, 1280, 720);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, platfrom.getContext().shadowDepthMap);

	drawObjects(viewShader, true);
}

void Game::renderSkybox() {
	glm::mat4 viewTransformation = camera->getViewTransformation();
	skyShader->setMat4("viewTransformation", viewTransformation);

	skybox->draw(skyShader);
}
