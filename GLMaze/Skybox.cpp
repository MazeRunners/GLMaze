#include "Skybox.h"
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

void Skybox::draw(GLShader* shader)
{
	// render container
	shader->use();
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Skybox::generateVertices()
{
	/*
	vertices = new float[180]{
		// positions          // texture Coords
        -6.0f, 0.0f, -6.0f,  0.0f, 0.0f,
		6.0f,  0.0f, -6.0f,  1.0f, 0.0f,
		6.0f, 12.0f, -6.0f,  1.0f, 1.0f,
		6.0f, 12.0f, -6.0f,  1.0f, 1.0f,
		-6.0f,12.0f, -6.0f,  0.0f, 1.0f,
		-6.0f, 0.0f, -6.0f,  0.0f, 0.0f,

		-6.0f, 0.0f,  6.0f,  0.0f, 0.0f,
		6.0f,  0.0f,  6.0f,  1.0f, 0.0f,
		6.0f, 12.0f,  6.0f,  1.0f, 1.0f,
		6.0f, 12.0f,  6.0f,  1.0f, 1.0f,
		-6.0f,12.0f,  6.0f,  0.0f, 1.0f,
		-6.0f, 0.0f,  6.0f,  0.0f, 0.0f,

		-6.0f, 12.0f,  6.0f,  1.0f, 0.0f,
		-6.0f, 12.0f, -6.0f,  1.0f, 1.0f,
		-6.0f,  0.0f, -6.0f,  0.0f, 1.0f,
		-6.0f,  0.0f, -6.0f,  0.0f, 1.0f,
		-6.0f,  0.0f,  6.0f,  0.0f, 0.0f,
		-6.0f, 12.0f,  6.0f,  1.0f, 0.0f,

		6.0f,  12.0f,  6.0f,  1.0f, 0.0f,
		6.0f,  12.0f, -6.0f,  1.0f, 1.0f,
		6.0f,   0.0f, -6.0f,  0.0f, 1.0f,
		6.0f,   0.0f, -6.0f,  0.0f, 1.0f,
		6.0f,   0.0f,  6.0f,  0.0f, 0.0f,
		6.0f,  12.0f,  6.0f,  1.0f, 0.0f,

		-6.0f,  0.0f, -6.0f,  0.0f, 1.0f,
		6.0f,   0.0f, -6.0f,  1.0f, 1.0f,
		6.0f,   0.0f,  6.0f,  1.0f, 0.0f,
		6.0f,   0.0f,  6.0f,  1.0f, 0.0f,
		-6.0f,  0.0f,  6.0f,  0.0f, 0.0f,
		-6.0f,  0.0f, -6.0f,  0.0f, 1.0f,

		-6.0f, 12.0f, -6.0f,  0.0f, 1.0f,
		6.0f,  12.0f, -6.0f,  1.0f, 1.0f,
		6.0f,  12.0f,  6.0f,  1.0f, 0.0f,
		6.0f,  12.0f,  6.0f,  1.0f, 0.0f,
		-6.0f, 12.0f,  6.0f,  0.0f, 0.0f,
		-6.0f, 12.0f, -6.0f,  0.0f, 1.0f
	};*/
	vertices = new float[108]{
		// positions          
		-6.0f,  6.0f, -6.0f,
		-6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		-6.0f,  6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f,  6.0f
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
/*
void Skybox::loadTexture(const char* path[6])
{
	for (int i = 0; i < 6; i++) {
		glGenTextures(1, &texture[i]);
		glBindTexture(GL_TEXTURE_2D, texture[i]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
											   // set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		unsigned char *data = stbi_load(path[i], &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
}*/

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