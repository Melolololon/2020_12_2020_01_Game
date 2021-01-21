#pragma once
#include"Object.h"

//�S���Ђ��N���X


class Rubber :public Object
{
private:

	//���ڂ̓_��(����ɂ����W�����߂�)
	int pointNum;
	//�G�l�~�[���R�ɂԂ����Ă�Ƃ��ɐi�ޓG�̐i�s����
	//pointNum1234 6789���������� 5��{0,0,0}
	Vector3 enemyMoveVector;
	
	//Enemy�Ƃǂꂩ1�������������ǂ���
	bool hitEnemy;

	//�S���̌��E�ړ���(�ׂƂ̋���������ȏ�ɂȂ�������E)
	const float MaxMoveDistance = 2.0f;

	//�߂��̃S���������Ƃ��̔{��
	const float MoveBairitu[8] = { 0.8f,0.6f,0.5f,0.4f,0.38f,0.1f,0.07f,0.05f };

	//���݂��A�N�Z�X�ł���
	static Rubber* rubberPtr[9];

	//�K��ʒu(��������ĂȂ��Ƃɂ̍��W)
	Vector3 normalPos;

	//�_�b�V���������ǂ���
	static bool dashFlag;

	//���E�������쎞�Ƀv���C���[���ǂ̂��炢������
	static Vector3 playerMoveVector;


	//0 ���t�g 1 ���C�g
	//�v���C���[����v���C���[�ւ̃x�N�g��(�񐳋K��)
	static Vector3 vecPToP[2];
	static Vector3 playerPos[2];
	static Vector3 playerVelocity[2];
	static Vector3 playerSpeed[2];
	static bool playerInputFlag[2];

	//���~�b�g���ǂ���
	static int rimitCount;
	static bool allRimitFlag;

	static bool leavePlayerFlag;

	//�_�b�V�����I�������ǂ���
	static bool playerDashEnd;
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

	static void setPlayerPosition(const Vector3& pos, const int& pNum);
	static void setPlayerToPlayerVector(const Vector3& vector, const int& pNum);

	static void setRubberPtr( Rubber* rP[]);

	void addPosition(const Vector3& vec);
	void setHitEnemy(const bool& flag);

	static void setDashFlag(const bool& flag);

	static Vector3 getPlayerMoveVector();

	static void setPlayerVectorAndSpeed(const Vector3& vel, const Vector3& spe,const int& pNum);
	static void resetRimitCount();
	static void setPlayerInputFlag(const bool& flag, const int& pNum);
	static void setLeavePlayerFlag(const bool& flag);
};

