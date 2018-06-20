#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;


//#include "shader.hpp"

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
	static const int MaxParticles = 1000;
	particle ParticlesContainer[MaxParticles];
	int LastUsedParticle = 0;
	double lastTime;
	double delta;
	int ParticlesCount;

	float* vertices;
	unsigned int VAO;
	unsigned int TextureID;
	//GLuint particleShader;
	//GLShader particleShader;
	//GLShader particleShader = GLShader("./shader/particle.vs", "./shader/particle.fs");
	unsigned int billVBO;
	unsigned int posVBO;
	unsigned int colVBO;
	unsigned int Texture;
	float* g_particule_position_size_data = new float[MaxParticles * 4];
	unsigned int* g_particule_color_data = new unsigned int[MaxParticles * 4];
	unsigned int CameraRight_worldspace_ID;
	unsigned int CameraUp_worldspace_ID;
	unsigned int ViewProjMatrixID;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 cameraPos;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
};