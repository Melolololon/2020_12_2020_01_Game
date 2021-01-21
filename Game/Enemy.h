#pragma once
#include "Object.h"
#include"LibMath.h"
#include"Vector.h"

class Enemy : public Object
{
	//目標タイプのenum
	enum TargetType
	{
		LEFT,
		RIGHT
	};

private:
	
	//目標タイプ
	TargetType targetType;
	//移動量更新に使用するタイマー
	int updateVelocityTimer = 0;

	//自分が発射されてるかどうか
	bool myShot;

	//ゴムと当たったかどうか
	bool hitRubber;
public:
	//コンストラクタ
	Enemy();
	//デストラクタ
	~Enemy();
	//初期化
	void Initialize();
	//更新
	void update() override;
	//描画
	void draw() override;

	void hit(Object* object, CollosionType collisionType)override;

	/// <summary>
	/// 移動量更新
	/// </summary>
	/// <param name="playerPosition">プレイヤー座標</param>
	void UpdateVelocity(Vector3 playerPosition);

	int GetUpdateVelocityTimer() { return updateVelocityTimer; }

	int GetTargetTypeAsInt();

	static Enemy* GetEnemy();

	void* getPtr()override;

	void GetVelocityAndSpeed(Vector3& vel,Vector3& spe);

	void AddPosition(const Vector3 vec);

	void ShotEnemy(const Vector3& vec);

	bool GetMyShot();
};

