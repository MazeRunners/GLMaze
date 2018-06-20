#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "Particle.h"

using namespace std;
Particle::Particle() {
	init();
}

Particle::~Particle() {
	delete[] vertices;
	delete[] positionData;
	delete[] colorData;
	//delete[] myShader;
}

void Particle::SortParticles() {
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}

int Particle::FindUnusedParticle() {

	for (int i = LastUsedParticle; i<MaxParticles; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i<LastUsedParticle; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void Particle::init()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// create shader
	myShader = new GLShader("./shader/particle.vert", "./shader/particle.frag");

	for (int i = 0; i < MaxParticles; i++) {
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;

		Texture = loadDDS("./resource/particle.DDS");
		vertices= new float[12] {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			0.5f,  0.5f, 0.0f,
		};

		glGenBuffers(1, &billVBO);
		glBindBuffer(GL_ARRAY_BUFFER, billVBO);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

		// The VBO containing the positions & sizes of the particles
		glGenBuffers(1, &posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		// Initialize with empty buffer
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);

		// The VBO containing the colors of the particles
		glGenBuffers(1, &colVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colVBO);
		// Initialize with empty buffer
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(unsigned int), NULL, GL_STREAM_DRAW);
	}

	lastTime = glfwGetTime();
}

void Particle::simulateParticles(Camera* myCamera)
{
	// compute time
	double currentTime = glfwGetTime();
	double delta = currentTime - lastTime;
	lastTime = currentTime;

	glm::vec3 CameraPosition = myCamera->getParameter().position;

	// generate 10 new particles/ms,
	int newparticles = (int)(delta*10000.0);
	if (newparticles > (int)(0.016f*10000.0))
		newparticles = (int)(0.016f*10000.0);

	for (int i = 0; i<newparticles; i++) {
		int particleIndex = FindUnusedParticle();
		ParticlesContainer[particleIndex].life = 5.0f; // particle lives 5s
		ParticlesContainer[particleIndex].pos = glm::vec3(0, 0, -20.0f);
		float spread = 1.5f;
		glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);

		// generate a random direction; 
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
		);

		ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;

		// generate a random color
		ParticlesContainer[particleIndex].r = rand() % 256;
		ParticlesContainer[particleIndex].g = rand() % 256;
		ParticlesContainer[particleIndex].b = rand() % 256;
		ParticlesContainer[particleIndex].a = (rand() % 256) / 3;

		ParticlesContainer[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;

	}

	// Simulate all particles
	int ParticlesCount = 0;
	for (int i = 0; i<MaxParticles; i++) {
		particle& p = ParticlesContainer[i];

		if (p.life > 0.0f) {

			// Decrease life
			p.life -= delta;
			if (p.life > 0.0f) {
				p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f;
				p.pos += p.speed * (float)delta;
				p.cameradistance = glm::length2(p.pos - CameraPosition);
				//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

				// Fill the GPU buffer
				positionData[4 * ParticlesCount + 0] = p.pos.x;
				positionData[4 * ParticlesCount + 1] = p.pos.y;
				positionData[4 * ParticlesCount + 2] = p.pos.z;

				positionData[4 * ParticlesCount + 3] = p.size;

				colorData[4 * ParticlesCount + 0] = p.r;
				colorData[4 * ParticlesCount + 1] = p.g;
				colorData[4 * ParticlesCount + 2] = p.b;
				colorData[4 * ParticlesCount + 3] = p.a;

			}
			else {
				// Particles die;
				p.cameradistance = -1.0f;
			}

			ParticlesCount++;

		}
	}

	SortParticles();

	// Update the buffer
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(float) * 4, positionData);

	glBindBuffer(GL_ARRAY_BUFFER, colVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(unsigned int), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(unsigned int) * 4, colorData);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader
	myShader->use();

	// Bind texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	myShader->setInt("myTextureSampler", 0);

	glm::vec3 right = glm::normalize(glm::cross(myCamera->getParameter().up, myCamera->getParameter().front));
	myShader->setVec3("CameraRight_worldspace", right);
	myShader->setVec3("CameraUp_worldspace", myCamera->getParameter().up);
	myShader->setMat4("VP", myCamera->getViewTransformation());

	// Set up attributes
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, colVBO);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

	glVertexAttribDivisor(0, 0); // particles vertices
	glVertexAttribDivisor(1, 1); // positions
	glVertexAttribDivisor(2, 1); // color

    // Draw particles
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
