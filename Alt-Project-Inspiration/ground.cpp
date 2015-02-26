/*
floor and some rocks
is given pointers to stored textures for grass and rocks
*/
#include <stdio.h>
#include <fstream>
#include "ground.h"
#include "textureLoader.h"
#include "glut.h"

Ground::Ground(GLuint grassText, GLuint rockText) : grassTexture( grassText), rockTexture( rockText)
{}
Ground::Ground(){}
void Ground::drawGround()
{
	const int howFar = 125;
	glPushMatrix();
		//GLfloat thisColorD[] = {0.1f, 0.35f, 0.31f, 1.0f};
		//GLfloat thisColorS[] = {0.45f, 0.55f, 0.45f, 1.0f};
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, thisColorD);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, thisColorS);
		//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, .25);

		glTranslatef(0,-.5,0);
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, grassTexture);
			for(unsigned int i = 0; i < 1; ++i)
			{
				glTranslatef(i,0,0);
				for(unsigned int j = 0; j < 1; ++j)
				{
					glTranslatef(0,j,0);
					glBegin(GL_QUADS);
						glTexCoord2f(0,0);
						//glTexCoord3f(-howFar,-10.0,-howFar);
						glVertex3f(-howFar,-10.0,-howFar);

						glTexCoord2f(1,0);
						//glTexCoord3f(-howFar,-10.0,howFar);
						glVertex3f(-howFar,-10.0,howFar);

						glTexCoord2f(1,1);
						//glTexCoord3f(howFar,-10.0,howFar);
						glVertex3f(howFar,-10.0,howFar);

						glTexCoord2f(0,1);
						//glTexCoord3f(howFar,-10.0,-howFar);
						glVertex3f(howFar,-10.0,-howFar);
					glEnd();
				}
			}
		glPopMatrix();
/*
		glPushMatrix(); 
			//glBindTexture(GL_TEXTURE_2D, rockTexture);
			glTranslatef(.5,0,-3);
			glutSolidCube(.5);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-.25,0,-5);
			glutSolidCube(.5);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-.5,0,-1);
			glutSolidCube(.5);
		glPopMatrix();
*/
	glPopMatrix();

}

void Ground::setGroundTexture(char* filename)
{
	//GLuint texture;
	grassTexture = LoadTextureRAW(filename, 0, 256,256);
	//grassTexture = texture;
}
void Ground::setRockTexture(char* filename)
{
	rockTexture = LoadTextureRAW(filename, 1, 1024,1024);
}