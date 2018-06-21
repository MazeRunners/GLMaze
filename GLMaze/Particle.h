#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;


//#include "shader.hpp"

#include "Camera.h"
#include "GLShader.h"
#include "Texture.h"

struct particle{
	glm::vec3 pos, speed;
	unsigned char r,g,b,a; // Color
	float size, angle, weight;
	float life; 
	float cameradistance; 

	bool operator<(const particle& that) const {
		return this->cameradistance > that.cameradistance;
	}
};

class Particle {
public:
	Particle();
	~Particle();

	void init();
	void simulate(Camera* camera);
	void draw(Camera* camera);

private:
	static const int MaxParticles = 100000;
	particle ParticlesContainer[MaxParticles];
	int LastUsedParticle = 0;

	double lastTime;
	double delta;
	int ParticlesCount;

	const float vertices[12] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};

	unsigned int VAO;
	unsigned int billVBO;
	unsigned int posVBO;
	unsigned int colVBO;
	unsigned int Texture;

	GLShader* myShader;
	//float positionData[MaxParticles * 4];
	//unsigned char colorData[MaxParticles * 4];
	float* positionData;
	unsigned char* colorData;

	unsigned int loadDDS(const char * imagepath);
	void SortParticles();
	int FindUnusedParticle();
};