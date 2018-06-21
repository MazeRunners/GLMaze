#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include "Camera.h"
#include "GLShader.h"

struct Particle {
	glm::vec3 pos, speed;
	unsigned char r, g, b, a;
	float size, angle, weight;
	float life;
	float cameradistance;

	bool operator<(const Particle& that) const {
		return this->cameradistance > that.cameradistance;
	}
};

class ParticleSystem {
public:
	ParticleSystem();
	~ParticleSystem();
	void calc(Camera* camera);
	void draw(Camera* camera);

private:
	const int MaxParticles = 1000;
	Particle ParticlesContainer[1000];
	int LastUsedParticle = 0;

	unsigned int VertexArrayID;
	GLShader* shader;

	float* g_particule_position_size_data;
	unsigned char* g_particule_color_data;

	unsigned int Texture;

	const float g_vertex_buffer_data[12] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};

	unsigned int billboard_vertex_buffer;
	unsigned int particles_position_buffer;
	unsigned int particles_color_buffer;

	int ParticlesCount;

	double currentTime;
	double lastTime;
	double delta;

	unsigned int loadDDS(const char * imagepath);

	int FindUnusedParticle();
	void SortParticles();
};