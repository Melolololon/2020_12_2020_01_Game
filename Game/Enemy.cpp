#include "Enemy.h"
#include"Rubber.h"
#include"DamageNumber.h"

#include"Player.h"

#include"ObjectManager.h"
#include"PolygonManager.h"
#include"Boss.h"
#include"Boss2.h"
#include"Particle.h"

Vector3 Enemy::playerPosition[2];

const int Enemy::TuibiEndTime = 60 * 7;

const int Enemy::CreateNum = 20;
int Enemy::createCount;

const int Enemy::PunchCreateNum = 10;
int Enemy::punchCreateCount;

bool Enemy::allDeadFlag;

Enemy::Enemy(const Vector3& pos, const EnemyType& enemyType)
{

	position = pos;
	this->enemyType = enemyType;

	Initialize();


	
}

Enemy::Enemy(const Vector3& pos, const Vector3& vel)
{

	position = pos;
	Initialize();
	velocity = vel;
	enemyType = SET_VELOCITY;

}

Enemy::~Enemy()
{
	//Library::deleteVertexData(vertexHandle);
	//Library::deleteHeapData(heapHandle);
}

void Enemy::loadModel()
{
	vertex v;
	heap h;
	std::string s;
	Library::loadOBJVertex("Resources/Obj/enemy.obj", true, true, &s, &v);
	Library::loadOBJMaterial("Resources/Obj/", s, CreateNum, &h);
	PolygonManager::getInstance()->addPolygonVertex("enemy", v);
	PolygonManager::getInstance()->addPolygonHeap("enemy", h);


	Library::loadOBJVertex("Resources/Obj/punch.obj", true, true, &s, &v);
	Library::loadOBJMaterial("Resources/Obj/", s, PunchCreateNum, &h);
	PolygonManager::getInstance()->addPolygonVertex("shotPunch", v);
	PolygonManager::getInstance()->addPolygonHeap("shotPunch", h);

}

void Enemy::Initialize()
{	//モデル変更
	if (enemyType == EnemyType::BOSS2_PUNCH)
	{
		vertexHandle = PolygonManager::getInstance()->getPolygonVertex("shotPunch");
		heapHandle = PolygonManager::getInstance()->getPolygonVertex("shotPunch");
		heapNum = punchCreateCount;
		punchCreateCount++;
		punchCreateCount = punchCreateCount >= PunchCreateNum ? 0 : punchCreateCount;
		//enemyType = EnemyType::PLAYER_TUIBI;
		speed = 0.2f;
		Library::setScale({ 3,3,3 }, heapHandle, heapNum);
	}
	else 
	{
		vertexHandle = PolygonManager::getInstance()->getPolygonVertex("enemy");
		heapHandle = PolygonManager::getInstance()->getPolygonVertex("enemy");
		heapNum = createCount;
		createCount++;
		createCount = createCount >= CreateNum ? 0 : createCount;
	}
	
	//座標はとりあえず奥からランダムで。
	//position = { 0, 0, 10 };
	//移動量はとりあえず手前方向へ。
	velocity = { 0, 0, -1.0f };

	//スピードはとりあえず2.0で。
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

	//ライフ
	life = 3;

	//無敵処理
	isMuteki = false;
	mutekiTimer = 0;

	float dir[2];
	dir[0] = LibMath::calcDistance3D(playerPosition[0], position);
	dir[1] = LibMath::calcDistance3D(playerPosition[1], position);
	if (dir[0] < dir[1]) 
	targetType = TargetType::LEFT;
	else
		targetType = TargetType::RIGHT;

	if (enemyType == EnemyType::PLAYER_TARGET ||
		enemyType == EnemyType::BOSS2_PUNCH) 
	{
		velocity = LibMath::otherVector(position, playerPosition[targetType]);
	}




	tuibiEndTimer = 0;

	Library::setPosition(position, heapHandle, heapNum);
}

void Enemy::update()
{
	if (allDeadFlag)isDead = true;

	UpdateVelocity();
	position = position + velocity * speed;
	Library::setPosition(position, heapHandle, heapNum);
	sphereData[0].position = position;
	updateVelocityTimer++;

	//無敵処理
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

	Library::setPipeline(PIPELINE_MATERIAL);
	if (enemyType == EnemyType::BOSS2_PUNCH)
	{
		Library::setSmoothingFlag(true);
	}

	Library::drawGraphic(vertexHandle, heapHandle, heapNum);

	Library::setPipeline(PIPELINE_NORMAL);
	Library::setSmoothingFlag(false);
}

void Enemy::UpdateVelocity()
{
	//速度落ちたら再度追尾(追尾しない奴はとりあえずそのまま直進)
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


	tuibiEndTimer++;
	if (tuibiEndTimer >= TuibiEndTime)
		return;

	if (enemyType != EnemyType::PLAYER_TUIBI && enemyType != EnemyType::BOSS2_PUNCH ||
		updateVelocityTimer < UPDATE_VELOCITY_TIME )return;


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
	Library::setPosition(position, heapHandle, heapNum);
	sphereData[0].position = position;
}

void Enemy::AddPosition(const Vector3& vec)
{
	position += vec;
	Library::setPosition(position, heapHandle, heapNum);
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

	//吹っ飛んでるときでも敵とぶつかったらダメージ
	if (typeid(*object) == typeid(Enemy))
	{
		Enemy* e = static_cast<Enemy*>(object->getPtr());
		if (e->GetMyShot() || myShot)
		{
			int damage;

			//多いほう(飛ばされてるほうの)ダメージを代入
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
			if (life <= 0)
			{
				for (int i = 0; i < 5; i++) 
				{
					ObjectManager::getInstance()->addObject(new Particle(position, Particle::PARTICLE_ENEMY_DEAD));
				}
			}
		}
		
		
	}

	
}

int Enemy::GetDamage()
{
	//これじゃダメージ全然与えられない?
	//なぜかダメージのやつ表示されない
	//damage代入してなかった
	float damageNum = speed.x * 10;
	damageNum /= 3;
	if (damageNum >= 9)damageNum = 9;
	return (int)damageNum;

}


void Enemy::SetPlayerPos(const Vector3& pos, const int& playerType)
{
	playerPosition[playerType - 1] = pos;
}

void Enemy::allDead(const bool& flag)
{
	allDeadFlag = flag;
}
