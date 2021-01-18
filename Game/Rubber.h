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
	//エネミーが紐にぶつかってるときに進む方向
	//pointNum1234 6789が同じ方向 5は{0,0,0}
	Vector3 enemyMoveVector;

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
};

