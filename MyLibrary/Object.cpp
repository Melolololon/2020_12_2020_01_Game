#include "Object.h"



Object::Object()
{
	position = 0;
	velocity = 0;
	speed = 0;
	isDead = false;
	heapNumber = 0;

	/*sphereData.position = 0.0f;
	sphereData.r = 0.0f;
	lineSegmentData.position[0] = 0.0f;
	lineSegmentData.position[1] = 1.0f;*/
	
	collisionFlag.plane = false;
	collisionFlag.ray = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = false;
}


Object::~Object()
{
}

void Object::update() 
{

}

void Object::draw() 
{

}

void Object::hit(Object* object, CollisionType collisionType)
{
}

bool Object::getIsDead() { return isDead; }


CollisionFlag Object::getCollisionFlag() 
{
	return collisionFlag;
}

std::vector<SphereData> Object::getSphereData()
{
	return sphereData;
}

void* Object::getPtr() 
{
	return this;
}

std::vector<LineSegmentData> Object::getLineSegmentData()
{
	return lineSegmentData;
}

std::vector<PlaneData> Object::getPlaneData()
{
	return planeData;
}

std::vector<BoardData> Object::getBoardData()
{
	return boardData;
}


void Object::setLineSegmentDataHitPos(const Vector3& pos, const int& arrayNum)
{
	lineSegmentData[arrayNum].hitPos = pos;
}

void Object::setBoardDataHitPos(const Vector3& pos, const int& arrayNum)
{
	boardData[arrayNum].hitPos = pos;
}
