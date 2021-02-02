#pragma once
#include "Object.h"
#include"LibMath.h"
#include"Vector.h"

class Enemy : public Object
{
public:
	//�ڕW�^�C�v��enum
	enum TargetType
	{
		LEFT,
		RIGHT
	};

	enum EnemyType
	{
		PLAYER_TARGET,
		PLAYER_TUIBI,
		SET_VELOCITY,
		BOSS2_PUNCH
	};

private:
	
	//�����̃^�C�v
	EnemyType enemyType;

	//�ڕW�^�C�v
	TargetType targetType;
	//�ړ��ʍX�V�Ɏg�p����^�C�}�[
	int updateVelocityTimer = 0;

	//���������˂���Ă邩�ǂ���
	bool myShot;

	//���C�t
	int life;

	//���G����
	bool isMuteki;
	int mutekiTimer;
	const int MutekiTime = 60 * 0.2f;

	static Vector3 playerPosition[2];

	//�ړ��ʂ��v���C���[�̌����ɍX�V����Ԋu
//�ɒ[�ɏ��Ȃ��ƃN���A�s�\�ɂȂ�
	const int UPDATE_VELOCITY_TIME = 30;
	const int ADD_ENEMY_TIME = 180;

	int tuibiEndTimer;
	static const int TuibiEndTime;

	int heapNum;
	static const int CreateNum;
	static int createCount;


	static const int PunchCreateNum;
	static int punchCreateCount;
public:
	//�R���X�g���N�^
	Enemy(const Vector3& pos,const EnemyType& enemyType);
	//�R���X�g���N�^
	Enemy(const Vector3& pos,const Vector3& vel);
	//�f�X�g���N�^
	~Enemy();
	
	static void loadModel();
	
	//������
	void Initialize();
	//�X�V
	void update() override;
	//�`��
	void draw() override;

	void hit(Object* object, CollisionType collisionType)override;


	
	void UpdateVelocity();

	//int GetUpdateVelocityTimer() { return updateVelocityTimer; }

	int GetTargetTypeAsInt();

	//static Enemy* GetEnemy();

	static void SetPlayerPos(const Vector3& pos,const int& playerType);

	void* getPtr()override;

	void GetVelocityAndSpeed(Vector3& vel,Vector3& spe);

	void SetPosition(const Vector3& pos);
	void AddPosition(const Vector3& vec);

	void ShotEnemy(const Vector3& vel ,const Vector3& spe);

	bool GetMyShot();

	int GetDamage();


};

