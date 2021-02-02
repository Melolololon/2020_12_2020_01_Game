#pragma once
#include "Object.h"
#include"LibMath.h"
#include"Vector.h"

class Enemy : public Object
{
public:
	//目標タイプのenum
	enum TargetType
	{
		LEFT,
		RIGHT
	};

	enum EnemyType
	{
		PLAYER_TARGET,
		PLAYER_TUIBI,
		SET_VELOCITY,
		BOSS2_PUNCH
	};

private:
	
	//自分のタイプ
	EnemyType enemyType;

	//目標タイプ
	TargetType targetType;
	//移動量更新に使用するタイマー
	int updateVelocityTimer = 0;

	//自分が発射されてるかどうか
	bool myShot;

	//ライフ
	int life;

	//無敵処理
	bool isMuteki;
	int mutekiTimer;
	const int MutekiTime = 60 * 0.2f;

	static Vector3 playerPosition[2];

	//移動量をプレイヤーの向きに更新する間隔
//極端に少ないとクリア不可能になる
	const int UPDATE_VELOCITY_TIME = 30;
	const int ADD_ENEMY_TIME = 180;

	int tuibiEndTimer;
	static const int TuibiEndTime;

	int heapNum;
	static const int CreateNum;
	static int createCount;


	static const int PunchCreateNum;
	static int punchCreateCount;
public:
	//コンストラクタ
	Enemy(const Vector3& pos,const EnemyType& enemyType);
	//コンストラクタ
	Enemy(const Vector3& pos,const Vector3& vel);
	//デストラクタ
	~Enemy();
	
	static void loadModel();
	
	//初期化
	void Initialize();
	//更新
	void update() override;
	//描画
	void draw() override;

	void hit(Object* object, CollisionType collisionType)override;


	
	void UpdateVelocity();

	//int GetUpdateVelocityTimer() { return updateVelocityTimer; }

	int GetTargetTypeAsInt();

	//static Enemy* GetEnemy();

	static void SetPlayerPos(const Vector3& pos,const int& playerType);

	void* getPtr()override;

	void GetVelocityAndSpeed(Vector3& vel,Vector3& spe);

	void SetPosition(const Vector3& pos);
	void AddPosition(const Vector3& vec);

	void ShotEnemy(const Vector3& vel ,const Vector3& spe);

	bool GetMyShot();

	int GetDamage();


};

