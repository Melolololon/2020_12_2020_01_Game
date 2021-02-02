#include "ParentEnemy.h"
#include"Enemy.h"
#include"ObjectManager.h"
#include"DamageNumber.h"
#include"PolygonManager.h"

int ParentEnemy::deadCount;
Vector3 ParentEnemy::playerPos[2];


const int ParentEnemy::CreateNum = 5;
int ParentEnemy::createCount;

ParentEnemy::ParentEnemy(const Vector3& pos, const PEnemyType& type)
{
	position = pos;
	pEnemyType = type;
	Initialize();
	
	generateTime = 60 * 2.5;
}


ParentEnemy::ParentEnemy(const Vector3& pos, const float& defAngle)
{
	position = pos;
	Initialize();
	angle = defAngle;
	pEnemyType = PENEMT_BARAMAKI;
	generateTime = 60 * 0.75;
}

ParentEnemy::~ParentEnemy()
{
	/*Library::deleteVertexData(vertexHandle);
	Library::deleteHeapData(heapHandle);*/
}

void ParentEnemy::loadModel()
{
	vertex v;
	heap h;
	std::string s;
	Library::loadOBJVertex("Resources/Obj/parentenemy.obj", true, true, &s, &v);
	Library::loadOBJMaterial("Resources/Obj/", s, CreateNum, &h);
	PolygonManager::getInstance()->addPolygonVertex("pEnemy", v);
	PolygonManager::getInstance()->addPolygonHeap("pEnemy", h);
}

void ParentEnemy::Initialize()
{
	vertexHandle = PolygonManager::getInstance()->getPolygonVertex("pEnemy");
	heapHandle = PolygonManager::getInstance()->getPolygonVertex("pEnemy");
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CreateNum ? 0 : createCount;

	/*position = {0,0,0 };*/
	position.y = 20;
	velocity = { 0, -1, 0 };
	speed = 0.5f;

	generateEnemyTimer = 0;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 4.0f;

	collisionFlag.board = false;
	collisionFlag.ray = false;
	collisionFlag.plane = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = true;

	Library::setScale({1.3,1.3,1.3}, heapHandle, heapNum);
	//‰¼()
	life = 7;

	isMuteki = false;
	mutekiTimer = 0;
	generateEnemyTimer = 0;

	angle = 0;
	generateEnemyTimer = 60 * -0.5;
}

void ParentEnemy::update()
{
	position = position + velocity * speed;
	Library::setPosition(position, heapHandle, heapNum);
	sphereData[0].position = position;



	//–³“Gˆ—
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

#pragma region ‰ñ“]ˆ—

	if (pEnemyType == PENEMY_TARGET)
	{
		Vector3 vec;
		Vector3 lPVec = playerPos[0] - position;
		Vector3 rPVec = playerPos[1] - position;
		if (LibMath::calcDistance3D(lPVec, position) < LibMath::calcDistance3D(rPVec, position))
			vec = LibMath::otherVector(position, lPVec);
		else
			vec = LibMath::otherVector(position, rPVec);

		angle = LibMath::vecto2rToAngle({ vec.x,vec.z });
	}

	if (pEnemyType == PENEMT_BARAMAKI)
	{
		angle += 3;
	}
	angle = angle >= 360 ? 0 : angle;
	Library::setAngle({ 0,-angle + 90,0 }, heapHandle, heapNum);
#pragma endregion

	if (position.y <= 0)
	{
		velocity = 0;
		position.y = 0;

		ShotEnemy();
	}
}

void ParentEnemy::draw()
{
	Library::setPipeline(PIPELINE_MATERIAL);
	if (!isMuteki)
		Library::drawGraphic(vertexHandle, heapHandle, heapNum);
	else
	{
		if (mutekiTimer % 2 == 0)
		{
			Library::drawGraphic(vertexHandle, heapHandle, heapNum);
		}
	}
	Library::setPipeline(PIPELINE_NORMAL);
}

void ParentEnemy::hit(Object * object, CollisionType collisionType)
{

	if (isMuteki)return;
	//‚Á”ò‚ñ‚Å‚é‚Æ‚«‚Å‚à“G‚Æ‚Ô‚Â‚©‚Á‚½‚çƒ_ƒ[ƒW
	if (typeid(*object) == typeid(Enemy))
	{
		//‚±‚±‚É“G‚ÆÕ“Ë‚µ‚½‚Æ‚«‚ÌƒR[ƒh‚ð‘‚­
		Enemy* e = static_cast<Enemy*>(object->getPtr());
		if (e->GetMyShot())
		{
			int damage = e->GetDamage();

			life -= damage;

			if (damage > 0) 
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

	if (generateEnemyTimer >= generateTime)
	{
		generateEnemyTimer = 0;

		if (pEnemyType == PEnemyType::PENEMY_TARGET)
			ObjectManager::getInstance()->addObject(new Enemy(position, Enemy::PLAYER_TARGET));


		if (pEnemyType == PEnemyType::PENEMT_BARAMAKI) 
		{
			Vector2 vec = LibMath::angleToVector2(angle);
			ObjectManager::getInstance()->addObject(new Enemy(position, { vec .x,0,vec .y}));
		}
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


void ParentEnemy::SetPlayerPosition(const Vector3& pos, const int& type)
{
	playerPos[type - 1] = pos;
}
