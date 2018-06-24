#include "Fraction.h"

#include <glad/glad.h>

#include <stb_image.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <string>
#include <iostream>

Fraction::Fraction() {
	output = new int[width*height];
	memset(output, 0, width * height * sizeof(int));

	mandelbrot();
	writePPMImage();
	loadTextureFromPPM();
}

Fraction::~Fraction() {
	delete[] output;
}

unsigned int Fraction::getTextureID() {
	return textureID;
}

int Fraction::mandel(float c_re, float c_im) {
	float z_re = c_re, z_im = c_im;
	int i;
	for (i = 0; i < maxIterations; ++i) {
		if (z_re * z_re + z_im * z_im > 4.f)
			break;

		float new_re = z_re * z_re - z_im * z_im;
		float new_im = 2.f * z_re * z_im;

		z_re = c_re + new_re;
		z_im = c_im + new_im;

	}

	return i;
}

void Fraction::mandelbrot() {
	float dx = (x1 - x0) / width;
	float dy = (y1 - y0) / height;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; ++i) {
			float x = x0 + i * dx;
			float y = y0 + j * dy;

			int index = (j * width + i);
			output[index] = mandel(x, y);
		}
	}
}

void Fraction::writePPMImage() {
	FILE *fp = fopen(filename, "wb");

	fprintf(fp, "P6\n");
	fprintf(fp, "%d %d\n", width, height);
	fprintf(fp, "255\n");

	for (int i = 0; i < width*height; ++i) {
		float mapped = pow(std::min(static_cast<float>(maxIterations),
			static_cast<float>(output[i])) / 256.f, .5f);

		unsigned char result = static_cast<unsigned char>(255.f * mapped);
		for (int j = 0; j < 3; ++j)
			fputc(result, fp);
	}
	fclose(fp);
}

void Fraction::loadTextureFromPPM() {
	std::string path = std::string(filename);

	glGenTextures(1, &textureID);

	int width, height, componentNum;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &componentNum, 0);

	if (data) {

		GLenum format;
		switch (componentNum) {
		case 1: {
			format = GL_RED;
			break;
		}
		case 3: {
			format = GL_RGB;
			break;
		}
		case 4: {
			format = GL_RGBA;
			break;
		}
		default:
			break;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << filename << std::endl;
		stbi_image_free(data);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}
