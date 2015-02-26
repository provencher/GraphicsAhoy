#include "collisionBox.h"
#include <iostream>
#include "glut.h"
/*
constructor, put in the dimensions of the object
*/
BoundingBox::BoundingBox(){}
BoundingBox::BoundingBox(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
	min.x=xmin;
	min.y=ymin;
	min.z=zmin;
	max.x=xmax;
	max.y=ymax;
	max.z=zmax;
}
/*
used to update the "position of the box"
*/
void BoundingBox::changeLocation(float xmove, float ymove, float zmove)
{
	min.x += xmove;
	min.y += ymove;
	min.z += zmove;

	max.x += xmove;
	max.y += ymove;
	max.z += zmove;
}
/*
set the coordinates of the box corners
*/
void BoundingBox::setDimension(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
	min.x = xmin;
	min.y = ymin;
	min.z = zmin;
	max.x = xmax;
	max.y = ymax;
	max.z = zmax;
}
/*
simple check to see if our object is inside another box
*/
bool BoundingBox::collision(BoundingBox *b)
{
/*	std::cout << "Coordinates of ship's box " << max.x << ","<<max.y<<","<<max.z<<"."<<std::endl;
	std::cout << "Coordinates of ring's box " << b->max.x << "," << b->max.y<<","<<b->max.z<<std::endl;

	std::cout << "Coordinates of ship's box " << min.x << ","<<min.y<<","<<min.z<<"."<<std::endl;
	std::cout << "Coordinates of ring's box " << b->min.x << "," << b->min.y<<","<<b->min.z<<std::endl;*/

	return max.x <= b->max.x && min.x >= b->min.x &&
		max.y <= b->max.y && min.y >= b->min.y &&
		max.z <= b->max.z && min.z >= b->min.z;
	
}
bool BoundingBox::collisionB(Vector3 *v)
{
	
	return max.x >= v->x && min.x <= v->x &&
		max.y >= v->y && min.y <= v->y &&
		max.z >= v->z && min.z <= v->z;
}
/*
check to see if you're colliding with a list of boxes
*/
bool BoundingBox::completeCollisionTest(std::vector<BoundingBox> boxes)
{
	bool rez = false;
	/*for(std::vector<BoundingBox>::iterator i = boxes.begin(); i < boxes.end() ; ++i)
	{
		BoundingBox temp = *i;
		rez = rez || collision(&temp);
	}*/
	for(std::vector<BoundingBox>::iterator i = boxes.begin(); i < boxes.end(); ++i)
	{
		rez = rez || (*i).collisionB(&min);
		rez = rez || (*i).collisionB(&max);
		
	}
	return rez;
}
/*
for debugging
*/
void BoundingBox::drawbox()
{
	GLfloat diffuse[] = {1.0f,1.0f,1.0f,1.0f};
	GLfloat mat_ambient[] = {0.19125f,0.0735f,0.0225f,1.0f};
	GLfloat mat_diffuse[] = {0.7038f, 0.27048f, 0.0828f, 1.0f};
	GLfloat mat_specular[] = {0.256777f, 0.137622f, 0.086014f, 1.0f};
	GLfloat mat_shininess = .1;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);

	glBegin (GL_LINE_LOOP);
	glVertex3f(max.x,max.y,min.z); // 0
	glVertex3f(min.x,max.y,min.z); // 1
	glVertex3f(min.x,min.y,min.z); // 2
	glVertex3f(max.x,min.y,min.z); // 3
	glEnd();

	glBegin (GL_LINE_LOOP);
	glVertex3f(max.x,min.y,max.z); // 4
	glVertex3f(max.x,max.y,max.z); // 5
	glVertex3f(min.x,max.y,max.z); // 6
	glVertex3f(min.x,min.y,max.z); // 7
	glEnd();

	glBegin (GL_LINE_LOOP);
	glVertex3f(max.x,max.y,min.z); // 0
	glVertex3f(max.x,max.y,max.z); // 5
	glVertex3f(min.x,max.y,max.z); // 6
	glVertex3f(min.x,max.y,min.z); // 1
	glEnd();

	glBegin (GL_LINE_LOOP);
	glVertex3f(max.x,min.y,max.z); // 4
	glVertex3f(min.x,min.y,max.z); // 7
	glVertex3f(min.x,min.y,min.z); // 2
	glVertex3f(max.x,min.y,min.z); // 3

	glEnd();

} 