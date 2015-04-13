#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include "GL\glew.h"

// Load a .BMP file using our custom loader
GLuint loadBMP(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);


#endif 