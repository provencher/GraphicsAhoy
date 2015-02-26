#include <iostream>

#include "Camera.h"
#include "glut.h"


float	xEye = 0.0f;
float	yEye = 2.0f;
float	zEye = 5.0f;

float	xLook = 0.0f;
float	yLook = 0.0f;
float	zLook = 0.0f;
 
float	xUp = 0.0f;
float	yUp = 1.0f;
float	zUp = 0.0f;

Camera::Camera()
{
}

void Camera::setCamera(float eX, float eY, float eZ, float lX, float lY, float lZ)
{
	xEye = eX;
	yEye = eY;
	zEye = eZ;

	xLook = lX;
	yLook = lY;
	zLook = lZ;

}

void Camera::lookAt()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	gluLookAt(xEye, yEye, zEye, xLook, yLook, zLook, xUp,yUp, zUp);

	//std::cout << xEye << " " << yEye << " " << zEye << std::endl;
}