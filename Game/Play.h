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

	enum GameState
	{
		GAME_STATE_NORMAL,
		GAME_STATE_GAMEOVER,
		GAME_STATE_CLEAR,
	};
	GameState gameState;

	Player* player[2];
	std::vector<Enemy*> enemies;

	//移動量をプレイヤーの向きに更新する間隔
	//極端に少ないとクリア不可能になる
	//Enemy.hに移動
	//const int UPDATE_VELOCITY_TIME = 90;
	//const int ADD_ENEMY_TIME = 180;

	int addEnemyTimer = 0;
	
	bool pauseFlag;


#pragma region スプライト
	sprite pLifeSpr[2];
	texture pLifeTex;

	//HPの文字
	sprite hpSpr[2];
	texture hpTex;

	//クリアゲームオーバー

#pragma endregion

	

	int sceneChangeTimer;
	static const int SceneChangeTime;

	Rubber* rP[9];
public:
	Play();
	~Play();

	void initialize();
	void update();
	void draw();
	void end();
	std::string nextScene();

};

