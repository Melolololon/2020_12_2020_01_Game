#include "ParentEnemy.h"

ParentEnemy::ParentEnemy()
{
	Library::createManyVertex3DBox({ 6,6,6 }, &vertexHandle);
	Library::createHeapData2({ 255,64,255,255 }, 3, &heapHandle);
	Initialize();
}

ParentEnemy::~ParentEnemy()
{
}

void ParentEnemy::Initialize()
{

	position = {0,0,10 };
	velocity = { 0, 0, 0 };
	speed = 0.0f;

	generateEnemyTimer = 0;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 3.0f;

}

void ParentEnemy::update()
{
	position = position + velocity * speed;
	Library::setPosition(position, heapHandle, 0);
	sphereData[0].position = position;
}

void ParentEnemy::draw()
{
	Library::drawGraphic(vertexHandle, heapHandle, 0);
}

void ParentEnemy::hit(Object * object, CollosionType collisionType)
{
	//吹っ飛んでるときでも敵とぶつかったらダメージ
	if (typeid(*object) == typeid(Enemy))
	{
		//ここに敵と衝突したときのコードを書く
	}

}

void * ParentEnemy::getPtr()
{
	return this;
}

void ParentEnemy::ShotEnemy()
{

}
