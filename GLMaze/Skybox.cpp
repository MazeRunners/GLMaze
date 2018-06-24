#include "Skybox.h"

#include <glad/glad.h>

#include <stb_image.h>

#include <iostream>
#include <fstream>

Skybox::Skybox()
{
}

Skybox::Skybox(const char * path[6])
{
	init();
	texture = loadCubemap(path);
}

Skybox::~Skybox()
{
	delete[] vertices;
}

void Skybox::draw()
{
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Skybox::generateVertices()
{
	vertices = new float[108]{
		// positions          
		-16.0f,  16.0f, -16.0f,
		-16.0f, -16.0f, -16.0f,
		16.0f, -16.0f, -16.0f,
		16.0f, -16.0f, -16.0f,
		16.0f,  16.0f, -16.0f,
		-16.0f,  16.0f, -16.0f,

		-16.0f, -16.0f,  16.0f,
		-16.0f, -16.0f, -16.0f,
		-16.0f,  16.0f, -16.0f,
		-16.0f,  16.0f, -16.0f,
		-16.0f,  16.0f,  16.0f,
		-16.0f, -16.0f,  16.0f,

		16.0f, -16.0f, -16.0f,
		16.0f, -16.0f,  16.0f,
		16.0f,  16.0f,  16.0f,
		16.0f,  16.0f,  16.0f,
		16.0f,  16.0f, -16.0f,
		16.0f, -16.0f, -16.0f,

		-16.0f, -16.0f,  16.0f,
		-16.0f,  16.0f,  16.0f,
		16.0f,  16.0f,  16.0f,
		16.0f,  16.0f,  16.0f,
		16.0f, -16.0f,  16.0f,
		-16.0f, -16.0f,  16.0f,

		-16.0f,  16.0f, -16.0f,
		16.0f,  16.0f, -16.0f,
		16.0f,  16.0f,  16.0f,
		16.0f,  16.0f,  16.0f,
		-16.0f,  16.0f,  16.0f,
		-16.0f,  16.0f, -16.0f,

		-16.0f, -16.0f, -16.0f,
		-16.0f, -16.0f,  16.0f,
		16.0f, -16.0f, -16.0f,
		16.0f, -16.0f, -16.0f,
		-16.0f, -16.0f,  16.0f,
		16.0f, -16.0f,  16.0f
		/*
		vertices = new float[108]{
		// positions
		-7.0f,  14.0f, -7.0f,
		-7.0f,  0.0f, -7.0f,
		7.0f,   0.0f, -7.0f,
		7.0f,  0.0f, -7.0f,
		7.0f,  14.0f, -7.0f,
		-7.0f,  14.0f, -7.0f,

		-7.0f, 0.0f,  7.0f,
		-7.0f, 0.0f, -7.0f,
		-7.0f,  14.0f, -7.0f,
		-7.0f,  14.0f, -7.0f,
		-7.0f,  14.0f,  7.0f,
		-7.0f, 0.0f,  7.0f,

		7.0f,  0.0f, -7.0f,
		7.0f,  0.0f,  7.0f,
		7.0f,  14.0f,  7.0f,
		7.0f,  14.0f,  7.0f,
		7.0f,  14.0f, -7.0f,
		7.0f, 0.0f, -7.0f,

		-7.0f, 0.0f,  7.0f,
		-7.0f,  14.0f,  7.0f,
		7.0f,  14.0f,  7.0f,
		7.0f,  14.0f,  7.0f,
		7.0f,  0.0f,  7.0f,
		-7.0f, 0.0f,  7.0f,

		-7.0f,  14.0f, -7.0f,
		7.0f,  14.0f, -7.0f,
		7.0f,  14.0f,  7.0f,
		7.0f,  14.0f,  7.0f,
		-7.0f,  14.0f,  7.0f,
		-7.0f,  14.0f, -7.0f,

		-7.0f, 0.0f, -7.0f,
		-7.0f, 0.0f,  7.0f,
		7.0f,  0.0f, -7.0f,
		7.0f,  0.0f, -7.0f,
		-7.0f, 0.0f,  7.0f,
		7.0f,  0.0f,  7.0f
	};*/
	};
}

void Skybox::init()
{
	generateVertices();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}
unsigned int Skybox::loadCubemap(const char* faces[])
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char *data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}