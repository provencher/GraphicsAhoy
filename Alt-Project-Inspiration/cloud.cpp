#include <cmath>
#include <stdlib.h>
#include "glut.h"
#include "cloud.h"
#include "collisionBox.h"

float bulbradius = 0.5f, slices = 20, stacks = 20;
float gunshipX = 3.0f, gunshipY = 1.0f, gunshipZ = 2.0f;
float multiplier = 1.5f;
Cloud::Cloud()
{
	crash = false;
	location.setDimension(-multiplier*gunshipX, multiplier*gunshipX, -multiplier*gunshipY, multiplier*gunshipY, -multiplier*gunshipZ, multiplier*gunshipZ);
}
void Cloud::drawCloud()
{
	glPushMatrix();
		GLUquadricObj *node = gluNewQuadric();
		GLfloat grey[] = {0.6, 0.6, 0.6, 1};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, grey);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,grey);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grey);
		
		gluSphere(node, bulbradius, slices, stacks);

		glTranslatef(bulbradius*1.5f,bulbradius*0.5f,0);
		gluSphere(node, bulbradius, slices, stacks);

		glTranslatef(-bulbradius*1.5f,-bulbradius*0.5f,bulbradius*0.3f);
		gluSphere(node, bulbradius, slices, stacks);

		glTranslatef(2*bulbradius*1.5f,-bulbradius*0.5f,-bulbradius*0.3f);
		gluSphere(node, bulbradius, slices, stacks);
		
	glPopMatrix();
}
void Cloud::setBox(float xchange, float ychange, float zchange)
{
	location.changeLocation(xchange, ychange, zchange);
}
void Cloud::makeFunky()
{
	setNoise(map32);
	loopForever();	
	char texture[256][256][3];
	for(unsigned int i = 0; i < 256; ++i)
	{
		for(unsigned int j = 0; j < 256; ++j)
		{
			float color = map256[i*256+j];
			texture[i][j][0]=color;
			texture[i][j][1]=color;
			texture[i][j][2]=color;
		}
	}
	unsigned int ID;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D,ID);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 256, 256, GL_RGB, GL_UNSIGNED_BYTE, texture);

	glMatrixMode(GL_TEXTURE);
	static float x; 
	x+=0.01f;
	glTranslatef(x,0,0);
	//draw the actual cloud texture
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glTexCoord2d(1,1); glVertex3f(0.5f, 0.5f, 0.);
		glTexCoord2d(0,1); glVertex3f(-0.5f, 0.5f, 0.);
		glTexCoord2d(0,0); glVertex3f(-0.5f, -0.5f, 0.);
		glTexCoord2d(1,0); glVertex3f(0.5f, -0.5f, 0.);
	glEnd(); 

	glMatrixMode(GL_MODELVIEW);
}

float Cloud::noise(int x, int y, int random)
{
	int n = x + y * 57 + random *131;
	n = (n<<13) ^ n;
	return (1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589)&0x7fffffff)* 0.000000000931322574615478515625f);
}
void Cloud::setNoise(float *map)
{
	float temp[34][34];
	int random = rand() % 5000;
	for (unsigned int y = 1; y < 33; ++y)
	{
		for(unsigned int x = 1; x < 33; ++x)
		{
			temp[x][y] = 128.0f + noise(x, y, random) * 128.0f;
		}
	}
	
	for(unsigned int x = 1; x < 33; ++x)
	{
		temp[0][x] = temp[32][x];
	temp[33][x] = temp[1][x];
	temp[x][0] = temp[x][32];
	temp[x][33] = temp[x][1];
	}
	temp[0][0] = temp[32][32];
	temp[33][33] = temp[1][1];
	temp[0][33] = temp[32][1];
	temp[33][0] = temp[1][32];

	for(unsigned int y=1; y<33; ++y)
	{
		for(unsigned int x=1; x<33; ++x)
		{
			float center = temp[x][y]/4.0f;
			float sides = (temp[x+1][y] + temp[x-1][y] + temp[x][y+1] + temp[x][y-1])/8.0f;
			float corners = (temp[x+1][y+1] + temp[x+1][y-1] + temp[x-1][y+1] + temp[x-1][y-1])/16.0f;

			map32[((x-1)*32) + (y-1)] = center + sides + corners;
		}

	}
}
float Cloud::interpolate(float x, float y, float *map)
{
	int Xint = (int)x;
	int Yint = (int)y; 

	float Xfrac = x-Xint;
	float Yfrac = y-Yint;

	int X0 = Xint % 32;
	int Y0 = Yint % 32;
	int X1 = (Xint + 1) % 32;
	int Y1 = (Yint + 1) % 32;
	float bot = map[X0*32 + Y0] + Xfrac * (map[X1*32 + Y0] - map[X0*32 + Y0]);
	float top = map[X0*32 + Y1] + Xfrac * (map[X1*32 +  Y1] - map[X0*32 + Y1]);

	return (bot + Yfrac * (top - bot));
}
void Cloud::overlapOctaves(float *map32, float *map256)
{
	for(unsigned int x = 0; x < 256*256; ++x)
	{
		map256[x] = 0;
	}
	for(unsigned int octave = 0; octave < 4; ++octave)
	{
		for (unsigned int x = 0; x <256; ++x)
		{
			for(unsigned int y = 0; y < 256; ++y)
			{
				float scale = 1 / pow(2.0, (double)(3-octave));
				float noise = interpolate(x*scale, y*scale, map32);
				map256[(y*256)+x]+= noise / pow(2.0, (double)octave);
			}
		}
	}
}
void Cloud::expFilter(float *map)
{
	float cover = 20.0f;
	float sharpness = 0.95f; 

	for(unsigned int x = 0; x < 256*256; ++x)
	{
		float c= map[x] - (255.0f-cover);
		if (c<0)
			c=0;
		map[x] = 255.0f - ((float)(pow(sharpness, c))*255.0f);
	}
}
void Cloud::loopForever()
{
	overlapOctaves(map32, map256);
	expFilter(map256);
}
BoundingBox* Cloud::getMainBox()
{
	return &location;
}
bool Cloud::getCrash()
{
	return crash;
}
void Cloud::setCrash(bool set)
{
	crash = set;
}