#pragma once
#include"Object.h"
//ゴムひもクラス
class Rubber :public Object
{
private:
	//プレイヤーからプレイヤーへのベクトル(非正規化)
	static Vector3 vecPToP;
	static Vector3 playerPos;//どっちかのプレイヤーの座標(どちらでもいい)

	//何個目の点か(これにより座標を決める)
	int pointNum;
	//エネミーが紐にぶつかってるときに進む敵の進行方向
	//pointNum1234 6789が同じ方向 5は{0,0,0}
	Vector3 enemyMoveVector;
	
	//Enemyとどれか1つが当たったかどうか
	bool hitEnemy;

	//紐の限界移動量
	const float MaxMoveDistance[5] = {2.0f,6.0f,12.0f,16.0f,20.0f};

	//近くのゴム動かすときの倍率
	const float MoveBairitu[8] = { 0.8f,0.7f,0.6f,0.5f,0.4f,0.3f,0.2f,0.1f };

	//お互いアクセスできる
	static Rubber* rubberPtr[9];

	//規定位置(動かされてないとにの座標)
	Vector3 normalPos;

	static bool dashFlag;
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="pNum">何個目の点か(これにより座標を決める)</param>
	Rubber(const int& pNum);
	~Rubber();

	void update();
	void draw();
	void hit(Object* object, CollosionType collisionType)override;

	static void setPlayerPosition(const Vector3& pos);
	static void setPlayerToPlayerVector(const Vector3& vector);

	static void setRubberPtr( Rubber* rP[]);

	void movePosition(const Vector3& vec);
	void setHitEnemy(const bool& flag);

	static void setDashFlag(const bool& flag);
};

