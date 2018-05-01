#pragma once

#include "GLPlatform.h"
#include "GLShader.h"
#include "GUI.h"
#include "Camera.h"
#include "Model.h"

class Game {
public:
	Game();
	~Game();
	void start();

private:
	GLPlatform platfrom;
	GLShader* shadowShader;
	GLShader* viewShader;
	GUI* GUIManager;
	Camera* camera;
	Model * model;

	void initShader();
	void initCamera();

	struct LightSpace {
		glm::vec3 position;
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 transformation;
	}lightSpace;

	void calculateLightSpaceTransformation();

	void renderScene();
	void calculateShadowDepth();
	void drawObjects();
};

