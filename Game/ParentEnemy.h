#pragma once
#include "Object.h"
#include "Enemy.h"
#include <vector>

class ParentEnemy : public Object
{

private:

	//���C�t
	int life;

	//���G����
	bool isMuteki;
	int mutekiTimer;
	const int MutekiTime = 60 * 0.2f;

	//�G�ł��o���Ɏg������
	int generateEnemyTimer = 0;
	const int GENERATETIME = 60 * 3;                                                           

	//���C�t
	int Life = 5;

public:

	//�R���X�g���N�^
	ParentEnemy();
	//�f�X�g���N�^
	~ParentEnemy();
	//������
	void Initialize();
	//�X�V
	void update() override;
	//�`��
	void draw() override;

	void hit(Object* object, CollosionType collisionType)override;

	void* getPtr()override;

	void ShotEnemy();

};

