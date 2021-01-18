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
	//���W�͂Ƃ肠���������烉���_���ŁB
	position = { (float)Library::getRandomNumber(100) - 50, 0, (float)Library::getRandomNumber(50) + 50 };
	//�ړ��ʂ͂Ƃ肠������O�����ցB
	velocity = { 0, 0, -1.0f };
	//�X�s�[�h�͂Ƃ肠����2.0�ŁB
	speed = 0.2f;
	//�ڕW�^�C�v�̓����_���Ō��߂�B�r���̕ύX�͂Ȃ�(���̂Ƃ���)
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
