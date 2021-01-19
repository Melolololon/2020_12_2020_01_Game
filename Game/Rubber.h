#pragma once
#include"Object.h"
//�S���Ђ��N���X
class Rubber :public Object
{
private:
	//�v���C���[����v���C���[�ւ̃x�N�g��(�񐳋K��)
	static Vector3 vecPToP;
	static Vector3 playerPos;//�ǂ������̃v���C���[�̍��W(�ǂ���ł�����)

	//���ڂ̓_��(����ɂ����W�����߂�)
	int pointNum;
	//�G�l�~�[���R�ɂԂ����Ă�Ƃ��ɐi�ޓG�̐i�s����
	//pointNum1234 6789���������� 5��{0,0,0}
	Vector3 enemyMoveVector;
	
	//Enemy�Ƃǂꂩ1�������������ǂ���
	bool hitEnemy;

	//�R�̌��E�ړ���
	const float MaxMoveDistance[5] = {2.0f,6.0f,12.0f,16.0f,20.0f};

	//�߂��̃S���������Ƃ��̔{��
	const float MoveBairitu[8] = { 0.8f,0.7f,0.6f,0.5f,0.4f,0.3f,0.2f,0.1f };

	//���݂��A�N�Z�X�ł���
	static Rubber* rubberPtr[9];

	//�K��ʒu(��������ĂȂ��Ƃɂ̍��W)
	Vector3 normalPos;

	static bool dashFlag;
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="pNum">���ڂ̓_��(����ɂ����W�����߂�)</param>
	Rubber(const int& pNum);
	~Rubber();

	void update();
	void draw();
	void hit(Object* object, CollosionType collisionType)override;

	static void setPlayerPosition(const Vector3& pos);
	static void setPlayerToPlayerVector(const Vector3& vector);

	static void setRubberPtr( Rubber* rP[]);

	void movePosition(const Vector3& vec);
	void setHitEnemy(const bool& flag);

	static void setDashFlag(const bool& flag);
};

