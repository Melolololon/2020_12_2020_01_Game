#pragma once
#include"Object.h"
class Boss :public Object
{

private:

	//���C�t
	int life;

	//���G����
	bool isMuteki;
	int mutekiTimer;
	static const int MutekiTime; /*= 60 * 1.5f;*/

	int bossPatternTimer;

	bool isJump;
	Vector3 purePos;

	static const Vector3 syogekihaVel[4][3];

	//��ʗh�炵
	static const Vector3 DefaultRTPos;
	bool yureFlag;
	int yureTimer;
	static const int YureTime;
	Vector3 yureRandNum;

	Vector3 angle;

	int heapNum;
	static const int CreateNum;
	static int createCount;
public:

	//�R���X�g���N�^
	Boss();
	//�f�X�g���N�^
	~Boss();

	static void loadModel();
	//�X�V
	void update() override;
	void bossPattern();
	//�`��
	void draw() override;

	void hit(Object* object, CollisionType collisionType)override;

	void* getPtr()override;

	bool getIsJump();

	bool getLifeZero();
	//life��0�ɂ������ƂɊR�ɗ��������ǂ���
	bool getDropFlag();

	Vector3 getYureRandNum();
};

