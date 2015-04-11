//
// COMP 371 Project Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
// Hierarchy, Color and Transforms Added by Jordan Rutty
//
//

#include "Model.h"
#include "../Path.h"
#include "../BSpline.h"
#include "../World.h"
#include "..\Tools\ray.h"
#include "..\Tools\RayTriangleCollision.h"
//#include "StaticCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>


using namespace std;
using namespace glm;
//Construct Destruct ###################################################
Model::Model() : mName("UNNAMED"), mPosition(0.0f, 0.0f, 0.0f), mScaling(1.0f, 1.0f, 1.0f), mRotationAxis(0.0f, 1.0f, 0.0f), mRotationAngleInDegrees(0.0f), mPath(nullptr), mSpeed(0.0f), mTargetWaypoint(1), mSpline(nullptr), mSplineParameterT(0.0f), mCollisionCube(nullptr)
{
	mParent = nullptr;
	materialConst = vec4(0.39f, 0.46f, 0.82f, 75.0f);
	transform = mat4(1.0f);

	mRotationAngleX = 0;
	mRotationAngleY = 0;
	mRotationAngleZ = 0;
	mNthChild = 0;
}
Model::~Model()
{
	if (mCollisionCube != nullptr)
	{
		delete mCollisionCube;
	}
	DeleteAllChildren();
}

void Model::Load(ci_istringstream& iss){
	ci_string line;

	// Parse model line by line
	while(std::getline(iss, line)){
		// Splitting line into tokens
		ci_istringstream strstr(line);
		istream_iterator<ci_string, char, ci_char_traits> it(strstr);
		istream_iterator<ci_string, char, ci_char_traits> end;
		vector<ci_string> token(it, end);

		if (ParseLine(token) == false){
			fprintf(stderr, "Error loading scene file... token:  %s!", token[0]);
			getchar();
			exit(-1);
		}
	}
}//Load from file
bool Model::ParseLine(const std::vector<ci_string> &token){
	if (token.empty() == false){
		if (token[0].empty() == false && token[0][0] == '#'){
			return true;
		} else if (token[0] == "name"){
			assert(token.size() > 2);
			assert(token[1] == "=");

			mName = token[2];	
		} else if (token[0] == "position"){
			assert(token.size() > 4);
			assert(token[1] == "=");
			SetPosition(vec3(
				static_cast<float>(atof(token[2].c_str())),
				static_cast<float>(atof(token[3].c_str())),
				static_cast<float>(atof(token[4].c_str()))
			));
		} else if (token[0] == "rotation"){
			assert(token.size() > 4);
			assert(token[1] == "=");

			SetRotation(vec3(
				static_cast<float>(atof(token[2].c_str())),
				static_cast<float>(atof(token[3].c_str())),
				static_cast<float>(atof(token[4].c_str()))),
				static_cast<float>(atof(token[5].c_str()))  //angle
			);

			glm::normalize(mRotationAxis);
		} else if (token[0] == "scaling"){
			assert(token.size() > 4);
			assert(token[1] == "=");
			SetScaling(vec3(
				static_cast<float>(atof(token[2].c_str())),
				static_cast<float>(atof(token[3].c_str())),
				static_cast<float>(atof(token[4].c_str()))
			));
		} else if (token[0] == "pathspeed"){
			assert(token.size() > 2);
			assert(token[1] == "=");

            float speed = static_cast<float>(atof(token[2].c_str()));
            SetSpeed(speed);
		} else if (token[0] == "boundpath"){
			assert(token.size() > 2);
			assert(token[1] == "=");

			ci_string pathName = token[2];
            World* w = World::GetInstance();
            mPath = w->FindPath(pathName);

			if (mPath == nullptr){
				SetSpline(w->FindSpline(pathName));
			}
			
			if (mPath != nullptr){
				mPosition = mPath->GetWaypoint(0);
			} else if (mSpline){
				mPosition = mSpline->GetPosition(mSplineParameterT);
			}
		} else {
			return false;
		}
	}

	return true;
}//parse line of text 
glm::mat4 Model::GetWorldMatrix(){
	mat4 worldMatrix(1.0f);

	mat4 t = glm::translate(mat4(1.0f), mPosition);
	mat4 r = glm::rotate(mat4(1.0f), mRotationAngleInDegrees, mRotationAxis);
	mat4 rx = glm::rotate(mat4(1.0f), mRotationAngleX, vec3(1,0,0));
	mat4 ry = glm::rotate(mat4(1.0f), mRotationAngleY, vec3(0,1,0));
	mat4 rz = glm::rotate(mat4(1.0f), mRotationAngleZ, vec3(0,0,1));

	mat4 s = glm::scale(mat4(1.0f), mScaling);
	worldMatrix = t * (r * rz * ry * rx)* s * transform;
	
	if(HasParent()){//relative to parent
		return Parent()->GetWorldMatrix() * worldMatrix;
	} else //relative to world
		return worldMatrix;
}
glm::mat4	Model::GetRecursiveScalingMatrix(){
	if(HasParent()){
		return glm::scale(Parent()->GetRecursiveScalingMatrix(), mScaling);
	} else {
		return glm::scale(mat4(1.0f), mScaling);
	}
}
void Model::Update(float dt){

	if(mPath != nullptr)		
		updatePath(dt);		//Move along Path
	else if (mSpline)		
		updateSpline(dt);	//Move along Spline

	UpdateChildren(dt);
	//updateChildren(Tree(list, values), dt) 
}
void Model::updatePath(float dt){
	//Translate Along Path
	//==============================================================================
	bool makeMove = true;
	glm::vec3 vTargetWaypoint	= mPath->GetWaypoint(mTargetWaypoint);						//current target waypoint
		
	//Compair distance with distance to go
	float step = mSpeed * dt;
	float distToGo = glm::distance(mPosition, vTargetWaypoint);	
	if (distToGo < step){																
		mPosition = vTargetWaypoint;														//set position to waypoint
		dt =- distToGo/mSpeed;																//recalculate dt with new position
			
		//are there any more waypoints? continue on the way to the next waypoint
		mTargetWaypoint++;																	//change target
		mTargetWaypoint %= mPath->GetWaypointSize();										//prevent overflow

		vTargetWaypoint	= mPath->GetWaypoint(mTargetWaypoint);								//current target waypoint
		// else makeMove = false;
	}

	if(makeMove){
		glm::vec3 moveDirection		= glm::normalize(vTargetWaypoint - mPosition);			//step direction toward waypoint, from model
		step = mSpeed * dt;	
		mPosition += step*moveDirection;													//step toward direction
	}
}
void Model::updateSpline(float dt){
	//Translate Along Spline	 @TODO - Animate along the spline
		//==============================================================================
		mSplineParameterT = mSpline->Travel(mSplineParameterT, dt*mSpeed);
		
		//every lap reset T-param
		int cPSize = mSpline->GetControlPointSize();
		if(floor(mSplineParameterT) == cPSize) 
			mSplineParameterT -= cPSize;
		
		mPosition = mSpline->GetPosition(mSplineParameterT);
}
void Model::Draw(){}


//Orientation ###################################################
void Model::SetPosition(glm::vec3 position){
	mPosition = position;
}
void Model::SetScaling(glm::vec3 scaling){
	mScaling = scaling;
}
void Model::SetRotation(glm::vec3 axis, float angleDegrees){
	mRotationAxis = axis;
	mRotationAngleInDegrees = angleDegrees;
}

//Hierarchy #####################################################
Model*	Model::Parent(){
	return mParent;
}
bool	Model::HasParent(){
	if (mParent != nullptr)
		return true;
	return false;
}
void	Model::SetParent(Model* m){
	this->mParent = m;
}
bool	Model::HasChild(ci_string key){
	return child.count(key) > 0;
}
void	Model::AddChild(Model* m){
	m->SetParent(this);
	std::string str = to_string(mNthChild++);
	m->SetName(str.c_str());
	child[str.c_str()] = m;
}	
void	Model::AddChild(ci_string key, Model* m){
	m->SetName(key);
	m->SetParent(this);
	child[key] = m;
	mNthChild++;
}	
Model*	Model::GetChild(ci_string key){ 
	return HasChild(key) ? child[key] : nullptr;	
}
void	Model::DeleteChild(ci_string key){ //delete child competly
	Model* old = child[key];	
	delete old;
	child.erase(key);			
}
Model*	Model::RemoveChild(ci_string key){ //Remove from Hierarchy
	Model* old = child[key];	//keep track of pointer
	child.erase(key);			//remove
	return old;					//return
}
void	Model::DeleteAllChildren(){
	std::map<ci_string, Model*>::iterator it;
	if(!child.empty()){
		for (it = child.begin(); it != child.end(); it++){
			Model* item = (*it).second;
			if(item != nullptr)
			delete item;
		}
		child.clear();
	}
}
void	Model::UpdateChildren(float dt){
	if (GetChildCount() > 0){
		typedef std::map<ci_string, Model*>::iterator it_type;
		for(it_type iterator = child.begin(); iterator != child.end(); iterator++) {
			iterator->second->Update(dt);
		}
	}
}
void	Model::DrawChildren(){
	if (GetChildCount() > 0){
		typedef std::map<ci_string, Model*>::iterator it_type;
		for(it_type iterator = child.begin(); iterator != child.end(); iterator++) {
			iterator->second->Draw();
		}
	}
}


//Collision ######################################################
void Model::CreateDefaultCollisionCube()
{
	if (mCollisionCube == nullptr)
	{
		mCollisionCube = new vec3(1.0f);
	}
}

void Model::ReScaleCollisionCube(vec3 newScale) // (1.0f, 1.0f, 1.0f) is normal (default)
{
	if (mCollisionCube != nullptr)
	{
		mCollisionCube->x *= newScale.x;
		mCollisionCube->y *= newScale.y; // should have no effect
		mCollisionCube->z *= newScale.z;
	}
}

bool Model::CollideWith(Model* other)
{
	if (this->mCollisionCube == nullptr || other == this || other->mCollisionCube == nullptr)
	{
		return false;
	}
	
	//vec3 thisWorldPosition = vec3(GetWorldMatrix() * vec4(mPosition, 1.0f));
	//vec3 otherWorldPosition = vec3(other->GetWorldMatrix() * vec4(other->mPosition, 1.0f));
	vec3 thisWorldPosition = this->mPosition;
	vec3 otherWorldPosition = other->mPosition;
	
	float Axa = thisWorldPosition.x - this->mCollisionCube->x * this->mScaling.x / 2.0f;
	float Axb = thisWorldPosition.x + this->mCollisionCube->x * this->mScaling.x / 2.0f;
	float Aya = thisWorldPosition.y - this->mCollisionCube->y * this->mScaling.y / 2.0f;
	float Ayb = thisWorldPosition.y + this->mCollisionCube->y * this->mScaling.y / 2.0f;
	float Aza = thisWorldPosition.z - this->mCollisionCube->z * this->mScaling.z / 2.0f;
	float Azb = thisWorldPosition.z + this->mCollisionCube->z * this->mScaling.z / 2.0f;
	float Bxa = otherWorldPosition.x - other->mCollisionCube->x * other->mScaling.x / 2.0f;
	float Bxb = otherWorldPosition.x + other->mCollisionCube->x * other->mScaling.x / 2.0f;
	float Bya = otherWorldPosition.y - other->mCollisionCube->y * other->mScaling.y / 2.0f;
	float Byb = otherWorldPosition.y + other->mCollisionCube->y * other->mScaling.y / 2.0f;
	float Bza = otherWorldPosition.z - other->mCollisionCube->z * other->mScaling.z / 2.0f;
	float Bzb = otherWorldPosition.z + other->mCollisionCube->z * other->mScaling.z / 2.0f;

	float xOffset = 0.0f;
	float yOffset = 0.0f;
	float zOffset = 0.0f;

	bool collided = false;
	
	if (Axa >= Bxa && Axa <= Bxb && Aya >= Bya && Aya <= Byb && Aza >= Bza && Aza <= Bzb)
	{
		xOffset = Bxb - Axa;
		yOffset = Byb - Aya;
		zOffset = Bzb - Aza;
		collided = true;
	}
	else if (Axa >= Bxa && Axa <= Bxb && Ayb >= Bya && Ayb <= Byb && Aza >= Bza && Aza <= Bzb)
	{
		xOffset = Bxb - Axa;
		yOffset = Bya - Ayb;
		zOffset = Bzb - Aza;
		collided = true;
	}
	else if (Axb >= Bxa && Axb <= Bxb && Ayb >= Bya && Ayb <= Byb && Aza >= Bza && Aza <= Bzb)
	{
		xOffset = Bxa - Axb;
		yOffset = Bya - Ayb;
		zOffset = Bzb - Aza;
		collided = true;
	}
	else if (Axb >= Bxa && Axb <= Bxb && Aya >= Bya && Aya <= Byb && Aza >= Bza && Aza <= Bzb)
	{
		xOffset = Bxa - Axb;
		yOffset = Byb - Aya;
		zOffset = Bzb - Aza;
		collided = true;
	}
	else if (Axa >= Bxa && Axa <= Bxb && Aya >= Bya && Aya <= Byb && Azb >= Bza && Azb <= Bzb)
	{
		xOffset = Bxb - Axa;
		yOffset = Byb - Aya;
		zOffset = Bza - Azb;
		collided = true;
	}
	else if (Axa >= Bxa && Axa <= Bxb && Ayb >= Bya && Ayb <= Byb && Azb >= Bza && Azb <= Bzb)
	{
		xOffset = Bxb - Axa;
		yOffset = Bya - Ayb;
		zOffset = Bza - Azb;
		collided = true;
	}
	else if (Axb >= Bxa && Axb <= Bxb && Ayb >= Bya && Ayb <= Byb && Azb >= Bza && Azb <= Bzb)
	{
		xOffset = Bxa - Axb;
		yOffset = Bya - Ayb;
		zOffset = Bza - Azb;
		collided = true;
	}
	else if (Axb >= Bxa && Axb <= Bxb && Aya >= Bya && Aya <= Byb && Azb >= Bza && Azb <= Bzb)
	{
		xOffset = Bxa - Axb;
		yOffset = Byb - Aya;
		zOffset = Bza - Azb;
		collided = true;
	}

	if (abs(xOffset) < abs(yOffset) && abs(xOffset) < abs(zOffset))
	{
		mPosition.x += xOffset;
	}
	else if (abs(yOffset) < abs(zOffset))
	{
		mPosition.y += yOffset;
	}
	else
	{
		mPosition.z += zOffset;
	}

	return collided;
}

void Model::Intersect(const Ray& ray, std::vector<std::pair<Model*, glm::vec3>>& intersectionPoints)
{
	if (this->mCollisionCube == nullptr)
	{
		return;
	}

	//vec3 worldPosition = vec3(GetWorldMatrix() * vec4(mPosition, 1.0f));
	vec3 worldPosition = this->mPosition;

	float xa = worldPosition.x - this->mCollisionCube->x * this->mScaling.x / 2.0f;
	float xb = worldPosition.x + this->mCollisionCube->x * this->mScaling.x / 2.0f;
	float ya = worldPosition.y - this->mCollisionCube->y * this->mScaling.y / 2.0f;
	float yb = worldPosition.y + this->mCollisionCube->y * this->mScaling.y / 2.0f;
	float za = worldPosition.z - this->mCollisionCube->z * this->mScaling.z / 2.0f;
	float zb = worldPosition.z + this->mCollisionCube->z * this->mScaling.z / 2.0f;

	vec3 intersection;
	
	if (RayTriangleCollision::TestIntersectionPlane(vec3(xa, yb, zb), vec3(xa, yb, za), vec3(xb, yb, za), ray, intersection, true))
	{
		intersectionPoints.push_back(std::pair<Model*, glm::vec3>(this, intersection));
	}
	if (RayTriangleCollision::TestIntersectionPlane(vec3(xa, yb, zb), vec3(xb, yb, zb), vec3(xb, yb, za), ray, intersection, true))
	{
		intersectionPoints.push_back(std::pair<Model*, glm::vec3>(this, intersection));
	}
	
	if (RayTriangleCollision::TestIntersectionPlane(vec3(xa, ya, zb), vec3(xa, ya, za), vec3(xb, ya, za), ray, intersection, true))
	{
		intersectionPoints.push_back(std::pair<Model*, glm::vec3>(this, intersection));
	}
	if (RayTriangleCollision::TestIntersectionPlane(vec3(xa, ya, zb), vec3(xb, ya, zb), vec3(xb, ya, za), ray, intersection, true))
	{
		intersectionPoints.push_back(std::pair<Model*, glm::vec3>(this, intersection));
	}
	
	if (RayTriangleCollision::TestIntersectionPlane(vec3(xb, yb, zb), vec3(xb, yb, za), vec3(xb, ya, za), ray, intersection, true))
	{
		intersectionPoints.push_back(std::pair<Model*, glm::vec3>(this, intersection));
	}
	if (RayTriangleCollision::TestIntersectionPlane(vec3(xb, yb, zb), vec3(xb, ya, zb), vec3(xb, ya, za), ray, intersection, true))
	{
		intersectionPoints.push_back(std::pair<Model*, glm::vec3>(this, intersection));
	}
	
	if (RayTriangleCollision::TestIntersectionPlane(vec3(xa, ya, za), vec3(xa, yb, za), vec3(xb, yb, za), ray, intersection, true))
	{
		intersectionPoints.push_back(std::pair<Model*, glm::vec3>(this, intersection));
	}
	if (RayTriangleCollision::TestIntersectionPlane(vec3(xa, ya, za), vec3(xb, ya, za), vec3(xb, yb, za), ray, intersection, true))
	{
		intersectionPoints.push_back(std::pair<Model*, glm::vec3>(this, intersection));
	}
}

//Physics #########################################################
void	Model::SetSplineParameterT(float t){
	mSplineParameterT = t;
}
void	Model::SetSpline(BSpline* sp){
	mSpline = sp;
}

BSpline*	Model::GetSpline(){
	return mSpline;
}
void		Model::SetSpeed(float spd){
    mSpeed = spd;
}
float		Model::GetSpeed(){return mSpeed;}
//force
//acceleration

