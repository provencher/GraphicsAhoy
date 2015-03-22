//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 24/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//
//*///////////////////////////////////////////////////////////////////
#include "BSpline.h"
#include "Renderer.h"
#include <math.h>			//for distance finding
#include <GL/glew.h>		// Include GLEW - OpenGL Extension Wrangler
using namespace glm;




//*///////////////////////////////////////////////////////////////////
BSpline::BSpline() : Model()
{
}
bool BSpline::ParseLine(const std::vector<ci_string> &token){
    if (token.empty()){
        return true;
    } else if (token[0] == "controlpoint"){
		assert(token.size() > 4);
		assert(token[1] == "=");

		float x = static_cast<float>(atof(token[2].c_str()));
		float y = static_cast<float>(atof(token[3].c_str()));
		float z = static_cast<float>(atof(token[4].c_str()));
		AddControlPoint(glm::vec3(x, y, z));
		return true;
	} else {
        return Model::ParseLine(token);
    }
}
BSpline::~BSpline()
{
}

void BSpline::CreateVertexBuffer()
{
    if(mSamplePoints.size() == 0)
    {
        GenerateSamplePoints();
    }

	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mSamplePoints.size(), &mSamplePoints[0], GL_STATIC_DRAW);
}
void BSpline::GenerateSamplePoints()
{
    if(mControlPoints.size() == 0){
        return;
    }
    mSamplePoints.clear();

	const int numPointsPerSegment = 10;
	float increment = 1.0f / numPointsPerSegment;

	//foreach control point
	float t;
	float sampleLength;
	float l;
	glm::vec3 sample;
	for (unsigned int i=0; i < mControlPoints.size(); ++i){
        t = 0.0f;
        
        // Set current control points
        vec3 p1 = mControlPoints[i];
        vec3 p2 = mControlPoints[(i + 1) % mControlPoints.size()];
        vec3 p3 = mControlPoints[(i + 2) % mControlPoints.size()];
        vec3 p4 = mControlPoints[(i + 3) % mControlPoints.size()];
        
		//for each segment of curve generate sample
		sampleLength = 0.0f;
		int mSamplePointsSize;
        for (int j=0; j < numPointsPerSegment; ++j){
            //sample point
			sample = ComputePosition(t, p1, p2, p3, p4);
			mSamplePoints.push_back(sample);					//push sample to draw lines
			mSamplePointsSize = mSamplePoints.size();
			
			//sample lengths from points
			if(j>0) sampleLength += glm::distance(mSamplePoints[mSamplePointsSize-1],mSamplePoints[mSamplePointsSize-2]);
	
			t += increment; //sample next control sequence
        }
		mSampleLengths.push_back(sampleLength);//store length of curve
	}
	for(int k=0; k<mSampleLengths.size();k++){
	//	std::cout << "C" << mSampleLengths[k] <<"\n";
	}
	std::cout << "------------------\n";

}

int BSpline::GetControlPointSize(){
	return mControlPoints.size();
}

/*///////////////////////////////////////////////////////////////////
Listeners 
//*///////////////////////////////////////////////////////////////////
void BSpline::Update(float dt)
{
}
void BSpline::Draw(){

	if(1){
		// The Model View Projection transforms are computed in the Vertex Shader
		glBindVertexArray(mVertexArrayID);

		GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
		glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);

		// 1st attribute buffer : vertex Positions
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
		glVertexAttribPointer(0,
			3,				// size
			GL_FLOAT,		// type
			GL_FALSE,		// normalized?
			sizeof(Vertex), // stride
			(void*)0        // array buffer offset
			);

		// Draw the triangles !
		glDrawArrays(GL_LINE_LOOP, 0, mSamplePoints.size());

		glDisableVertexAttribArray(0);
	}
}



//*Controles///////////////////////////////////////////////////////////////////
void BSpline::AddControlPoint(glm::vec3 point)
{
	mControlPoints.push_back(point);
}
void BSpline::ClearControlPoints()
{
	mControlPoints.clear();
}



//*T Param ///////////////////////////////////////////////////////////////////
float BSpline::Travel(float t, float dist){
	float segmentLength = GetLengthOfSegment(t);
	//update t by fraction or skip segment
	return t += (segmentLength != 0.0f) ? dist/segmentLength : 1;
}
float BSpline::Travel(float t, float dt, float speed){
	float dist = (dt*speed);
	//----------------------------------------------------------------------------
	float inc;
	float segmentLength = GetLengthOfSegment(t);
	if(segmentLength != 0.0f){ 
		//travel along segment
		inc = dist/segmentLength;

		//*/If moving to new segment adjust segment length, important if waypoints vary by great distances apart
		if(floor(t) != floor(t+inc)){ //moving to new segment\
			//move to end of segment to start next
			t = floor(t+inc); 
			
			//determin dt for the new segment--------------------------------
			float diff = t+inc - floor(t+inc);
			float dt2 = (diff*segmentLength)/speed;

			//move distance on new segment
			segmentLength = GetLengthOfSegment(t);
			t += dt2*(speed/segmentLength);
			//---------------------------------------------------------------
		} else //*/// continue as usuail
			t += inc;
	} else
		t++; //skip segment
	return t;
}

float BSpline::GetLengthOfSegment(float t){
	int segment = ((int) t) % mControlPoints.size(); // get first point
	return  mSampleLengths[segment];
}
glm::vec3 BSpline::GetPosition(float t)
{
    // The value of t includes where on the spline you are.
    // The integral value of t is the first control point
    // The decimal value of t is the parameter t within the segment
    
	int p1 = ((int) t) % mControlPoints.size(); // get first point
	int p2 = (p1 + 1)  % mControlPoints.size();
	int p3 = (p2 + 1)  % mControlPoints.size();
	int p4 = (p3 + 1)  % mControlPoints.size();

	return vec3(GetWorldMatrix() * vec4(BSpline::ComputePosition(t - (int) t, mControlPoints[p1], mControlPoints[p2], mControlPoints[p3], mControlPoints[p4]), 1.0f));
}
glm::vec3 BSpline::GetTangent(float t)
{
    // The value of t includes where on the spline you are.
    // The integral value of t is the first control point
    // The decimal value of t is the parameter t within the segment
    
	int p1 = ((int) t) % mControlPoints.size();
	int p2 = (p1 + 1)  % mControlPoints.size();
	int p3 = (p2 + 1)  % mControlPoints.size();
	int p4 = (p3 + 1)  % mControlPoints.size();

	return vec3(GetWorldMatrix() * vec4(BSpline::ComputeTangent(t - (int) t, mControlPoints[p1], mControlPoints[p2], mControlPoints[p3], mControlPoints[p4]), 1.0f))-mPosition;
}
glm::vec3 BSpline::GetAcceleration(float t)
{
    // The value of t includes where on the spline you are.
    // The integral value of t is the first control point
    // The decimal value of t is the parameter t within the segment
    
	int p1 = ((int) t) % mControlPoints.size();
	int p2 = (p1 + 1)  % mControlPoints.size();
	int p3 = (p2 + 1)  % mControlPoints.size();
	int p4 = (p3 + 1)  % mControlPoints.size();

	return vec3(GetWorldMatrix() * vec4(BSpline::ComputeAcceleration(t - (int) t, mControlPoints[p1], mControlPoints[p2], mControlPoints[p3], mControlPoints[p4]), 1.0f))-mPosition;
}



//*Compute///////////////////////////////////////////////////////////////////
glm::vec3 BSpline::ComputePosition(float t, const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p4)
{
    // Returns the position of the object on the spline
    // based on parameter t and the four control points p1, p2, p3, p4
    vec4 params(t*t*t, t*t, t, 1);
    mat4 coefficients(
		vec4(-1, 3, -3, 1), 
		vec4(3, -6, 0, 4), 
		vec4(-3, 3, 3, 1), 
		vec4(1, 0, 0, 0)
	);
    vec4 product = (1.0f / 6.0f) * params * coefficients;
    return vec3(vec4(product.x * p1 + product.y * p2 + product.z * p3 + product.w * p4, 1.0f));
}
glm::vec3 BSpline::ComputeTangent(float t, const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p4)
{
    // Returns the Tangent of the object on the spline
    // based on parameter t and the four control points p1, p2, p3, p4
    vec4 params(3*t*t, 2*t, 1, 0);
    mat4 coefficients(
		vec4(-1, 3, -3, 1), 
		vec4(3, -6, 0, 4), 
		vec4(-3, 3, 3, 1), 
		vec4(1, 0, 0, 0)
	);
    vec4 product = (1.0f / 6.0f) * params * coefficients;
    return vec3(vec4(product.x * p1 + product.y * p2 + product.z * p3 + product.w * p4, 1.0f));
}
glm::vec3 BSpline::ComputeAcceleration(float t, const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p4)
{
    // Returns the Acceleration of the object on the spline
    // based on parameter t and the four control points p1, p2, p3, p4
    vec4 params(6*t, 2, 0, 0);
    mat4 coefficients(
		vec4(-1, 3, -3, 1), 
		vec4(3, -6, 0, 4), 
		vec4(-3, 3, 3, 1), 
		vec4(1, 0, 0, 0)
	);
    vec4 product = (1.0f / 6.0f) * params * coefficients;
    return vec3(vec4(product.x * p1 + product.y * p2 + product.z * p3 + product.w * p4, 1.0f));
}

