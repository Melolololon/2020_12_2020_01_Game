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
	const int MutekiTime = 60 * 1.5f;

	//�G�ł��o���Ɏg������
	int generateEnemyTimer = 0;
	const int GENERATETIME = 60 * 3;                                                           

	static int deadCount;
public:

	//�R���X�g���N�^
	ParentEnemy(const Vector3& pos);
	//�f�X�g���N�^
	~ParentEnemy();
	//������
	void Initialize();
	//�X�V
	void update() override;
	//�`��
	void draw() override;

	void hit(Object* object, CollisionType collisionType)override;

	void* getPtr()override;

	void ShotEnemy();


	static int GetDeadCount();
	static void ResetDeadCount();
};

