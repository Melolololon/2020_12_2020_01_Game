#include "Rubber.h"
Vector3 Rubber::vecPToP;
Vector3 Rubber::playerPos;

Rubber::Rubber(const int& pNum)
{
	pointNum = pNum;

	position = 0;
	velocity = 0;
	speed = 0;
	isDead = false;

	collisionFlag.board = false;
	collisionFlag.lay = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = true;
	collisionFlag.plane = false;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 0.2f;

	Library::createManyVertex3DBox({ 1,1,1 }, &vertexHandle);
	Library::createHeapData2({ 255,255,255,255 }, 1, &heapHandle);

	enemyMoveVector = 0;
}

Rubber::~Rubber()
{
}

void Rubber::update()
{
	position = playerPos + vecPToP * (pointNum * 0.1);

	sphereData[0].position = position;
	Library::setPosition(position, heapHandle, 0);

	
}

void Rubber::draw()
{
	Library::drawGraphic(vertexHandle, heapHandle, 0);
}

void Rubber::hit(Object* object, CollosionType collisionType)
{
	//�G�͕R�̒��S�ɂ��悤�ɒ���

	//if (typeid(*object) == typeid(Enemy)) 
	{
		//�G��velocity�Ƒ��x���擾
		
		//�G�̃x�N�g��
		Vector3 eVec;
		
		//�S���ƃv���C���[�𓮂���

		//�v���C���[�̂��_�b�V��������G�𓮂���(������΂�)
		//����̈ʒu(�������ĂȂ��Ƃ��̍��W)���ǂ̂��炢����Ă��邩�ňЗ͂�ς���
		
		//�R�̌��(���������Ă�ق��ɂ���G)�ɂ���G�͐�����΂Ȃ��悤�ɑ΍􂷂� 
	}

}

void Rubber::setPlayerPosition(const Vector3& pos)
{
	playerPos = pos;
}

void Rubber::setPlayerToPlayerVector(const Vector3& vector) 
{
	vecPToP = vector;
}

