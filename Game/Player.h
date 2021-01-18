#pragma once
#include"Object.h"
#include"LibMath.h"
#include"Vector.h"
class Player : public Object
{
public:
	enum PlayerType
	{
		NONE,
		LEFT,
		RIGHT,
	};

private:

	enum InputDirection
	{
		INPUT_DIRECTION_CENTER,
		INPUT_DIRECTION_UP,
		INPUT_DIRECTION_DOWN,
		INPUT_DIRECTION_LEFT,
		INPUT_DIRECTION_RIGHT,
	};

	static PlayerType firstAddType;
	static Vector3 leftPlayerPosition;
	static Vector3 rightPlayerPosition;
	static Vector3 leftPlayerVelocity;
	static Vector3 rightPlayerVelocity;
	static Player* firstAddPlayer;

	//自分のプレイヤータイプ(LかRか)
	PlayerType playerType;

#pragma region ダッシュ
	//同じ方向に2回入力するとダッシュ

	//入力方向
	InputDirection leftInputDirectionUpDown;
	InputDirection leftInputDirectionLeftRight;
	InputDirection rightInputDirectionUpDown;
	InputDirection rightInputDirectionLeftRight;

	//ダッシュ入力可能かどうか(スティックを一定位置まで戻さないとtrueにならない)
	bool dashOperPossibleFlag;
	//入力受付時間かどうか
	bool dashOperTimeFlag;
	//ダッシュするための二回目のスティック入力受付時間
	int dashOperTimer;
	const int DashOperTime = 60.0f * 0.2f;

	//ダッシュ時間
	int dashTimer;
	const int DashTime = 60.0f * 0.25f;

	//ダッシュしてるかどうか
	bool isDash;
	//最高ダッシュスピード
	const Vector3 MaxDashSpeed = { 1.0f,1.0f,1.0f };
	//通常スピード
	const Vector3 NormalSpeed = { 0.2f,0.2f,0.2f };
#pragma endregion


	//最高距離(playerMaxDistanceNumber以上は離れることができない)
	const float PlayerMaxDistanceNumber = 20.0f;
	//もう片方のプレイヤーへの力が働く距離
	const float PlayerRevDidtanceNumber = 7.5f;
	const float PlayerMaxDistance = LibMath::calcDistance3D({ PlayerMaxDistanceNumber,0,0 }, { 0,0,0 });//25がmax
	const float PlayerRevDistance = LibMath::calcDistance3D({ PlayerRevDidtanceNumber,0,0 }, { 0,0,0 });//15でrev

#pragma region 紐


	vertex lineVertexHandle;
	heap lineHeapHandle;

	//スプラインの制御点の座標
	Vector3 linePos[9];

	struct LineVertex 
	{
		//座標　法線　敵が振れたことによる移動量
		Vector3 pos, normal, addPos;
	};
	//頂点データ
	std::vector<LineVertex>lineVertexData;

#pragma endregion


public:


	Player( const Vector3& pos, const PlayerType& playerType);
	~Player();

	void update();
	void draw();
	void hit(Object* object, CollosionType collisionType)override;

	void setPosition(Vector3 pos);
};

