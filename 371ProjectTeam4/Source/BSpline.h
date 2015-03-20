//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 24/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "ParsingHelper.h"
#include "Models/Model.h"
#include <vector>

#include <GLM/glm.hpp>


class BSpline : public Model
{
public:
    
	BSpline();
    virtual ~BSpline();

	void CreateVertexBuffer();

    virtual void Update(float dt);
    virtual void Draw();

    void AddControlPoint(glm::vec3 waypoint);
    void ClearControlPoints();
	int GetControlPointSize();

	// t from 0 to 1 is the first segment, from 1 to 2 is the second, etc...
	float Travel(float T, float dt, float speed);
	float Travel(float T, float dist);

	float GetLengthOfSegment(float t); //return the sample length of the curve
	glm::vec3 GetPosition(float t);
	glm::vec3 GetTangent(float t);			// @TODO
	glm::vec3 GetAcceleration(float t);


	// this is the more general BSpline equation, 4 control points, 0 <= t <= 1
	static glm::vec3 ComputePosition(float t, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4);
	static glm::vec3 ComputeTangent(float t, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4);
	static glm::vec3 ComputeAcceleration(float t, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4);
    
    
protected:
    virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	// The vertex format could be different for different types of models
	struct Vertex
	{
		glm::vec3 position;
	};

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;

    // Control points used by the B-Spline equation
    std::vector<glm::vec3> mControlPoints;

	

    // Sample points along the curve, used for drawing
    std::vector<glm::vec3> mSamplePoints;
	std::vector<unsigned long> mSampleLengths;			//very important these are "long" for divisions!!
    void GenerateSamplePoints();
};
