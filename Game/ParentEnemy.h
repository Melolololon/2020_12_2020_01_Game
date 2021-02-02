#pragma once
#include "Object.h"
#include "Enemy.h"
#include "Vector.h"
#include <vector>

class ParentEnemy : public Object
{
public:
	enum PEnemyType
	{
		PENEMY_TARGET,
		PENEMT_BARAMAKI,
	};

private:

	//ライフ
	int life;

	//無敵処理
	bool isMuteki;
	int mutekiTimer;
	const int MutekiTime = 60 * 1.5f;

	//敵打ち出しに使う時間
	int generateEnemyTimer;
	int generateTime;

	static int deadCount;

	PEnemyType pEnemyType;
	float angle;

	static Vector3 playerPos[2];

	static const int CreateNum;
	static  int createCount;
	int heapNum;
public:

	//コンストラクタ
	ParentEnemy(const Vector3& pos, const PEnemyType& type);
	ParentEnemy(const Vector3& pos, const float& defAngle);
	//デストラクタ
	~ParentEnemy();

	static void loadModel();

	//初期化
	void Initialize();
	//更新
	void update() override;
	//描画
	void draw() override;

	void hit(Object* object, CollisionType collisionType)override;

	void* getPtr()override;

	void ShotEnemy();


	static int GetDeadCount();
	static void ResetDeadCount();

	static void SetPlayerPosition(const Vector3& pos,const int& type);
};

