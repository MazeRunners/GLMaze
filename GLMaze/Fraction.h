#pragma once
class Fraction {
public:
	Fraction();
	~Fraction();
	unsigned int getTextureID();

private:
	const unsigned int width = 1200;
	const unsigned int height = 800;
	const int maxIterations = 256;

	const float x0 = -2;
	const float x1 = 1;
	const float y0 = -1;
	const float y1 = 1;

	int* output;
	const char* filename = "./resource/mandelbrot.ppm";
	unsigned int textureID = -1;

	int mandel(float c_re, float c_im);
	void mandelbrot();
	void writePPMImage();
	void loadTextureFromPPM();
};

