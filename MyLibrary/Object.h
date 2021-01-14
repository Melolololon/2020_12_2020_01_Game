#pragma once
#include<typeinfo>
#include<vector>
#include"Library.h"
#include"Vector.h"
#include"CollisionType.h"

//オブジェクトマネージャー追加時に判定を選ぶようにする?(判定ごとに追加関数と配列作る)

class Object
{
public:
	//判定
	struct CollisionFlag
	{
		bool sphere;
		bool lay;
		bool lineSegment;
		bool plane;
		bool board;
	};

protected:
	//座標
	Vector3 position;
	//向き
	Vector3 velocity;
	//移動速度
	Vector3 speed;

	//生死フラグ 
	bool isDead;

	//頂点データを呼び出すもの
	vertex vertexHandle;
	//ヒープデータを呼び出すもの
	heap heapHandle;
	//ヒープのどこにあるかを表す番号
	int heapNumber;

#pragma region 判定データ
	std::vector<SphereData> sphereData;
	std::vector<LineSegmentData> lineSegmentData;
	std::vector<LayData> layData;
	std::vector<PlaneData>planeData;
	std::vector<BoardData>boardData;
#pragma endregion

	CollisionFlag collisionFlag;
public:

	//コンストラクタ
	Object();
	//デストラクタ
	virtual ~Object();
	//処理
	virtual void update() = 0;
	//描画処理
	virtual void draw() = 0;
	//判定処理
	virtual void hit(Object* object, CollosionType collisionType);

	bool getIsDead();
	CollisionFlag getCollisionFlag();
	std::vector<SphereData> getSphereData();
	std::vector<LineSegmentData> getLineSegmentData();
	std::vector<PlaneData> getPlaneData();
	std::vector<BoardData> getBoardData();

	virtual void* getPtr();

	
};