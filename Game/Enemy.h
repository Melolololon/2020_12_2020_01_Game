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

	//�R�Ɉ����������Ď~�܂邩�ǂ���
	bool stopFlag;

	//���������˂���Ă邩�ǂ���
	bool myShot;
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

	void SetStopFlag(const bool& flag);

	void SetVelocity(const Vector3& vec);

	bool GetMyShot();
};

