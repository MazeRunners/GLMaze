#pragma once
#include "GLPlatform.h"
#include "GLShader.h"

#include "GUI.h"
#include "GameConfig.h"

#include "GLModel.h"
#include "Camera.h"
#include "Collision.h"
#include "Skybox.h"
#include "Cloth.h"
#include "Particle.h"
#include "Fraction.h"

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
	GLShader* clothShader;
	GLShader* particleShader;

	GUI* GUIManager;
	Camera* camera;
	GLModel * model;
	GameConfig config;
	Collision collision;
	Particle* particles;
	Skybox* skybox;
	Cloth* cloth;
	GLModel * ironman;
	Fraction* fraction;

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
	void renderIronman();

	void renderSkybox();
	void renderParticles();
	void renderCloth();

	void drawObjects(GLShader* shader, bool no_texture);
};
