#include "ParentEnemy.h"
#include"Enemy.h"
#include"ObjectManager.h"
#include"DamageNumber.h"

int ParentEnemy::deadCount;

ParentEnemy::ParentEnemy(const Vector3& pos)
{
	position = pos;
	Library::createManyVertex3DBox({ 6,6,6 }, &vertexHandle);
	Library::createHeapData2({ 255,64,255,255 }, 3, &heapHandle);
	Initialize();
}

ParentEnemy::~ParentEnemy()
{
	Library::deleteVertexData(vertexHandle);
	Library::deleteHeapData(heapHandle);
}

void ParentEnemy::Initialize()
{

	/*position = {0,0,0 };*/
	velocity = { 0, 0, 0 };
	speed = 0.0f;

	generateEnemyTimer = 0;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 3.0f;

	collisionFlag.board = false;
	collisionFlag.ray = false;
	collisionFlag.plane = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = true;

	//��()
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

	//���G����
	if (isMuteki)mutekiTimer++;
	if (mutekiTimer >= MutekiTime)
	{
		mutekiTimer = 0;
		isMuteki = false;
	}

	if (life <= 0)
	{
		deadCount++;
		isDead = true;
	}

	ShotEnemy();
}

void ParentEnemy::draw()
{
	if (!isMuteki)
		Library::drawGraphic(vertexHandle, heapHandle, 0);
	else
	{
		if (mutekiTimer % 2 == 0)
		{
			Library::drawGraphic(vertexHandle, heapHandle, 0);
		}
	}
}

void ParentEnemy::hit(Object * object, CollosionType collisionType)
{

	if (isMuteki)return;
	//�������ł�Ƃ��ł��G�ƂԂ�������_���[�W
	if (typeid(*object) == typeid(Enemy))
	{
		//�����ɓG�ƏՓ˂����Ƃ��̃R�[�h������
		Enemy* e = static_cast<Enemy*>(object->getPtr());
		if (e->GetMyShot())
		{
			int damage = e->GetDamage();

			life -= damage;

			if (damage != 0) 
			{
				isMuteki = true;
				ObjectManager::getInstance()->addObject(new DamageNumber({ position.x,position.y + 3.0f,position.z }, damage));
			}
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

int ParentEnemy::GetDeadCount()
{
	return deadCount;
}

void ParentEnemy::ResetDeadCount()
{
	deadCount = 0;
}

