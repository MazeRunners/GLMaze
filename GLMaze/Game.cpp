#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

Game::Game() {
	initShader();

	GameConfig::Parameters configuration = config.getParameters();
	GUIManager = new GUI(platfrom.getContext().window, configuration);
	initCamera(configuration);

	model = new GLModel("./resource/maze/maze.obj");
	const char* path[] = {
		"./resource/skybox/miramar_ft.png",
		"./resource/skybox/miramar_bk.png",
		"./resource/skybox/miramar_up.png",
		"./resource/skybox/miramar_dn.png",
		"./resource/skybox/miramar_rt.png",
		"./resource/skybox/miramar_lf.png"
	};
	skybox = new Skybox(path);
	cloth = new Cloth(0.01, 20, 20, "./resource/cloth.jpg");

	particles = new Particle();
	fraction = new Fraction();

	text = new Text();
}

Game::~Game() {
	delete shadowShader;
	delete viewShader;
	delete clothShader;
	delete GUIManager;
	delete camera;
	delete model;
	delete skybox;
	delete particles;
	delete fraction;
	delete text;
}

void Game::start() {
	const GLPlatform::Context context = platfrom.getContext();

	calculateLightSpaceTransformation();

	bool noMove = false;

	while (!glfwWindowShouldClose(context.window)) {
		glfwPollEvents();
		GUIManager->recordUserInput();

		Camera::Parameters camera_args = camera->calcNextParameter(GUIManager->getUserInput());
		collision.updateCameraBody(camera_args.position.x, camera_args.position.y, camera_args.position.z);
		if (!collision.testCollision() || true) {
			camera->moveTo(camera_args);
		}

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
	clothShader = new GLShader("./shader/clothShader.vert", "./shader/clothShader.frag");
	particleShader = new GLShader("./shader/particle.vert", "./shader/particle.frag");
	textShader = new GLShader("./shader/textshader.vert", "./shader/textshader.frag");
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
	glViewport(0, 0, 1280, 720);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	calculateShadowDepth();

	renderSkybox();
	renderMaze();
	renderParticles();
	//renderCloth();
	renderText();
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, platfrom.getContext().shadowDepthMap);

	drawObjects(viewShader, true);
}

void Game::renderIronman()
{
}

void Game::renderSkybox() {
	skyShader->use();
	glm::mat4 viewTransformation = camera->getViewTransformation();
	skyShader->setMat4("viewTransformation", viewTransformation);
	skybox->draw();
}

void Game::renderParticles()
{
	particles->simulate(camera);
	particles->draw(camera);
}

void Game::renderCloth()
{
	clothShader->use();
	clothShader->setVec3("viewPos", camera->getParameter().position);
	clothShader->setVec3("lightPos", lightSpace.position);
	glm::mat4 viewTransformation = camera->getViewTransformation();
	clothShader->setMat4("viewTransformation", viewTransformation);
	//clothShader->setMat4("lightSpaceTransformation", lightSpace.transformation);
	clothShader->setInt("clothTexture", 0);
	// clothShader->setVec3("clothColor", glm::vec3(1.0f, 0, 0));
	cloth->draw();
}

void Game::renderText() {
	glm::vec3 color = glm::vec3(0.4f, 0.2f, 0.8f);
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	textShader->use();
	textShader->setMat4("projection", projection);
	text->RenderText(*textShader, "Maze Runner", 15.0f, 555.0f, 0.9f, glm::vec3(0.9f, 0.9f, 0.9f));
	//text->RenderText(*textShader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.4f, 0.2f, 0.8f));
}
