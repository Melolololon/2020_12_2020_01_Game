#include "Enemy.h"
#include"Rubber.h"

Enemy::Enemy()
{
	Library::createManyVertex3DBox({ 2,2,2 }, &vertexHandle);
	Library::createHeapData2({ 64,255,64,255 }, 2, &heapHandle);
	Initialize();
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	//座標はとりあえず奥からランダムで。
	position = { (float)Library::getRandomNumber(100) - 50, 0, (float)Library::getRandomNumber(50) + 50 };
	//移動量はとりあえず手前方向へ。
	velocity = { 0, 0, -1.0f };
	//スピードはとりあえず2.0で。
	speed = 0.15f;
	//目標タイプはランダムで決める。途中の変更はない(今のところ)
	if (Library::getRandomNumber(2) == 0)
	{
		targetType = TargetType::LEFT;
	}
	else
	{
		targetType = TargetType::RIGHT;
	}

	myShot = false;

	collisionFlag.board = false;
	collisionFlag.lay = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = true;
	collisionFlag.plane = false;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 0.2f;

	hitRubber = false;
}

void Enemy::update()
{
	position = position + velocity * speed;
	Library::setPosition(position, heapHandle, 0);
	sphereData[0].position = position;
	updateVelocityTimer++;
}

void Enemy::draw()
{
	Library::drawGraphic(vertexHandle, heapHandle, 0);
}

void Enemy::UpdateVelocity(Vector3 playerPosition)
{
	if (myShot)return;

	if (!hitRubber) 
	{
		Vector3 v = velocity;
		velocity = v + (playerPosition - position);
		velocity = Vector3::normalize(velocity);
		for (int i = 0; i < 5; i++)
		{
			velocity = v + velocity;
			velocity = Vector3::normalize(velocity);
		}
	}

	hitRubber = false;
}

int Enemy::GetTargetTypeAsInt()
{
	if (targetType == TargetType::LEFT)
		return 0;

	return 1;
}

Enemy * Enemy::GetEnemy()
{
	Enemy* enemy = new Enemy;
	return enemy;
}

void* Enemy::getPtr()
{
	return this;
}

void Enemy::GetVelocityAndSpeed(Vector3& vel, Vector3& spe)
{
	vel = velocity;
	spe = speed;
}

void Enemy::AddPosition(const Vector3 vec)
{
	position += vec;
	Library::setPosition(position, heapHandle, 0);
	sphereData[0].position = position;
}

void Enemy::ShotEnemy(const Vector3 & vec)
{
	velocity = vec;
	myShot = true;

	sphereData[0].r = 1.0f;
}

bool Enemy::GetMyShot()
{
	return myShot;
}

void Enemy::hit(Object* object, CollosionType collisionType)
{
	if (typeid(*object) == typeid(Enemy))
	{
		Enemy* e = static_cast<Enemy*>(object->getPtr());
		if (e->GetMyShot())
		{
			isDead = true;
		}
	}

	if (typeid(*object) == typeid(Rubber)) 
	{
		hitRubber = true;
	}
}