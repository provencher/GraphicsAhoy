#include <fstream>
#include <iostream>
#include "textureLoader.h"

GLuint LoadTextureRAW(const char*filename, int wrap, int width, int height)
{
	// glMatrixMode(GL_TEXTURE);
	GLuint texture;

	unsigned char * data;
	FILE * file;

	//open texture file
	file = fopen(filename, "rb");
	//allocate buffer
	data = (unsigned char*)malloc(width * height * 3);

	//read texture
	fread(data, width * height * 3, 1, file);
	fclose(file);

	//allocate the texture's name and then select to use
	glGenTextures(1, &texture);
	//glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	/*
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 
		width, height, 
		GL_RGB, GL_UNSIGNED_BYTE, 
		data);
	*/

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB);

	free(data);

	return texture;
}

int LoadBMP(GLuint glTex, char *fileName)
{	
	SDL_Surface *g_image_surface = NULL; 
    g_image_surface = IMG_Load(fileName);

    glBindTexture(GL_TEXTURE_2D,glTex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    //glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, g_image_surface->format->BytesPerPixel, 
		g_image_surface->w, g_image_surface->h,0,GL_RGB,GL_UNSIGNED_BYTE,g_image_surface->pixels);
    
    SDL_FreeSurface(g_image_surface);

	return 0;
}

GLuint LoadTextureBMP(char *filename, int width, int height, int depth, GLenum colour_type, GLenum filter_type)
{
	GLuint texture;
	GLubyte *raw_bitmap;
	FILE *file;
	glGenTextures(1, &texture);
	//glBindTexture(GL_TEXTURE_2D, texture);
	if ((file = fopen(filename, "rb"))==NULL)
	{
		std::cout << "File not found : %s\n",filename;
		exit(1);
	}
	raw_bitmap = (GLubyte *) malloc (width * height * depth * (sizeof(GLubyte)));
	if (raw_bitmap == NULL)
	{
		std::cout << "Cannot allocate memory for texture\n";
		fclose(file);
		exit(1);
	}
	fread(raw_bitmap, width*height*depth, 1, file);
	fclose(file);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_type);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, colour_type, width, height,colour_type, GL_UNSIGNED_BYTE, raw_bitmap); 
	free(raw_bitmap);

	return texture;
}
/*
reads a ppm file. taken from Nate tutorials
*/
GLubyte* readPPMFile(char *filename, int *width, int*height)
{
	FILE* fp;
	int i, w, h,d;
	unsigned char* image;
	char head[70];
	
	fp = fopen(filename, "rb");
	if(!fp) 
	{
		perror(filename);
		return NULL;
	}

	 fgets(head, 70, fp);
	if (strncmp(head, "P6", 2)) {
		fprintf(stderr, "%s: Not a raw PPM file\n", filename);
		return NULL;
	}

	i = 0;
	while(i < 3) {
		fgets(head, 70, fp);
		if (head[0] == '#')
			continue;
		if (i == 0)
			i += sscanf(head, "%d %d %d", &w, &h, &d);
		else if (i == 1)
			i += sscanf(head, "%d %d", &h, &d);
		else if (i == 2)
			i += sscanf(head, "%d", &d);
	}
	image = (unsigned char*)malloc(sizeof(unsigned char)*w*h*3);
	fread(image, sizeof(unsigned char), w*h*3, fp);
	fclose(fp);
	*width = w;
	*height = h;
	return image;
	
}
	