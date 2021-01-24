#pragma once
#include"Scene.h"
#include"Library.h"
#include"LibMath.h"
#include"Vector.h"
#include"Player.h"
#include"Enemy.h"
#include<vector>
class Play :public Scene
{
private:
	Player* player[2];
	std::vector<Enemy*> enemies;

	//�ړ��ʂ��v���C���[�̌����ɍX�V����Ԋu
	//�ɒ[�ɏ��Ȃ��ƃN���A�s�\�ɂȂ�
	const int UPDATE_VELOCITY_TIME = 90;
	const int ADD_ENEMY_TIME = 180;

	int addEnemyTimer = 0;
	
	bool pauseFlag;

	vertex fierdVertexH;
	heap fierdHeapH;
public:
	Play();
	~Play();

	void initialize();
	void update();
	void draw();
	void end();
	std::string nextScene();

};

