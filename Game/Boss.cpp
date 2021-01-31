#include "Boss.h"
#include"Enemy.h"
#include"DamageNumber.h"
#include"ObjectManager.h"
#include"DamageObject.h"

const int Boss::MutekiTime = 60 * 1.5f;

//揺れ
const Vector3 Boss::DefaultRTPos = {0,0,0};
const int Boss::YureTime = 60 * 1;

const Vector3 Boss::syogekihaVel[4][3] =
{
	{
		{0,0,-1},
		{0.5,0,-0.5},
		{-0.5,0,-0.5},
	},
	{
		{0,0,-1},
		{0.25,0,-0.75},
		{-0.25,0,-0.75},
	},
	{
		{0,0,-1},
		{0.75,0,-0.25},
		{-0.75,0,-0.25},
	},
{
		{0,0,-1},
		{-1,0,0},
		{1,0,0},
	},
};

Boss::Boss()
{
	position = { 0,30,13 };
	velocity = 0;
	speed = 0.1f;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 3.0f;


	collisionFlag.board = false;
	collisionFlag.ray = false;
	collisionFlag.plane = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = true;
	isDead = false;

	life = 15;
	mutekiTimer = 0;
	isMuteki = false;
	bossPatternTimer = 60 * -4;

	isJump = true;
	purePos = position;

	Library::createManyVertex3DBox({ 6,6,6 }, &vertexHandle);
	Library::createHeapData2({ 255,64,255,255 }, 1, &heapHandle);

	Library::setPosition(position, heapHandle, 0);

	yureFlag = false;
	yureTimer = 0;
	yureRandNum = 0;

	angle = 0;
}

//デストラクタ
Boss::~Boss()
{
}


//更新
void Boss::update()
{
	bossPatternTimer++;	
	bossPattern();
	
	purePos = position;
	position += velocity * speed;
	Library::setPosition(position, heapHandle, 0);
	sphereData[0].position = position;

	Library::setAngle(angle, heapHandle, 0);

	//無敵処理
	if (isMuteki)mutekiTimer++;
	if (mutekiTimer >= MutekiTime)
	{
		mutekiTimer = 0;
		isMuteki = false;
	}


#pragma region 画面揺らし

	Library::setRenderTargetPosition({0,0,0}, 0);

	yureRandNum = 0;
	if (yureFlag)
	{
		yureTimer++;
		if(yureTimer >= YureTime)
		{
			yureFlag = false;
			yureTimer = 0;
		}

		
		yureRandNum.x = Library::getRandomNumber(20) - 10 ;
		yureRandNum.y = Library::getRandomNumber(20) - 10 ;
		yureRandNum.x /= 400;
		yureRandNum.y /= 400;
		Library::setRenderTargetPosition(yureRandNum, 0);
	}
	
	if (position.y <= -50 && position.y >= -51)yureFlag = true;
#pragma endregion

	
}

//描画
void Boss::draw()
{
	Library::drawGraphic(vertexHandle, heapHandle, 0);
}

void Boss::hit(Object* object, CollisionType collisionType)
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

			if (damage > 0)
			{
				isMuteki = true;
				ObjectManager::getInstance()->addObject(new DamageNumber({ position.x,position.y + 3.0f,position.z }, damage));
			}

			if (life <= 0)
			{
				isJump = true;
				velocity.y = 6.0f;
				velocity.z = 1.0f;
			}
		}

	}
}

void* Boss::getPtr()
{
	return this;
}

void Boss::bossPattern() 
{
	if (isJump) velocity.y -= 0.1f;

	if (life <= 0)
	{
		angle.x += 7;
		return;
	}

	//ジャンプして着地したら
	//y = 0にしない(地上にいる時0にならないから)
	if(purePos.y > position.y && 
		position.y <= 0 )
	{
		position.y = 0;
		velocity = 0;
		isJump = false;
		yureFlag = true;

		if (bossPatternTimer < 60 * 0)return;

		int arrNum = Library::getRandomNumber(3);
		for (int i = 0; i < 3; i++) 
		{
			ObjectManager::getInstance()->addObject
			(
				new DamageObject
				(
					position,
					syogekihaVel[arrNum][i], 
					{ 0.2f,0,0.2f },
					DamageObject::DAMAGE_OBJECT_SYOGEKIHA
				)
			);
		}
	}

	if (bossPatternTimer < 60 * 0)return;

	if (bossPatternTimer == 60 * 2) 
	{
		isJump = true;
		velocity.y = 4.0f;
	}

	if (bossPatternTimer == 60 * 6) 
	{
		ObjectManager::getInstance()->addObject(new Enemy(position, Enemy::PLAYER_TARGET));
	}

	if (bossPatternTimer == 60 * 7)
	{
		ObjectManager::getInstance()->addObject(new Enemy(position, Enemy::PLAYER_TARGET));
	}

	if (bossPatternTimer == 60 * 8)
	{
		if (life >= 7) 
		{
			ObjectManager::getInstance()->addObject(new Enemy(position, Enemy::PLAYER_TARGET));
		}
		else
		{
			ObjectManager::getInstance()->addObject(new Enemy(position, Enemy::PLAYER_TUIBI));
		}
		bossPatternTimer = 0;
	}

}


bool Boss::getIsJump() 
{
	return isJump;
}

bool Boss::getDropFlag()
{
	if (velocity.y <= -50) return true;
	return false;
	
}

bool Boss::getLifeZero() 
{
	if (life <= 0)return true;
	return false;
}

Vector3 Boss::getYureRandNum()
{
	return yureRandNum;
}
