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

