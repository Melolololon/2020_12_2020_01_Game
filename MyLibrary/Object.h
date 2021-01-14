#pragma once
#include<typeinfo>
#include<vector>
#include"Library.h"
#include"Vector.h"
#include"CollisionType.h"

//�I�u�W�F�N�g�}�l�[�W���[�ǉ����ɔ����I�Ԃ悤�ɂ���?(���育�Ƃɒǉ��֐��Ɣz����)

class Object
{
public:
	//����
	struct CollisionFlag
	{
		bool sphere;
		bool lay;
		bool lineSegment;
		bool plane;
		bool board;
	};

protected:
	//���W
	Vector3 position;
	//����
	Vector3 velocity;
	//�ړ����x
	Vector3 speed;

	//�����t���O 
	bool isDead;

	//���_�f�[�^���Ăяo������
	vertex vertexHandle;
	//�q�[�v�f�[�^���Ăяo������
	heap heapHandle;
	//�q�[�v�̂ǂ��ɂ��邩��\���ԍ�
	int heapNumber;

#pragma region ����f�[�^
	std::vector<SphereData> sphereData;
	std::vector<LineSegmentData> lineSegmentData;
	std::vector<LayData> layData;
	std::vector<PlaneData>planeData;
	std::vector<BoardData>boardData;
#pragma endregion

	CollisionFlag collisionFlag;
public:

	//�R���X�g���N�^
	Object();
	//�f�X�g���N�^
	virtual ~Object();
	//����
	virtual void update() = 0;
	//�`�揈��
	virtual void draw() = 0;
	//���菈��
	virtual void hit(Object* object, CollosionType collisionType);

	bool getIsDead();
	CollisionFlag getCollisionFlag();
	std::vector<SphereData> getSphereData();
	std::vector<LineSegmentData> getLineSegmentData();
	std::vector<PlaneData> getPlaneData();
	std::vector<BoardData> getBoardData();

	virtual void* getPtr();

	
};