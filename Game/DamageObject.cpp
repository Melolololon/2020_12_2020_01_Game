#include "DamageObject.h"
#include"PolygonManager.h"
#include"LibMath.h"

const int DamageObject::CreateNum = 5;
int DamageObject::createCount; 

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
		sphereData[0].r = 1.5f;
		break;
	}

	vertexHandle = PolygonManager::getInstance()->getPolygonVertex("syogekiha");
	heapHandle = PolygonManager::getInstance()->getPolygonVertex("syogekiha");
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CreateNum ? 0 : createCount;
	Library::setPosition(position, heapHandle, heapNum);
	Library::setScale({1.5,1.5,1.5}, heapHandle, heapNum);
}

DamageObject::~DamageObject(){}


void DamageObject::loadModel()
{
	vertex v;
	heap h;
	Library::loadOBJVertex("Resources/Obj/syogekiha.obj", false, true, nullptr, &v);
	Library::createHeapData2({ 255,255,50,255 }, CreateNum, &h);
	PolygonManager::getInstance()->addPolygonVertex("syogekiha", v);
	PolygonManager::getInstance()->addPolygonHeap("syogekiha", h);
}

void DamageObject::update() 
{
	position = position + velocity * speed;
	Library::setPosition(position, heapHandle, heapNum);
	sphereData[0].position = position;

#pragma region ‰ñ“]ˆ—
	float angle = LibMath::vecto2rToAngle({ velocity.x,velocity.z });
	Library::setAngle({ 0,-angle - 90,0 }, heapHandle, heapNum);
#pragma endregion


	if (position.x >= 60 ||
		position.x <= -60 ||
		position.z >= 60 ||
		position.z <= -60)isDead = true;
}
//•`‰æ
void DamageObject::draw() 
{
	Library::setPipeline(PIPELINE_NOT_SHADOW);
	Library::drawGraphic(vertexHandle, heapHandle, heapNum);
	Library::setPipeline(PIPELINE_NORMAL);
}

void DamageObject::hit(Object* object, CollisionType collisionType)
{

}

void* DamageObject::getPtr()
{
	return this;
}



