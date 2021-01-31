#include "DamageObject.h"

DamageObject::DamageObject(const Vector3& pos, const Vector3& vel, const Vector3& spe, const DamageObjectType& type)
{
	position = pos;
	velocity = vel;
	speed = spe;
	isDead = false;

	objectType = type;

	sphereData.resize(1);
	sphereData[0].position = position;

	collisionFlag.board = false;
	collisionFlag.ray = false;
	collisionFlag.plane = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = true;
	switch (type)
	{
	case DAMAGE_OBJECT_SYOGEKIHA:
		sphereData[0].r = 1.0f;
		break;
	}

	Library::createManyVertex3DBox({ 2,2,2 }, &vertexHandle);
	Library::createHeapData2({ 255,64,255,255 }, 1, &heapHandle);
	Library::setPosition(position, heapHandle, 0);
}

DamageObject::~DamageObject(){}

void DamageObject::update() 
{
	position = position + velocity * speed;
	Library::setPosition(position, heapHandle, 0);
	sphereData[0].position = position;
}
//•`‰æ
void DamageObject::draw() 
{
	Library::drawGraphic(vertexHandle, heapHandle, 0);
}

void DamageObject::hit(Object* object, CollisionType collisionType)
{

}

void* DamageObject::getPtr()
{
	return this;
}