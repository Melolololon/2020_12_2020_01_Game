#include "Rubber.h"
#include"Enemy.h"

Vector3 Rubber::vecPToP;
Vector3 Rubber::playerPos;
Rubber* Rubber::rubberPtr[9];
bool Rubber::dashFlag;

Rubber::Rubber(const int& pNum)
{
	pointNum = pNum;

	velocity = 0;
	speed = 0.5f;
	isDead = false;

	collisionFlag.board = false;
	collisionFlag.lay = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = true;
	collisionFlag.plane = false;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 0.15f;

	Library::createManyVertex3DBox({ 1,1,1 }, &vertexHandle);
	Library::createHeapData2({ 255,255,255,255 }, 1, &heapHandle);

	enemyMoveVector = 0;
	hitEnemy = false;

	normalPos = 0;

	position = playerPos + vecPToP * (pointNum * 0.1);
}

Rubber::~Rubber()
{
}

void Rubber::update()
{
	normalPos = playerPos + vecPToP * (pointNum * 0.1);

	//�������ĂȂ�������߂�悤��
	if (!hitEnemy) 
	{
		//�����̍��W����K��ʒu�̃x�N�g��
		/*Vector3 myPosToNorPos = LibMath::otherVector(position, normalPos);

		if(LibMath::calcDistance3D(position, normalPos) >= 1.0f)
		position += myPosToNorPos * speed;*/


		position = normalPos;
	}

	sphereData[0].position = position;
	Library::setPosition(position, heapHandle, 0);
	
	hitEnemy = false;
}

void Rubber::draw()
{
	Library::drawGraphic(vertexHandle, heapHandle, 0);
}

void Rubber::hit(Object* object, CollosionType collisionType)
{
	//�G�͕R�̒��S�ɂ��悤�ɒ���

	if (typeid(*object) == typeid(Enemy)) 
	{
		Enemy* e = static_cast<Enemy*>(object->getPtr());
		
		//���˂���Ă��画�薳��
		if (e->GetMyShot())return;

		hitEnemy = true;

		//�G��velocity�Ƒ��x���擾
		
		//�G�̃x�N�g��
		Vector3 eVel;
		Vector3 eSpe;
		e->GetVelocityAndSpeed(eVel, eSpe);

		//�S���ƃv���C���[�𓮂���
#pragma region �S��
		//���E��������ړ����Ȃ�

		//�������ǂ����t���O
		bool moveFlag = true;
		//�z��ԍ�
		int maxMoveDistanceMoveNum = 0;
		switch (pointNum)
		{
		case 1:
		case 9:
			maxMoveDistanceMoveNum = 0;
			break;

		case 2:
		case 8:
			maxMoveDistanceMoveNum = 1;
			break;
		case 3:
		case 7:
			maxMoveDistanceMoveNum = 2;
			break;
		case 4:
		case 6:
			maxMoveDistanceMoveNum = 3;
			break;

		case 5:
			maxMoveDistanceMoveNum = 4;
			break;
		}

		//��苗����������S���𓮂����Ȃ�
		if (LibMath::calcDistance3D(normalPos, position) >= MaxMoveDistance[maxMoveDistanceMoveNum])
			moveFlag = false;
		
		//�S���������Ȃ��ꍇ�A�G���Œ�
		if (!moveFlag)
			e->SetStopFlag(true);

		//�ړ�
		if (moveFlag) 
		{
			position += eVel * eSpe;
			Library::setPosition(position, heapHandle, 0);
			sphereData[0].position = position;

			//�߂��̓_��������
		/*	for (int i = pointNum + 1; i < 9; i++) 
			{
				rubberPtr[i]->movePosition(eVel * eSpe * MoveBairitu[i - 1 - pointNum]);
				rubberPtr[i]->setHitEnemy(true);
			}
			for (int i = pointNum - 1; i > -1; i--)
			{
				rubberPtr[i]->movePosition(eVel * eSpe * MoveBairitu[i + 1 + pointNum]);
				rubberPtr[i]->setHitEnemy(true);
			}*/

			//�Œ肵�Ȃ�
			e->SetStopFlag(false);
		}
#pragma endregion

		//�v���C���[
		


		//�v���C���[�̂��_�b�V��������G�𓮂���(������΂�)
		//����̈ʒu(�������ĂȂ��Ƃ��̍��W)���ǂ̂��炢����Ă��邩�ňЗ͂�ς���
		if (dashFlag) 
		{
			e->SetVelocity(normalPos - position);
		}
		
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

void Rubber::setRubberPtr( Rubber* rP[])
{
	rubberPtr[0] = rP[0];
	rubberPtr[1] = rP[1];
	rubberPtr[2] = rP[2];
	rubberPtr[3] = rP[3];
	rubberPtr[4] = rP[4];
	rubberPtr[5] = rP[5];
	rubberPtr[6] = rP[6];
	rubberPtr[7] = rP[7];
	rubberPtr[8] = rP[8];
	rubberPtr[9] = rP[9];
}

void Rubber::movePosition(const Vector3& vec)
{
	position += vec;
	sphereData[0].position = position;
	Library::setPosition(position, heapHandle, 0);
}

void Rubber::setHitEnemy(const bool& flag)
{
	hitEnemy = flag;
}

void Rubber::setDashFlag(const bool& flag)
{
	dashFlag = flag;
}
