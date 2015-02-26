#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
	Camera();
	void setCamera(float eX, float eY, float eZ, float lX, float lY, float lZ);
	void lookAt();
};
#endif
