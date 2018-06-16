#include "Skybox.h"
#include <stb_image.h>
#include <iostream>
#include <fstream>

Skybox::Skybox()
{
}

Skybox::Skybox(const char * up, 
	const char * down, 
	const char * front, 
	const char * back, 
	const char * left, 
	const char * right) 
{
	init();
	loadTexture(down);
}

Skybox::~Skybox()
{
	delete [] vertices;
	delete [] indices;
}

void Skybox::draw(GLShader* shader)
{
	// bind Texture
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, texture);

	// render container
	shader->use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Skybox::generateVertices()
{
	vertices = new float[32]{
		// positions          // colors           // texture coords
		6.0f,  6.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		6.0f, -6.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		-6.0f, -6.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		-6.0f,  6.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
	};
	indices = new unsigned int[6]{
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
}

void Skybox::init()
{
	generateVertices();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Skybox::loadTexture(const char* path)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
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
