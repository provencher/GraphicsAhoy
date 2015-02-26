#include "ring.h"
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <iterator>
#include <iostream>
#include "glut.h"
#include "collisionBox.h"

const int QuadMode = 1;
const int NumWraps = 20;
const int NumPerWrap = 20;
const float MajorRadius = 4.0;
const float MinorRadius = 0.5;
const float PI2 = 2.0*3.1415926535f;
const float shipSize = 2.0f;
float tempxmin = 5000.0f, tempymin = 5000.0f, tempzmin = 5000.0f;
float tempxmax = -5000.0f, tempymax = -5000.0f, tempzmax = -5000.0f;

Ring::Ring(void)
{
	location.setDimension(-MajorRadius/2,MajorRadius/2,-MajorRadius/2,MajorRadius/2,-shipSize/2,shipSize/2);
	done = false;
}
Ring::~Ring()
{
	explode();
//	delete;
}
void Ring::putVert(float i, float j) {
	float wrapFrac = j/(float)NumPerWrap;
	float phi = PI2*wrapFrac;
	float theta = PI2*(i+wrapFrac)/(float)NumWraps;
	float y = MinorRadius*(float)sin(phi);
	float r = MajorRadius + MinorRadius*(float)cos(phi);
	float x = (float)sin(theta)*r;
	float z = (float)cos(theta)*r;
	glVertex3f(x,y,z);
	/*
	calculate temporary outside boundaries of the disc
	*/
	if(x > tempxmax)
		tempxmax = x;
	if(x < tempxmin)
		tempxmin = x;
	if(y > tempymax)
		tempymax = y;
	if(y < tempymin)
		tempymin = y;
	if(z > tempzmax)
		tempzmax = z;
	if(z < tempzmin)
		tempzmin = z;
}
void Ring::drawRing()
{
	int i,j; 
	glPushMatrix();
		GLfloat diffuse[] = {1.0f,1.0f,1.0f,1.0f};
		GLfloat mat_ambient[] = {0.329412f,0.329412,0.027451f,1.0f};
		GLfloat mat_diffuse[] = {0.780392f, 0.568627f, 0.113725f, 1.0f};
		GLfloat mat_specular[] = {0.992157f, 0.941176f, 0.807843f, 1.0f};
		GLfloat mat_shininess = 27.8;
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
		glBegin( GL_QUAD_STRIP );
			for (i=0; i<NumWraps; i++ ) 
			{
				/*
				make a disc at point i on the ring
				connect it to the next disc that will be made
				create a bounding box around the disc
				reset the temp values
				*/
				for (j=0; j<NumPerWrap; j++) //each disc
				{
					putVert((float)i,(float)j);
					putVert((float)(i+1),(float)j);
				}
				BoundingBox tempBox(tempxmin, tempxmax, tempymin, tempymax, tempzmin, tempzmax);
				boundaries.push_back(tempBox);
				tempxmin = tempymin = tempzmin = 5000.0f;
				tempxmax = tempymax = tempzmax = -5000.0f;
			}
			putVert(0.0,0.0);
			putVert(1.0,0.0);
		glEnd();
	glPopMatrix();
}
bool Ring::getDone()
{
	return done;
}
void Ring::setDone(bool state)
{
	done = state;
}
void Ring:: explode()
{
}
void Ring::spin()
{
}
BoundingBox* Ring::getMainBox()
{
	return &location;
}
std::vector<BoundingBox>* Ring::getSubBoxes()
{
	return &boundaries;
}
void Ring::setBox(float xchange, float ychange, float zchange)
{
	location.changeLocation(xchange, ychange,zchange);
	/*for(std::vector<BoundingBox>::iterator i = boundaries.begin(); i < boundaries.end(); ++i)
	{
		static_cast<BoundingBox>(*i).changeLocation(xchange,ychange,zchange);
	}*/
	std::cout << "Size of this ring's boundaries vector " << boundaries.size() << std::endl;
	for(unsigned int i = 0; i < boundaries.size(); ++i)
	{
		(boundaries[i]).changeLocation(xchange,ychange,zchange);
	}
}