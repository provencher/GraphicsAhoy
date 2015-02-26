//http://www.nullterminator.net/gltexture.html
#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include "glut.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

GLuint LoadTextureRAW(const char *filename, int wrap, int width, int height);
GLuint LoadTextureBMP(char *filename, int width, int height, int depth, GLenum colour_type, GLenum filter_type);
GLubyte* readPPMFile(char *filename, int *width, int*height);

int LoadBMP(GLuint glTex, char *fileName);



#endif