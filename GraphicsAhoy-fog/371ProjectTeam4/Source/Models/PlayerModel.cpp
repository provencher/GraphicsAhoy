//
//

#include "GroupModel.h"
#include "../Renderer.h"

#include <vector>
#include <GL/glew.h>// Include GLEW - OpenGL Extension Wrangler

using namespace glm;

GroupModel::GroupModel(vec3 size) : Model()
{
	// Create Vertex Buffer for all the verices of the Layer
	vec3 halfSize = size * 0.5f;

	//initialize rotation speed;
	rotationSpeed = 0.0f;
}

GroupModel::~GroupModel()
{

}

bool GroupModel::ParseLine(const std::vector<ci_string> &token){
	if (token.empty()){
		return true;
	} else {
		return Model::ParseLine(token);
	}
}


void GroupModel::Update(float dt)
{
	// If you are curious, un-comment this line to have spinning Layers!
	// That will only work if your world transform is correct...

	if(rotationSpeed != 0.0f){
		mRotationAngleInDegrees += rotationSpeed * dt;
	}
	Model::Update(dt);
}

void GroupModel::Draw(){
	DrawChildren();
	
	
	/*
	// Draw paths
	if(mPath.size() > 0)
		for (vector<Path*>::iterator it = mPath.begin(); it < mPath.end(); ++it)
			(*it)->Draw();
	
	/*
	// Draw B-Spline Lines (using the same shader for Path Lines)
	if(mSpline.size() > 0)
		for (vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it)
			(*it)->Draw();
	*/
}



void GroupModel::SetRotationSpeed(float s){
	rotationSpeed = s;
}

