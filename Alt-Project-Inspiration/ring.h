/*
just a basic ring based on some code I found online
http://www.math.ucsd.edu/~sbuss/MathCG/OpenGLsoft/WrapTorus/WrapTorus.html
*/
#ifndef RING_H
#define RING_H
#include <vector>
#include "collisionBox.h"
class Ring
{
public:
	Ring(void);
	~Ring();//calls explode when you fly through?
	void drawRing();
	BoundingBox* getMainBox();
	std::vector<BoundingBox>* getSubBoxes();
	void setBox(float xchange, float ychange, float zchange);
	bool checkIfInside(BoundingBox *Ship);
	bool getDone();
	void setDone(bool state);
//private:
	void spin();//continuously spin
	void explode();//build an animation to simulate ring exploding
	void putVert(float i, float j);
	BoundingBox location;
	std::vector<BoundingBox> boundaries;
	bool done;
};
#endif