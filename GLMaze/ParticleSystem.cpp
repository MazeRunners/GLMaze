#include "ParticleSystem.h"

#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <fstream>

ParticleSystem::ParticleSystem() {
	init();
}

void ParticleSystem::init() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	positionData = new float[MaxParticles * 4];
	colorData = new unsigned char[MaxParticles * 4];

	for (int i = 0; i < MaxParticles; i++) {
		particlesContainer[i].life = -1.0f;
		particlesContainer[i].cameraDistance = -1.0f;
	}

	texture = loadDDS("./resource/particle/smallsnow.DDS");

	glGenBuffers(1, &billVBO);
	glGenBuffers(1, &posVBO);
	glGenBuffers(1, &colVBO);

	glBindBuffer(GL_ARRAY_BUFFER, billVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// The VBO containing the positions & sizes of the particles
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	// Initialize with empty buffer
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glBindBuffer(GL_ARRAY_BUFFER, colVBO);
	// Initialize with empty buffer
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(unsigned char), NULL, GL_STREAM_DRAW);

	glBindVertexArray(0);

	lastTime = glfwGetTime();
}

ParticleSystem::~ParticleSystem() {
	delete[] positionData;
	delete[] colorData;

	glDeleteBuffers(1, &colVBO);
	glDeleteBuffers(1, &posVBO);
	glDeleteBuffers(1, &billVBO);
	glDeleteBuffers(1, &texture);
	glDeleteVertexArrays(1, &VAO);
}

void ParticleSystem::render(Camera::Parameters parameters, Camera::Matrices matrices) {
	simulate(parameters.position);
	draw(parameters, matrices);
}

void ParticleSystem::simulate(glm::vec3 cameraPosition) {
	// compute time
	double currentTime = glfwGetTime();
	double delta = currentTime - lastTime;
	lastTime = currentTime;

	// generate 10 new particles/ms,
	int newparticles = (int)(delta*5000.0);
	if (newparticles > (int)(0.016f*10000.0))
		newparticles = (int)(0.016f*10000.0);

	for (int i = 0; i < newparticles; i++) {
		int particleIndex = findUnusedParticle();
		particlesContainer[particleIndex].life = 10.0f; // particle lives 5s
		particlesContainer[particleIndex].pos = glm::vec3(8.1f, 9.1878f, -3.208f); // position
		float spread = 0.05f;
		glm::vec3 maindir = glm::vec3(-3.0f, 10.0f, 0.0f);

		// generate a random direction; 
		glm::vec3 randomdir = glm::vec3(
			(rand() % 20000 - 10000.0f) / 100.0f,
			(rand() % 20000 - 10000.0f) / 100.0f,
			(rand() % 20000 - 10000.0f) / 100.0f
		);

		particlesContainer[particleIndex].speed = maindir + randomdir * spread;

		particlesContainer[particleIndex].r = 245;
		particlesContainer[particleIndex].g = 240;
		particlesContainer[particleIndex].b = 240;
		particlesContainer[particleIndex].a = 100;

		particlesContainer[particleIndex].size = (rand() % 1000) / 3000.0f + 0.02f;

	}

	// Simulate all particles
	particlesCount = 0;
	for (int i = 0; i < MaxParticles; i++) {
		Particle& p = particlesContainer[i];

		if (p.life > 0.0f) {
			// Decrease life
			p.life -= delta;
			if (p.life > 0.0f) {
				p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f;
				p.pos += p.speed * (float)delta;
				p.cameraDistance = glm::length2(p.pos - cameraPosition);
				//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

				// Fill the GPU buffer
				positionData[4 * particlesCount + 0] = p.pos.x;
				positionData[4 * particlesCount + 1] = p.pos.y;
				positionData[4 * particlesCount + 2] = p.pos.z;

				positionData[4 * particlesCount + 3] = p.size;

				colorData[4 * particlesCount + 0] = p.r;
				colorData[4 * particlesCount + 1] = p.g;
				colorData[4 * particlesCount + 2] = p.b;
				colorData[4 * particlesCount + 3] = p.a;

			}
			else {
				// Particles die;
				p.cameraDistance = -1.0f;
			}

			particlesCount++;

		}
	}

	sortParticles();
}

void ParticleSystem::draw(Camera::Parameters parameters, Camera::Matrices matrices) {
	glBindVertexArray(VAO);

	// Update the buffer
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(float) * 4, positionData);

	glBindBuffer(GL_ARRAY_BUFFER, colVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(unsigned char), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(unsigned char) * 4, colorData);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader
	particleShader.use();

	// Bind texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	particleShader.setInt("myTextureSampler", 0);

	glm::vec3 right = glm::normalize(glm::cross(parameters.up, parameters.front));
	particleShader.setVec3("CameraRight_worldspace", right);
	particleShader.setVec3("CameraUp_worldspace", parameters.up);
	particleShader.setMat4("VP", matrices.transformation);

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
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindVertexArray(0);
}

unsigned int ParticleSystem::loadDDS(const char * imagepath) {
	const int FOURCC_DXT1 = 0x31545844; // Equivalent to "DXT1" in ASCII
	const int FOURCC_DXT3 = 0x33545844; // Equivalent to "DXT3" in ASCII
	const int FOURCC_DXT5 = 0x35545844; // Equivalent to "DXT5" in ASCII

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC) {
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;

	}
	free(buffer);

	return textureID;
}

void ParticleSystem::sortParticles() {
	std::sort(&particlesContainer[0], &particlesContainer[MaxParticles]);
}

int ParticleSystem::findUnusedParticle() {

	for (int i = lastUsedParticle; i < MaxParticles; i++) {
		if (particlesContainer[i].life < 0) {
			lastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < lastUsedParticle; i++) {
		if (particlesContainer[i].life < 0) {
			lastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}