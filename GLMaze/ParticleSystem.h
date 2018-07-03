#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include "Camera.h"
#include "GLShader.h"

class ParticleSystem {
public:
	ParticleSystem();
	~ParticleSystem();
	void render(Camera::Parameters parameters, Camera::Matrices matrices);

private:
	struct Particle {
		glm::vec3 pos, speed;
		unsigned char r, g, b, a; // Color
		float size, angle, weight;
		float life;
		float cameraDistance;

		bool operator<(const Particle& that) const {
			return this->cameraDistance > that.cameraDistance;
		}
	};

	static const int MaxParticles = 1000;
	Particle particlesContainer[MaxParticles];
	int lastUsedParticle = 0;

	double lastTime;
	double delta;
	int particlesCount;

	const float vertices[12] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};

	//float positionData[MaxParticles * 4];
	//unsigned char colorData[MaxParticles * 4];
	float* positionData;
	unsigned char* colorData;

	unsigned int VAO;
	unsigned int billVBO;
	unsigned int posVBO;
	unsigned int colVBO;
	unsigned int texture;

	GLShader particleShader = GLShader("./shader/particle.vert", "./shader/particle.frag");

	void init();

	void simulate(glm::vec3 cameraPosition);
	void draw(Camera::Parameters parameters, Camera::Matrices matrices);

	unsigned int loadDDS(const char * imagepath);
	void sortParticles();
	int findUnusedParticle();
};