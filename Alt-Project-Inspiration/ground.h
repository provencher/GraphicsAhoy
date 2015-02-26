#ifndef GROUND_H
#define GOUND_H
#include "glut.h"
class Ground
{
public:
	Ground(GLuint grassText, GLuint rockText);
	Ground();
	void drawGround();
	void setGroundTexture(char* filename);
	void setRockTexture(char* filename);
private:
	GLuint grassTexture, rockTexture;
};

#endif