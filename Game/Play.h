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

	//�ړ��ʂ��v���C���[�̌����ɍX�V����Ԋu
	//�ɒ[�ɏ��Ȃ��ƃN���A�s�\�ɂȂ�
	//Enemy.h�Ɉړ�
	//const int UPDATE_VELOCITY_TIME = 90;
	//const int ADD_ENEMY_TIME = 180;

	int addEnemyTimer = 0;
	
	bool pauseFlag;

#pragma region ���f��

	vertex fierdVertexH;
	heap fierdHeapH;
#pragma endregion

#pragma region �X�v���C�g
	sprite pLifeSpr[2];
	texture pLifeTex;

	//HP�̕���
	sprite hpSpr[2];
	texture hpTex;
#pragma endregion


	int sceneChangeTimer;
	static const int SceneChangeTime;

public:
	Play();
	~Play();

	void initialize();
	void update();
	void draw();
	void end();
	std::string nextScene();

};

