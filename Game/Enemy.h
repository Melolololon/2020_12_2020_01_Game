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


	/// <summary>
	/// 移動量更新
	/// </summary>
	/// <param name="playerPosition">プレイヤー座標</param>
	void UpdateVelocity(Vector3 playerPosition);

	int GetUpdateVelocityTimer() { return updateVelocityTimer; }

	int GetTargetTypeAsInt();

	static Enemy* GetEnemy();
};

