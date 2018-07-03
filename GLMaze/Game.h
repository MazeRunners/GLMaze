#pragma once
#include "GLPlatform.h"

#include "GUI.h"
#include "Camera.h"
#include "Lighting.h"
#include "Maze.h"
#include "Collision.h"
#include "Skybox.h"
#include "Cloth.h"
#include "ParticleSystem.h"
#include "Fraction.h"
#include "Text.h"

struct GLFWwindow;

class Game {
public:
	Game(GLPlatform* platform);
	~Game();
	void start();

private:
	GLFWwindow * window;
	GUI GUIManager;
	Camera camera;
	Lighting lighting;
	Maze maze;
	Collision collision;
	ParticleSystem particles;
	Skybox skybox;
	Cloth cloth;
	Fraction fraction;
	Text text;

	void moveCameraWithCollision();
	void renderScene();
};
