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
	
	collisionFlag.board = false;
	collisionFlag.lay = false;
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

void Object::hit(Object* object, CollosionType collisionType)
{
}

bool Object::getIsDead() { return isDead; }


Object::CollisionFlag Object::getCollisionFlag() 
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