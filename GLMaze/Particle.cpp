#include "Particle.h"

#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <fstream>

Particle::Particle() {
	init();
}

Particle::~Particle() {
	delete[] positionData;
	delete[] colorData;

	glDeleteBuffers(1, &colVBO);
	glDeleteBuffers(1, &posVBO);
	glDeleteBuffers(1, &billVBO);
	glDeleteBuffers(1, &Texture);
	glDeleteVertexArrays(1, &VAO);
	
	delete myShader;
}



void Particle::init()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// create shader
	myShader = new GLShader("./shader/particle.vert", "./shader/particle.frag");
	positionData = new float[MaxParticles * 4];
	colorData = new unsigned char[MaxParticles * 4];

	for (int i = 0; i < MaxParticles; i++) {
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
	}

	Texture = loadDDS("./resource/particle.DDS");

	glGenBuffers(1, &billVBO);
	glBindBuffer(GL_ARRAY_BUFFER, billVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// The VBO containing the positions & sizes of the particles
	glGenBuffers(1, &posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	// Initialize with empty buffer
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &colVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colVBO);
	// Initialize with empty buffer
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(unsigned char), NULL, GL_STREAM_DRAW);

	glBindVertexArray(0);

	lastTime = glfwGetTime();
}

void Particle::simulate(Camera* myCamera)
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
		ParticlesContainer[particleIndex].pos = glm::vec3(8.1f, 5.1878f, -3.208f); // position
		float spread = 5.5f;
		glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);

		// generate a random direction; 
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
		);

		ParticlesContainer[particleIndex].speed = maindir + randomdir*spread;
		//ParticlesContainer[particleIndex].speed = glm::vec3(0.0f, 0.0f, 0.0f);

		// generate a random color
		/*ParticlesContainer[particleIndex].r = rand() % 256;
		ParticlesContainer[particleIndex].g = rand() % 256;
		ParticlesContainer[particleIndex].b = rand() % 256;
		ParticlesContainer[particleIndex].a = (rand() % 256) / 3;*/

		ParticlesContainer[particleIndex].r = 245;
		ParticlesContainer[particleIndex].g = 240;
		ParticlesContainer[particleIndex].b = 240;
		ParticlesContainer[particleIndex].a = (rand() % 256) / 3;

		ParticlesContainer[particleIndex].size = (rand() % 1000) / 10000.0f + 0.1f;

	}

	// Simulate all particles
	ParticlesCount = 0;
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
}

void Particle::draw(Camera* camera) {
	glBindVertexArray(VAO);

    // Update the buffer
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(float) * 4, positionData);

	glBindBuffer(GL_ARRAY_BUFFER, colVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(unsigned char), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(unsigned char) * 4, colorData);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader
	myShader->use();

	// Bind texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	myShader->setInt("myTextureSampler", 0);

	glm::vec3 right = glm::normalize(glm::cross(camera->getParameter().up, camera->getParameter().front));
	myShader->setVec3("CameraRight_worldspace", right);
	myShader->setVec3("CameraUp_worldspace", camera->getParameter().up);
	myShader->setMat4("VP", camera->getViewTransformation());

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

	glBindVertexArray(0);
}

unsigned int Particle::loadDDS(const char * imagepath) {
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
	switch (fourCC)
	{
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
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
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