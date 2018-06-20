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
	void simulateParticles(Camera* myCamera);

private:
	void SortParticles();
	int FindUnusedParticle();

	static const int MaxParticles = 1000;
	particle ParticlesContainer[MaxParticles];
	int LastUsedParticle = 0;

	double lastTime;
	double delta;
	int ParticlesCount;

	unsigned int VAO;
	unsigned int billVBO;
	unsigned int posVBO;
	unsigned int colVBO;
	unsigned int TextureID;
	unsigned int Texture;

	GLShader* myShader;
	float* vertices;
	float* positionData = new float[MaxParticles * 4];
	unsigned int* colorData = new unsigned int[MaxParticles * 4];

};