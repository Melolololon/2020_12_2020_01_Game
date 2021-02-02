#pragma once
#include "Object.h"
#include "Enemy.h"
#include "Vector.h"
#include <vector>

class ParentEnemy : public Object
{
public:
	enum PEnemyType
	{
		PENEMY_TARGET,
		PENEMT_BARAMAKI,
	};

private:

	//���C�t
	int life;

	//���G����
	bool isMuteki;
	int mutekiTimer;
	const int MutekiTime = 60 * 1.5f;

	//�G�ł��o���Ɏg������
	int generateEnemyTimer;
	int generateTime;

	static int deadCount;

	PEnemyType pEnemyType;
	float angle;

	static Vector3 playerPos[2];

	static const int CreateNum;
	static  int createCount;
	int heapNum;
public:

	//�R���X�g���N�^
	ParentEnemy(const Vector3& pos, const PEnemyType& type);
	ParentEnemy(const Vector3& pos, const float& defAngle);
	//�f�X�g���N�^
	~ParentEnemy();

	static void loadModel();

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

	static void SetPlayerPosition(const Vector3& pos,const int& type);
};

