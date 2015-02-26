/*
bounding boxes for collision detection
*/
#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <vector>
#include <iterator>
class Vector3
{
public:
	float x,y,z;
};

class BoundingBox
{
public:
	/*
	constructors
	*/
	BoundingBox();
	BoundingBox(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
	/*
	modifiers
	*/
	void changeLocation(float xmove, float ymove, float zmove);
	void setDimension(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
	/*
	checkers
	*/

	/*
	checks for a collision with specified coordinates
	*/
	bool collision(BoundingBox *b);
	bool collisionB(Vector3 *v);
	/*
	checks for a collision with a list of boxes
	*/
	bool completeCollisionTest(std::vector<BoundingBox> boxes);
	/*
	to help debug
	*/
	void drawbox();
	/*
	info
	*/
	Vector3 min, max;

};
#endif