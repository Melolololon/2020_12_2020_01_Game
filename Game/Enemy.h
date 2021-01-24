#pragma once
#include "Object.h"
#include"LibMath.h"
#include"Vector.h"

class Enemy : public Object
{
	//�ڕW�^�C�v��enum
	enum TargetType
	{
		LEFT,
		RIGHT
	};

private:
	
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



public:
	//�R���X�g���N�^
	Enemy();
	//�f�X�g���N�^
	~Enemy();
	//������
	void Initialize();
	//�X�V
	void update() override;
	//�`��
	void draw() override;

	void hit(Object* object, CollosionType collisionType)override;

	/// <summary>
	/// ���f���̓ǂݍ���
	/// </summary>
	static void loadModelData();

	/// <summary>
	/// �ړ��ʍX�V
	/// </summary>
	/// <param name="playerPosition">�v���C���[���W</param>
	void UpdateVelocity(Vector3 playerPosition);

	int GetUpdateVelocityTimer() { return updateVelocityTimer; }

	int GetTargetTypeAsInt();

	static Enemy* GetEnemy();

	void* getPtr()override;

	void GetVelocityAndSpeed(Vector3& vel,Vector3& spe);

	void AddPosition(const Vector3 vec);

	void ShotEnemy(const Vector3& vel ,const Vector3& spe);

	bool GetMyShot();

	int GetDamage();
};

