#pragma once
#include "GLPlatform.h"
#include "GLShader.h"
#include "GUI.h"
#include "Camera.h"
#include "GLModel.h"
#include "Collision.h"
#include "Skybox.h"
#include "GameConfig.h"
<<<<<<< HEAD
#include "Particle.h"
=======
#include "ParticleSystem.h"
>>>>>>> be19c0cd009e7e82cf37ff6e7ba327a6cce3a19b

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
	GameConfig config;
	Collision collision;
	ParticleSystem* p;
	Skybox* skybox;

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
