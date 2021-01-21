#pragma once
#include"Object.h"

//ゴムひもクラス


class Rubber :public Object
{
private:

	//何個目の点か(これにより座標を決める)
	int pointNum;
	//エネミーが紐にぶつかってるときに進む敵の進行方向
	//pointNum1234 6789が同じ方向 5は{0,0,0}
	Vector3 enemyMoveVector;
	
	//Enemyとどれか1つが当たったかどうか
	bool hitEnemy;

	//ゴムの限界移動量(隣との距離がこれ以上になったら限界)
	const float MaxMoveDistance = 2.0f;

	//近くのゴム動かすときの倍率
	const float MoveBairitu[8] = { 0.8f,0.6f,0.5f,0.4f,0.38f,0.1f,0.07f,0.05f };

	//お互いアクセスできる
	static Rubber* rubberPtr[9];

	//規定位置(動かされてないとにの座標)
	Vector3 normalPos;

	//ダッシュしたかどうか
	static bool dashFlag;

	//限界かつ未操作時にプレイヤーがどのくらい動くか
	static Vector3 playerMoveVector;


	//0 レフト 1 ライト
	//プレイヤーからプレイヤーへのベクトル(非正規化)
	static Vector3 vecPToP[2];
	static Vector3 playerPos[2];
	static Vector3 playerVelocity[2];
	static Vector3 playerSpeed[2];
	static bool playerInputFlag[2];

	//リミットかどうか
	static int rimitCount;
	static bool allRimitFlag;

	static bool leavePlayerFlag;

	//ダッシュが終ったかどうか
	static bool playerDashEnd;
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

	static void setPlayerPosition(const Vector3& pos, const int& pNum);
	static void setPlayerToPlayerVector(const Vector3& vector, const int& pNum);

	static void setRubberPtr( Rubber* rP[]);

	void addPosition(const Vector3& vec);
	void setHitEnemy(const bool& flag);

	static void setDashFlag(const bool& flag);

	static Vector3 getPlayerMoveVector();

	static void setPlayerVectorAndSpeed(const Vector3& vel, const Vector3& spe,const int& pNum);
	static void resetRimitCount();
	static void setPlayerInputFlag(const bool& flag, const int& pNum);
	static void setLeavePlayerFlag(const bool& flag);
};

