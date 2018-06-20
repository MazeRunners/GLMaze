#ifndef TEXTURE_H
#define TEXTURE_H

// Load a .BMP file using our custom loader
unsigned int loadBMP_custom(const char * imagepath);

// Load a .DDS file using GLFW's own loader
unsigned int loadDDS(const char * imagepath);

#endif