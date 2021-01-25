#pragma once
#include "Object.h"
#include "Enemy.h"
#include <vector>

class ParentEnemy : public Object
{

private:

	//ライフ
	int life;

	//無敵処理
	bool isMuteki;
	int mutekiTimer;
	const int MutekiTime = 60 * 1.0f;

	//敵打ち出しに使う時間
	int generateEnemyTimer = 0;
	const int GENERATETIME = 60 * 3;                                                           

	static int deadCount;
public:

	//コンストラクタ
	ParentEnemy(const Vector3& pos);
	//デストラクタ
	~ParentEnemy();
	//初期化
	void Initialize();
	//更新
	void update() override;
	//描画
	void draw() override;

	void hit(Object* object, CollosionType collisionType)override;

	void* getPtr()override;

	void ShotEnemy();


	static int GetDeadCount();
	static void ResetDeadCount();
};

