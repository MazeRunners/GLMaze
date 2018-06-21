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

	model = new GLModel("./resource/maze.obj");
	const char* path[] = {
		"./resource/skybox/miramar_ft.png",
		"./resource/skybox/miramar_bk.png",
		"./resource/skybox/miramar_up.png",
		"./resource/skybox/miramar_dn.png",
		"./resource/skybox/miramar_rt.png",
		"./resource/skybox/miramar_lf.png"
	};
	skybox = new Skybox(path);

<<<<<<< HEAD
	p = new ParticleSystem(); // ³õÊ¼»¯init
=======
	// particles
	particles = new Particle(); // �޳�ʼ��init
>>>>>>> master
}

Game::~Game() {
	delete shadowShader;
	delete viewShader;
	delete GUIManager;
	delete camera;
	delete model;
	delete skybox;
	delete p;
}

void Game::start() {
	const GLPlatform::Context context = platfrom.getContext();

	calculateLightSpaceTransformation();

	bool noMove = false;

	while (!glfwWindowShouldClose(context.window)) {
		glfwPollEvents();
		GUIManager->recordUserInput();

		Camera::Parameters camera_args = camera->calcNextParameter(GUIManager->getUserInput());
		collision.update(camera_args.position.x, camera_args.position.y, camera_args.position.z);
		if (!collision.testCollision()) {
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
	particleShader = new GLShader("./shader/particle.vert", "./shader/particle.frag");
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

void Game::renderSkybox() {
	skyShader->use();
	glm::mat4 viewTransformation = camera->getViewTransformation();
	skyShader->setMat4("viewTransformation", viewTransformation);
	skybox->draw();
}

void Game::renderParticles()
{
<<<<<<< HEAD
	p->calc(camera);
	p->draw(camera);
=======
	particles->simulate(camera);
	particles->draw(camera);
>>>>>>> master
}

