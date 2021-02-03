#include "DamageNumber.h"
#include"PipelineManager.h"

DamageNumber::DamageNumber(const Vector3& pos, const int& damageNum)
{
	position = pos;
	velocity = { 0,1,0 };
	speed = { 0,0.05f,0 };
	isDead = false;

	collisionFlag.board = false;
	collisionFlag.plane = false;
	collisionFlag.sphere = false;
	collisionFlag.lineSegment = false;
	collisionFlag.ray = false;

	Library::createBoard({ 1,2 }, dimention3D, &vertexHandle);
	Library::createHeapData(L"Resources/Texture/damageNum02.png", 1, &heapHandle);
	//Library::setAngle({ 0,180,0 }, heapHandle, 0);
	Library::setPosition(position, heapHandle, 0);

	pl = PipelineManager::getInstance()->getPipeline("billboardPL");
	damage = damageNum;

	deadTimer = 0;
}

DamageNumber::~DamageNumber()
{
	Library::deleteVertexData(vertexHandle);
	Library::deleteHeapData(heapHandle);
}

void DamageNumber::update()
{
	position += velocity * speed;
	Library::setPosition(position, heapHandle, 0);

	deadTimer++;
	if (deadTimer >= DeadTime)isDead = true;
}

void DamageNumber::draw()
{
	Library::setPipeline(pl);
	Library::setIsBillboard(true, true, true);
	Library::changeAnimation(vertexHandle, 10, 1, damage + 1, 1);
	Library::drawGraphic(vertexHandle, heapHandle, 0);
	Library::setIsBillboard(false, false, false);
	Library::setPipeline(PIPELINE_NORMAL);

}
