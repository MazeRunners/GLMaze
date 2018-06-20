#pragma once
#include <glad\glad.h>
#include <GLFW/glfw3.h>

#include "GLPlatform.h"
#include "GLShader.h"
#include "GUI.h"
#include "Camera.h"
#include "GLModel.h"
#include "Collision.h"
#include "Skybox.h"
#include "GameConfig.h"
#include "Particle.h"
#include "Texture.h"

class Game {
public:
	Game();
	~Game();
	void start();

private:
	GLPlatform platfrom;
	GLShader* shadowShader;
	GLShader* viewShader;
	GLShader* skyShader;
	GLShader* particleShader;

	GUI* GUIManager;
	Camera* camera;
	GLModel * model;
	Skybox* skybox;
	GameConfig config;
	Collision collision;
	Particle* particles;

	void initShader();
	void initCamera(GameConfig::Parameters config);

	struct LightSpace {
		glm::vec3 position;
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 transformation;
	}lightSpace;

	void calculateLightSpaceTransformation();

	void renderScene();
	void calculateShadowDepth();
	void renderMaze();
	void renderSkybox();
	void renderParticles();
	void drawObjects(GLShader* shader, bool no_texture);
};
