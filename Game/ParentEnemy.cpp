#include "ParentEnemy.h"
#include"Enemy.h"
#include"ObjectManager.h"
#include"DamageNumber.h"

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

	position = {0,0,0 };
	velocity = { 0, 0, 0 };
	speed = 0.0f;

	generateEnemyTimer = 0;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 3.0f;

	//仮()
	life = 7;

	isMuteki = false;
	mutekiTimer = 0;
	generateEnemyTimer = 0;
}

void ParentEnemy::update()
{
	position = position + velocity * speed;
	Library::setPosition(position, heapHandle, 0);
	sphereData[0].position = position;

	ShotEnemy();
}

void ParentEnemy::draw()
{
	Library::drawGraphic(vertexHandle, heapHandle, 0);
}

void ParentEnemy::hit(Object * object, CollosionType collisionType)
{

	if (isMuteki)return;
	//吹っ飛んでるときでも敵とぶつかったらダメージ
	if (typeid(*object) == typeid(Enemy))
	{
		//ここに敵と衝突したときのコードを書く
		Enemy* e = static_cast<Enemy*>(object->getPtr());
		if (e->GetMyShot())
		{
			int damage = e->GetDamage();

			life -= damage;
			isMuteki = true;

			if (damage != 0)
				ObjectManager::getInstance()->addObject(new DamageNumber(position, damage));
		}

	}

}

void * ParentEnemy::getPtr()
{
	return this;
}

void ParentEnemy::ShotEnemy()
{
	generateEnemyTimer++;

	if (generateEnemyTimer >= GENERATETIME) 
	{
		generateEnemyTimer = 0;
		ObjectManager::getInstance()->addObject(new Enemy(position, Enemy::PLAYER_TARGET));
	}
}
