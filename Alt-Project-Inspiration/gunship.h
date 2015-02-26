#ifndef GUNSHIP_H
#define GUNSHIP_H
#include "collisionBox.h"
class Gunship
{
public:

	Gunship(void);
	void drawGunship();
	void setPropeller(bool state);
	bool getPropellerState();
	void setMovement(bool state);
	bool getMovementState();
	void setMaterials(bool state);	
	bool getMaterials();
	void setRotate(float x, float y, float z);
	float getRotateX();
	float getRotateY();
	float getRotateZ();
	void setPosition(float x, float y, float z);
	float getXPosition();
	float getYPosition();
	float getZPosition();
	float getGunshipMovementX();
	float getGunshipMovementY();
	float getGunshipMovementZ();
	BoundingBox* getGunBox();
	void Gunship::drawExplosion(float i);
	bool getExplode();
	void setExplode(bool state);
//private:
	void propeller(float length, float height, float width);
	void propellers(float length, float width, float height);
	void rotor(float radius, float rotation);
	void tail();
	void cockpit(double scale);
	void leg();
	void engine();
	void gunship(void);
	
	BoundingBox currentLocation;
	bool explode;
};
#endif
