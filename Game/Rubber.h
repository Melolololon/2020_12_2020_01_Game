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
	//�G�l�~�[���R�ɂԂ����Ă�Ƃ��ɐi�ޕ���
	//pointNum1234 6789���������� 5��{0,0,0}
	Vector3 enemyMoveVector;

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
};

