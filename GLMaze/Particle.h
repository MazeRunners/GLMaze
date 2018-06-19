#pragma once

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;


#include "shader.hpp"
#include "texture.hpp"
#include "Camera.h"
#include "GLShader.h"

struct particle{
	glm::vec3 pos, speed;
	unsigned char r,g,b,a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

class Particle {
public:
	Particle();
	Particle(glm::vec3 cameraFront, glm::vec3 cameraUp, glm::vec3 cameraPosition, glm::mat4 viewMatrix);
	~Particle();
	void draw();
	void init();
	void generateVertices();
	void generateParticles();
	void simulate();
	void SortParticles();
	int FindUnusedParticle();


private:
	const int MaxParticles = 1;
	particle ParticlesContainer[1];
	int LastUsedParticle = 0;
	double lastTime;
	double delta;
	int ParticlesCount;

	GLfloat* vertices;
	GLuint VAO;
	GLuint TextureID;
	//GLuint particleShader;
	//GLShader particleShader;
	//GLShader particleShader = GLShader("./shader/particle.vs", "./shader/particle.fs");
	GLuint billVBO;
	GLuint posVBO;
	GLuint colVBO;
	GLuint Texture;
	GLfloat* g_particule_position_size_data = new GLfloat[1 * 4];
	GLubyte* g_particule_color_data = new GLubyte[1 * 4];
	GLuint CameraRight_worldspace_ID;
	GLuint CameraUp_worldspace_ID;
	GLuint ViewProjMatrixID;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 cameraPos;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
};