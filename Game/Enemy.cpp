#include "Enemy.h"

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
	speed = 0.1f;
	//目標タイプはランダムで決める。途中の変更はない(今のところ)
	if (Library::getRandomNumber(2) == 0)
	{
		targetType = TargetType::LEFT;
	}
	else
	{
		targetType = TargetType::RIGHT;
	}

	stopFlag = false;
	myShot = false;

	collisionFlag.board = false;
	collisionFlag.lay = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = true;
	collisionFlag.plane = false;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 1.0f;

}

void Enemy::update()
{
	if (stopFlag)return;
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
	velocity = Vector3::straight(position, playerPosition);
	velocity = Vector3::normalize(velocity);
	updateVelocityTimer = 0;

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


void Enemy::SetStopFlag(const bool& flag)
{
	stopFlag = flag;
}

void Enemy::SetVelocity(const Vector3& vec)
{
	velocity = vec;
	myShot = true;
}

bool Enemy::GetMyShot()
{
	return myShot;
}
