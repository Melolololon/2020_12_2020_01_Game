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
	//���W�͂Ƃ肠���������烉���_���ŁB
	position = { (float)Library::getRandomNumber(100) - 50, 0, (float)Library::getRandomNumber(50) + 50 };
	//�ړ��ʂ͂Ƃ肠������O�����ցB
	velocity = { 0, 0, -1.0f };
	//�X�s�[�h�͂Ƃ肠����2.0�ŁB
	speed = 0.15f;
	//�ڕW�^�C�v�̓����_���Ō��߂�B�r���̕ύX�͂Ȃ�(���̂Ƃ���)
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
	sphereData[0].r = 1.0f;

	//���C�t
	life = 20;

	//���G����
	isMuteki = false;
	mutekiTimer = 0;
}

void Enemy::update()
{
	position = position + velocity * speed;
	Library::setPosition(position, heapHandle, 0);
	sphereData[0].position = position;
	updateVelocityTimer++;

	//���G����
	if (isMuteki)mutekiTimer++;
	if (mutekiTimer >= MutekiTime) 
	{
		mutekiTimer = 0;
		isMuteki = false;
	}
}

void Enemy::draw()
{
	Library::drawGraphic(vertexHandle, heapHandle, 0);
}

void Enemy::UpdateVelocity(Vector3 playerPosition)
{
	//���x��������ēx�ǔ�
	if (myShot) 
	{
		speed -= {0.025, 0, 0.025};
		if (speed.x <= 0.0f &&
			speed.z <= 0.0f) 
		{
			myShot = false;
			speed = 0.15f;
		}

		return;
	}

	float pAndEDistance = LibMath::calcDistance3D(playerPosition, position);
	if (pAndEDistance >= 10.0f) 
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

void Enemy::ShotEnemy(const Vector3 & vel, const Vector3& spe)
{
	velocity = vel;
	speed = spe;
	myShot = true;

}

bool Enemy::GetMyShot()
{
	return myShot;
}

void Enemy::hit(Object* object, CollosionType collisionType)
{


	if (typeid(*object) == typeid(Rubber)) 
	{
	}

	if (isMuteki)return;

	//�������ł�Ƃ��ł��G�ƂԂ�������_���[�W
	if (typeid(*object) == typeid(Enemy))
	{
		Enemy* e = static_cast<Enemy*>(object->getPtr());
		if (e->GetMyShot())
		{
			life -= e->GetDamage();
			isMuteki = true;
		}
	}
}

int Enemy::GetDamage()
{
	if (myShot)
	{
		float damageNum = speed.x * 10;
		return (int)damageNum;
	}

	return 0;
}
