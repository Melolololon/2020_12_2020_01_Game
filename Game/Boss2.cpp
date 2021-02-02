#include "Boss2.h"
#include"Enemy.h"
#include"ObjectManager.h"
#include"DamageNumber.h"
#include"DamageObject.h"
#include"PolygonManager.h"

const int Boss2::MutekiTime = 60 * 1.5f;
Vector3 Boss2::playerPos[2];
const int Boss2::punchTime = 60 * 0.5f;
bool Boss2::punchJunbiFlag[2];

const int Boss2::syogekiTime = 60 * 0.8;

const int Boss2::CreateNum = 1;
int Boss2::createCount;


const int Boss2::PunchCreateNum = 2;
int Boss2::punchCreateCount;
//揺れ
const Vector3 Boss2::DefaultRTPos = { 0,0,0 };
const int Boss2::YureTime = 60 * 1;


const Vector3 Boss2::syogekihaVel[3][5] =
{
	{
		{0,0,-1},
		{0.75,0,-0.25},
		{-0.75,0,-0.25},
		{0.25,0,-0.75},
		{-0.25,0,-0.75},
	},

	{
		{0,0,-1},
		{0.75,0,-0.25},
		{0.5,0,-0.5},
		{0.25,0,-0.75},
		{1,0,0.0},
	},
	{
		{0,0,-1},
		{-0.75,0,-0.25},
		{-0.5,0,-0.5},
		{-0.25,0,-0.75},
		{-1,0,0.0},
	},
};
Boss2::Boss2()
{

	position = { 0,-50,28 };
	velocity = 0;
	speed = 0.3f;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 3.5f;


	collisionFlag.board = false;
	collisionFlag.ray = false;
	collisionFlag.plane = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = true;
	isDead = false;

	life = 15;
	
	mutekiTimer = 0;
	isMuteki = false;
	bossPatternTimer = 60 * -6;



	vertexHandle = PolygonManager::getInstance()->getPolygonVertex("boss2");
	heapHandle = PolygonManager::getInstance()->getPolygonVertex("boss2");
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CreateNum ? 0 : createCount;

	bossPunchVertexH = PolygonManager::getInstance()->getPolygonVertex("punch");
	bossPunchHeapH = PolygonManager::getInstance()->getPolygonVertex("punch");
	punchHeapNum = punchCreateCount;
	punchCreateCount += 2;
	punchCreateCount = punchCreateCount >= PunchCreateNum ? 0 : punchCreateCount;

	Library::setPosition(position, heapHandle, heapNum);

	angle = 0;
	scale = 3;
	Library::setScale(scale, heapHandle, heapNum);
	punchAngle = 0;
	punchScale[0] = 1;
	punchScale[1] = 1;

	punchPos[0] = { 3,0,2 };
	punchPos[1] = { 3,0,-2 };

	Library::setPosition({0,0,6}, bossPunchHeapH, punchHeapNum);
	Library::setPosition({0,0,-6}, bossPunchHeapH, punchHeapNum + 1);

	punchTimer = 0;
	punchJunbi = false;

	 syogekiTimer = 0;
	 syogekiZyunbi = 0;

	 yureFlag = false;
	 yureTimer = 0;
	 yureRandNum = 0;
}

Boss2::~Boss2()
{
}

void Boss2::loadModel()
{
	vertex v;
	heap h;
	std::string s;
	Library::loadOBJVertex("Resources/Obj/boss2.obj", true, true, &s, &v);
	Library::loadOBJMaterial("Resources/Obj/", s, CreateNum, &h);
	PolygonManager::getInstance()->addPolygonVertex("boss2", v);
	PolygonManager::getInstance()->addPolygonHeap("boss2", h);


	Library::loadOBJVertex("Resources/Obj/punch.obj", true, true, &s, &v);
	Library::loadOBJMaterial("Resources/Obj/", s, PunchCreateNum, &h);
	PolygonManager::getInstance()->addPolygonVertex("punch", v);
	PolygonManager::getInstance()->addPolygonHeap("punch", h);
}

//更新
void Boss2::update()
{
	bossPatternTimer++;
	bossPattern();

	position += velocity * speed;
	Library::setPosition(position, heapHandle, heapNum);
	sphereData[0].position = position;

	Library::setParent(bossPunchHeapH, punchHeapNum, heapHandle, heapNum);
	Library::setParent(bossPunchHeapH, punchHeapNum + 1, heapHandle, heapNum);
	Library::setPosition(punchPos[0], bossPunchHeapH, punchHeapNum);
	Library::setPosition(punchPos[1], bossPunchHeapH, punchHeapNum + 1);

	for (int i = 0; i < _countof(punchScale); i++)
	{
		if (punchScale[i].x < 1)
		{
			punchScale[i].x += 0.05;
			punchScale[i].y += 0.05;
			punchScale[i].z += 0.05;
		}
		else
		{
			punchScale[i] = 1;
		}

	}

	Library::setScale(punchScale[1], bossPunchHeapH, punchHeapNum);
	Library::setScale(punchScale[0], bossPunchHeapH, punchHeapNum + 1);

	//無敵処理
	if (isMuteki)mutekiTimer++;
	if (mutekiTimer >= MutekiTime)
	{
		mutekiTimer = 0;
		isMuteki = false;
	}

#pragma region 回転処理
	if (life <= 0 && scale.x > 0)
	{
		angle.y += 5;
		Library::setAngle({ 0,angle.y ,0 }, heapHandle, heapNum);
		scale.x -= 0.02f;
		scale.y -= 0.02f;
		scale.z -= 0.02f;
		Library::setScale(scale, heapHandle, heapNum);

		return;
	}
	Vector3 vec;
	Vector3 lPVec = playerPos[0] - position;
	Vector3 rPVec = playerPos[1] - position;
	if (LibMath::calcDistance3D(lPVec, position) < LibMath::calcDistance3D(rPVec, position))
		vec = LibMath::otherVector(position, lPVec);
	else
		vec = LibMath::otherVector(position, rPVec);

	angle.y = LibMath::vecto2rToAngle({ vec.x,vec.z });



	angle.y = angle.y >= 360 ? 0 : angle.y;
	Library::setAngle({ angle.x,-angle.y + punchAngle,angle.z }, heapHandle, heapNum);
	Library::setAngle({ 0,0,0 }, bossPunchHeapH, punchHeapNum);
	Library::setAngle({ 0,0,0 }, bossPunchHeapH, punchHeapNum + 1);
#pragma endregion



#pragma region 画面揺らし

	Library::setRenderTargetPosition({ 0,0,0 }, 0);

	yureRandNum = 0;
	if (yureFlag)
	{
		yureTimer++;
		if (yureTimer >= YureTime)
		{
			yureFlag = false;
			yureTimer = 0;
		}


		yureRandNum.x = Library::getRandomNumber(20) - 10;
		yureRandNum.y = Library::getRandomNumber(20) - 10;
		yureRandNum.x /= 400;
		yureRandNum.y /= 400;
		Library::setRenderTargetPosition(yureRandNum, 0);
	}

	/*if (position.y <= -50 && position.y >= -51)yureFlag = true;*/
#pragma endregion

	
}

void  Boss2::bossPattern() 
{
	bossPatternTimer++;

	if (life <= 0)return;

	if (position.y < 2.5) 
	{
		velocity.y = 1;
	}
	else 
		if(position.z >= 18)
	{
		position.y = 2.5;
		velocity.y = 0;
		velocity.z = -1;

	}
	else
	{
		velocity.z = 0;
	
		Vector3 vec;
		Vector3 lPVec = playerPos[0] - position;
		Vector3 rPVec = playerPos[1] - position;
		if (LibMath::calcDistance3D(lPVec, position) < LibMath::calcDistance3D(rPVec, position))
			vec = LibMath::otherVector(position, lPVec);
		else
			vec = LibMath::otherVector(position, rPVec);
		velocity.x = vec.x * 4;
		speed = 0.15f;
	}

	if(bossPatternTimer == 60 * 7)
	{
		punchJunbi = true;
	}

	if (bossPatternTimer == 60 * 12) 
	{
		syogekiZyunbi = true;
		bossPatternTimer = 0;
	}



	syogeki();
	punch();
}
//描画
void  Boss2::draw()
{
	Library::setPipeline(PIPELINE_MATERIAL);
	Library::setSmoothingFlag(true);

	if (life > 0)
	{
		if (!isMuteki)
		{
			Library::drawGraphic(vertexHandle, heapHandle, heapNum);
			Library::drawGraphic(bossPunchVertexH, bossPunchHeapH, punchHeapNum);
			Library::drawGraphic(bossPunchVertexH, bossPunchHeapH, punchHeapNum + 1);
		}
		else
		{
			if (mutekiTimer % 2 == 0)
			{
				Library::drawGraphic(vertexHandle, heapHandle, heapNum);
				Library::drawGraphic(bossPunchVertexH, bossPunchHeapH, punchHeapNum);
				Library::drawGraphic(bossPunchVertexH, bossPunchHeapH, punchHeapNum + 1);
			}
		}
	}
	else 
	{
		Library::drawGraphic(vertexHandle, heapHandle, heapNum);
		Library::drawGraphic(bossPunchVertexH, bossPunchHeapH, punchHeapNum);
		Library::drawGraphic(bossPunchVertexH, bossPunchHeapH, punchHeapNum + 1);
	}

	Library::setPipeline(PIPELINE_NORMAL);
	Library::setSmoothingFlag(false);
}

void  Boss2::hit(Object* object, CollisionType collisionType)
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
			
			}
		}

	}
}

void* Boss2::getPtr()
{
	return this;
}

void Boss2::setPlayerPos(const Vector3& pos, const int& type)
{
	playerPos[type - 1] = pos;
}

void Boss2::punch()
{

	int num = 0;
	//どっち飛ばすか決める
    if(punchJunbi && punchTimer == 0)
	{
		num = Library::getRandomNumber(2);
		punchJunbiFlag[num] = true;

		
	}

	//カウント
	if (punchJunbi)punchTimer++;
	if (punchTimer >= punchTime)
	{
		if (punchJunbiFlag[0])punchAngle -= 5.8f;
		if (punchJunbiFlag[1])punchAngle += 5.8f;
	}
	else
	{

		if (punchJunbiFlag[0])punchAngle += 1.8f;
		if (punchJunbiFlag[1])punchAngle -= 1.8f;

	}
	if (punchTimer < punchTime)return;
	if (punchJunbiFlag[0] && punchAngle > 0)return;
	if (punchJunbiFlag[1] && punchAngle < 0)return;

	//位置指定と発射
	punchTimer = 0;
	punchJunbi = false;
	punchAngle = 0;

	Vector3 ePos = position;
	ePos.y = 0;
	ePos.z -= 5;
	if (punchJunbiFlag[0])
	{
		ePos.x -= 6;
		ObjectManager::getInstance()->addObject(new Enemy(ePos, Enemy::BOSS2_PUNCH));
		punchJunbiFlag[0] = false;

		punchScale[0] = 0;
	}
	if (punchJunbiFlag[1])
	{
		ePos.x += 6;
		ObjectManager::getInstance()->addObject(new Enemy(ePos, Enemy::BOSS2_PUNCH));
		punchJunbiFlag[1] = false;

		punchScale[1] = 0;
	}


	

}

bool Boss2::getLifeZero()
{
	if (life <= 0)return true;
	return false;
}

bool Boss2::scaleZeroFlag() 
{
	if (scale.x <= 0)return true;
	return false;
}

void Boss2::syogeki()
{
	if (syogekiZyunbi)syogekiTimer++;
	if (syogekiTimer < syogekiTime - 60 * 0.2 && syogekiZyunbi)
		angle.z += 2;
	if (syogekiTimer >= syogekiTime)
		angle.z -= 10;
	if (syogekiTimer < syogekiTime || angle.z > -45)return;
	angle.z = 0;
	syogekiTimer = 0;
	syogekiZyunbi = false;
	yureFlag = true;

	int arrNum = Library::getRandomNumber(3);
	for (int i = 0; i < 5; i++)
	{
		ObjectManager::getInstance()->addObject
		(
			new DamageObject
			(
				{ position.x ,position.y - 3 ,position .z},
				syogekihaVel[arrNum][i],
				{ 0.2f,0,0.2f },
				DamageObject::DAMAGE_OBJECT_SYOGEKIHA
			)
		);
	}
}

