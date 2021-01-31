#include "Enemy.h"
#include"Rubber.h"
#include"DamageNumber.h"

#include"Player.h"

#include"ObjectManager.h"

Vector3 Enemy::playerPosition[2];

const int Enemy::TuibiEndTime = 60 * 10;

Enemy::Enemy(const Vector3& pos, const EnemyType& enemyType)
{
	Library::createManyVertex3DBox({ 2,2,2 }, &vertexHandle);
	Library::createHeapData2({ 64,255,64,255 }, 1, &heapHandle);

	position = pos;
	Library::setPosition(position, heapHandle, 0);
	this->enemyType = enemyType;

	Initialize();

	
}

Enemy::Enemy(const Vector3& pos, const Vector3& vel)
{
	Library::createManyVertex3DBox({ 2,2,2 }, &vertexHandle);
	Library::createHeapData2({ 64,255,64,255 }, 1, &heapHandle);

	Initialize();
	position = pos;
	velocity = vel;
	Library::setPosition(position, heapHandle, 0);
	enemyType = SET_VELOCITY;
	tuibiEndTimer = 0;
}

Enemy::~Enemy()
{
	Library::deleteVertexData(vertexHandle);
	Library::deleteHeapData(heapHandle);
}

void Enemy::loadModelData()
{

}

void Enemy::Initialize()
{
	//���W�͂Ƃ肠���������烉���_���ŁB
	//position = { 0, 0, 10 };
	//�ړ��ʂ͂Ƃ肠������O�����ցB
	velocity = { 0, 0, -1.0f };

	//�X�s�[�h�͂Ƃ肠����2.0�ŁB
	speed = 0.15f;

	myShot = false;

	collisionFlag.board = false;
	collisionFlag.ray = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = true;
	collisionFlag.plane = false;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 1.0f;

	//���C�t
	life = 3;

	//���G����
	isMuteki = false;
	mutekiTimer = 0;

	float dir[2];
	dir[0] = LibMath::calcDistance3D(playerPosition[0], position);
	dir[1] = LibMath::calcDistance3D(playerPosition[1], position);
	if (dir[0] < dir[1]) 
	targetType = TargetType::LEFT;
	else
		targetType = TargetType::RIGHT;

	if (enemyType == EnemyType::PLAYER_TARGET) 
	{
		velocity = LibMath::otherVector(position, playerPosition[targetType]);
	}

}

void Enemy::update()
{
	UpdateVelocity();
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

	if (life <= 0 && !myShot)isDead = true;

	if (position.x >= 60 ||
		position.x <= -60 ||
		position.z >= 60 ||
		position.z <= -60)isDead = true;
}

void Enemy::draw()
{
	Library::drawGraphic(vertexHandle, heapHandle, 0);
}

void Enemy::UpdateVelocity()
{
	//���x��������ēx�ǔ�(�ǔ����Ȃ��z�͂Ƃ肠�������̂܂ܒ��i)
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

	if (enemyType != EnemyType::PLAYER_TUIBI || 
		updateVelocityTimer < UPDATE_VELOCITY_TIME )return;

	tuibiEndTimer++;
	if (tuibiEndTimer >= TuibiEndTime)return;

	//updateVelocityTimer = 0;
	float pAndEDistance = LibMath::calcDistance3D(playerPosition[GetTargetTypeAsInt()], position);
	if (pAndEDistance >= 12.0f) 
	{
		Vector3 v = velocity;
		velocity = v + (playerPosition[GetTargetTypeAsInt()] - position);
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

//Enemy * Enemy::GetEnemy()
//{
//	Enemy* enemy = new Enemy();
//	return enemy;
//}

void* Enemy::getPtr()
{
	return this;
}

void Enemy::GetVelocityAndSpeed(Vector3& vel, Vector3& spe)
{
	vel = velocity;
	spe = speed;
}


void Enemy::SetPosition(const Vector3& pos)
{
	position = pos;
	Library::setPosition(position, heapHandle, 0);
	sphereData[0].position = position;
}

void Enemy::AddPosition(const Vector3& vec)
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

void Enemy::hit(Object* object, CollisionType collisionType)
{
	if (typeid(*object) == typeid(Player))isDead = true;

	if (typeid(*object) == typeid(Rubber)) 
	{
	}

	if (isMuteki)return;

	//�������ł�Ƃ��ł��G�ƂԂ�������_���[�W
	if (typeid(*object) == typeid(Enemy))
	{
		Enemy* e = static_cast<Enemy*>(object->getPtr());
		if (e->GetMyShot() || myShot)
		{
			int damage;

			//�����ق�(��΂���Ă�ق���)�_���[�W����
			int myDamage, otherDamage;
			myDamage = GetDamage();
			otherDamage = e->GetDamage();
			if (myDamage >= otherDamage)
				damage = myDamage;
			else
				damage = otherDamage;

			life -= damage;
			

			if (damage > 0)
			{
				isMuteki = true;
				ObjectManager::getInstance()->addObject(new DamageNumber(position, damage));
			}
		}
		
		
	}
}

int Enemy::GetDamage()
{
	//���ꂶ��_���[�W�S�R�^�����Ȃ�?
	//�Ȃ����_���[�W�̂�\������Ȃ�
	//damage������ĂȂ�����
	float damageNum = speed.x * 10;
	damageNum /= 3;
	if (damageNum >= 9)damageNum = 9;
	return (int)damageNum;

}


void Enemy::SetPlayerPos(const Vector3& pos, const int& playerType)
{
	playerPosition[playerType - 1] = pos;
}
