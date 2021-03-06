#pragma once
#include"Object.h"
#include"Enemy.h"
class Boss2 :public Object
{	

private:

	//ライフ
	int life;

	//無敵処理
	bool isMuteki;
	int mutekiTimer;
	static const int MutekiTime; /*= 60 * 1.5f;*/

	int bossPatternTimer;
	
	Vector3 angle;
	Vector3 scale;
	float punchAngle;

	//腕
	Vector3 punchScale[2];
	vertex bossPunchVertexH;
	heap bossPunchHeapH;

	static Vector3 playerPos[2];

	void punch();
	
	int punchTimer;
	static const int punchTime;
	bool punchJunbi;
	static bool punchJunbiFlag[2];
	Vector3 punchPos[2];

	void syogeki();
	int syogekiTimer;
	static const int syogekiTime;
	bool syogekiZyunbi;

	//画面揺らし
	static const Vector3 DefaultRTPos;
	bool yureFlag;
	int yureTimer;
	static const int YureTime;
	Vector3 yureRandNum;

	static const Vector3 syogekihaVel[3][5];

	int heapNum;
	static const int CreateNum;
	static int createCount;

	int punchHeapNum;
	static const int PunchCreateNum;
	static int punchCreateCount;
public:
	Boss2();
	~Boss2();

	static void loadModel();
	//更新
	void update() override;
	void bossPattern();
	//描画
	void draw() override;

	void hit(Object* object, CollisionType collisionType)override;

	void* getPtr()override;

	static void setPlayerPos(const Vector3& pos,const int& type);

	bool getLifeZero();
	bool scaleZeroFlag();

};

