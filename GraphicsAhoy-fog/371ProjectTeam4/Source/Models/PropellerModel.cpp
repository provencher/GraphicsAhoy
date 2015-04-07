//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "PropellerModel.h"
#include "CubeModel.h"
#include "../Renderer.h"

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

using namespace glm;

PropellerModel::PropellerModel(vec3 size) : GroupModel(){
	CubeModel* rutter= new CubeModel();
	rutter->SetRotation(glm::vec3(0.0f,0.0f,2.0f), -45.0f);
	rutter->SetScaling(glm::vec3(1.0f,0.15f, 0.3f));
	AddChild(rutter);
	
	CubeModel* rutter2= new CubeModel();
	rutter2->SetRotation(glm::vec3(0.0f,0.0f,2.0f), 45.0f);
	rutter2->SetScaling(glm::vec3(1.0f,0.15f, 0.3f));
	AddChild(rutter2);

}

PropellerModel::~PropellerModel(){
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void PropellerModel::Update(float dt){
	GroupModel::Update(dt);
}

void PropellerModel::Draw(){
	GroupModel::Draw();
}


bool PropellerModel::ParseLine(const std::vector<ci_string> &token){
	if (token.empty()){
		return true;
	} else {
		return Model::ParseLine(token);
	}
}
