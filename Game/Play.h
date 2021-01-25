#pragma once
#include"Scene.h"
#include"Library.h"
#include"LibMath.h"
#include"Vector.h"
#include"Player.h"
#include"Enemy.h"
#include "ParentEnemy.h"
#include<vector>
class Play :public Scene
{
private:
	Player* player[2];
	std::vector<Enemy*> enemies;
	ParentEnemy* parentEnemy;

	//移動量をプレイヤーの向きに更新する間隔
	//極端に少ないとクリア不可能になる
	//Enemy.hに移動
	//const int UPDATE_VELOCITY_TIME = 90;
	//const int ADD_ENEMY_TIME = 180;

	int addEnemyTimer = 0;
	
	bool pauseFlag;

#pragma region モデル

	vertex fierdVertexH;
	heap fierdHeapH;
#pragma endregion

#pragma region スプライト
	sprite pLifeSpr[2];
	texture pLifeTex;

	//HPの文字
	sprite hpSpr[2];
	texture hpTex;
#pragma endregion

public:
	Play();
	~Play();

	void initialize();
	void update();
	void draw();
	void end();
	std::string nextScene();

};

