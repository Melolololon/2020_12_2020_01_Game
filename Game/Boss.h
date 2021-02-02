#pragma once
#include"Object.h"
class Boss :public Object
{

private:

	//ライフ
	int life;

	//無敵処理
	bool isMuteki;
	int mutekiTimer;
	static const int MutekiTime; /*= 60 * 1.5f;*/

	int bossPatternTimer;

	bool isJump;
	Vector3 purePos;

	static const Vector3 syogekihaVel[4][3];

	//画面揺らし
	static const Vector3 DefaultRTPos;
	bool yureFlag;
	int yureTimer;
	static const int YureTime;
	Vector3 yureRandNum;

	Vector3 angle;

	int heapNum;
	static const int CreateNum;
	static int createCount;
public:

	//コンストラクタ
	Boss();
	//デストラクタ
	~Boss();

	static void loadModel();
	//更新
	void update() override;
	void bossPattern();
	//描画
	void draw() override;

	void hit(Object* object, CollisionType collisionType)override;

	void* getPtr()override;

	bool getIsJump();

	bool getLifeZero();
	//lifeを0にしたあとに崖に落ちたかどうか
	bool getDropFlag();

	Vector3 getYureRandNum();
};

