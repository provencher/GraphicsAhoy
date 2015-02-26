//http://www.gamedev.net/reference/articles/article2085.asp
#ifndef CLOUD_H
#define CLOUD_H
#include "collisionBox.h"
class Cloud
{
public:
	Cloud();
	void drawCloud();
	void setBox(float xchange, float ychange, float zchange);
	BoundingBox* getMainBox();
	bool getCrash();
	void setCrash(bool set);
private:
	float noise(int x, int y, int random);
	void setNoise(float *map);
	void makeFunky();
	float interpolate(float x, float y, float *map);
	void overlapOctaves(float *map32, float *map256);
	void expFilter(float *map);
	void loopForever();

	float map32[32 * 32];
	float map256[256 * 256];
	BoundingBox location;
	bool crash;
};
#endif