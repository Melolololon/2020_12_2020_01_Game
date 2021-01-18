#include "Enemy.h"

Enemy::Enemy()
{
	Library::createManyVertex3DBox({ 2,2,2 }, &vertexHandle);
	Library::createHeapData2({ 64,64,64,255 }, 2, &heapHandle);
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
	speed = 0.2f;
	//目標タイプはランダムで決める。途中の変更はない(今のところ)
	if (Library::getRandomNumber(2) == 0)
	{
		targetType = TargetType::LEFT;
	}
	else
	{
		targetType = TargetType::RIGHT;
	}

}

void Enemy::update()
{
	position = position + velocity * speed;
	Library::setPosition(position, heapHandle, 0);
	updateVelocityTimer++;
}

void Enemy::draw()
{
	Library::drawGraphic(vertexHandle, heapHandle, 0);
}

void Enemy::UpdateVelocity(Vector3 playerPosition)
{
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
