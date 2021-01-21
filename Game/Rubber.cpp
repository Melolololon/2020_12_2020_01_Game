#include "Rubber.h"
#include"Enemy.h"

Rubber* Rubber::rubberPtr[9];
bool Rubber::dashFlag;
Vector3 Rubber::playerMoveVector;

//0 ���t�g 1 ���C�g
Vector3 Rubber::vecPToP[2];
Vector3 Rubber::playerPos[2];
Vector3 Rubber::playerVelocity[2];
Vector3 Rubber::playerSpeed[2];
bool Rubber::playerInputFlag[2];

int Rubber::rimitCount;
bool Rubber::allRimitFlag; 

bool Rubber::leavePlayerFlag;

Rubber::Rubber(const int& pNum)
{
	pointNum = pNum;

	velocity = 0;
	speed = 1.0f;
	isDead = false;

	collisionFlag.board = false;
	collisionFlag.lay = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = true;
	collisionFlag.plane = false;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 1.0f;

	Library::createManyVertex3DBox({ 1,1,1 }, &vertexHandle);
	Library::createHeapData2({ 255,255,255,255 }, 1, &heapHandle);

	enemyMoveVector = 0;
	hitEnemy = false;


	position = playerPos[0] + vecPToP[0] * (pointNum * 0.1);
	normalPos = position;
	
}

Rubber::~Rubber()
{
}

void Rubber::update()
{
	//���Z�b�g
	//static�Ȃ̂ɂȂ��������������BspTest�Ŏ���
	playerMoveVector = 0;

	//�K��ʒu���擾
	normalPos = playerPos[0] + vecPToP[0] * (pointNum * 0.1);

	//�������ĂȂ�������߂�悤��
	//�����̍��W����K��ʒu�̃x�N�g��
	Vector3 myPosToNorPos = LibMath::otherVector(position, normalPos);
	if (!hitEnemy) 
	{
		float dis = LibMath::calcDistance3D(position, normalPos);
		//���ȏ㗣��Ă��猳�̈ʒu�Ɍ������Ĉړ�
		if (dis >= 0.45f)
		{
			position += myPosToNorPos * speed;
		}
		else
			position = normalPos;
	}

	//���͂��ĂȂ����A�v���C���[�����ȏ㗣��Ă�����B�v���C���[�Ɠ����悤�Ɉړ�
	Vector3 nVecPToP;
	if (pointNum <= 5 && !playerInputFlag[0] && leavePlayerFlag)
	{ 
		nVecPToP = Vector3::normalize(vecPToP[0]);
		position += nVecPToP * 0.15f;
	}
	if (pointNum >= 5 && !playerInputFlag[1] && leavePlayerFlag)
	{
		nVecPToP = Vector3::normalize(vecPToP[1]);
		position += nVecPToP * 0.15f;
	}

	sphereData[0].position = position;
	Library::setPosition(position, heapHandle, 0);
	
	hitEnemy = false;




	//�����𑪂邽�߂̊�ƂȂ���W
	Vector3 v;
	//v�ɑ��
	if (pointNum == 1) v = playerPos[0];
	if (pointNum == 2) v = rubberPtr[0]->getSphereData()[0].position;
	if (pointNum == 3) v = rubberPtr[1]->getSphereData()[0].position;
	if (pointNum == 4) v = rubberPtr[2]->getSphereData()[0].position;
	if (pointNum == 5) v = rubberPtr[3]->getSphereData()[0].position;
	if (pointNum == 6) v = rubberPtr[6]->getSphereData()[0].position;;
	if (pointNum == 7) v = rubberPtr[7]->getSphereData()[0].position;;
	if (pointNum == 8) v = rubberPtr[8]->getSphereData()[0].position;;
	if (pointNum == 9) v = playerPos[1];

	//��苗������������E
	float rubberDis = LibMath::calcDistance3D(v, position);
	
	if(pointNum != 1 && pointNum != 9)
	{
		if (rubberDis >= MaxMoveDistance)
		{
			allRimitFlag = true;
		}
	}

	if (pointNum == 1 || pointNum == 9) 
	{
		if (rubberDis  >= MaxMoveDistance + 1.0f)
		{
			allRimitFlag = true;
		}
	}

	if (rimitCount == 9)allRimitFlag = true;

	
}

void Rubber::draw()
{
	Library::drawGraphic(vertexHandle, heapHandle, 0);
}

void Rubber::hit(Object* object, CollosionType collisionType)
{
	//��邱��
	//1.�G���R�̒��S�ɂ��悤�ɒ���
	//1.���� �x�N�g���Œ肾�ƃv���C���[�̈ʒu���t�ɂȂ������ɔ��Α��s�����Ⴄ����΍�
	//1.���� pointNum5�ւ̃x�N�g�������߂āA�������Ɉړ�������?�B(Enemy�̑��x�����Ȃ�)
	//1.���� ���x�͖��Ȃ������B���΂߂̎��ɓ˂�����ł��^�񒆍s���悤�ɂ���?

	//3.�R�̌��(���������Ă�ق��ɂ���G)�ɂ���G�͐�����΂Ȃ��悤�ɑ΍􂷂� (���ł����Ȃ��̂ł�? ���������Ă�킯���������I�ɖ��Ȃ��Ǝv����)
	//3.�����@�������A�K��ʒu����ǂ̂��炢�ړ��������Ŕ��f���邽�߁A�߂����ᐁ�����
	//3.���� �S���ƓG�̈ʒu���K��ʒu�ւ̃x�N�g���Ƒ�̋t���������΂��Ȃ���΂���?
	
	//5.�G�������������Ăăv���C���[�������ĂȂ��Ƃ��́A�����悤�ɂ���(�v���C���[�����������A�S������������)
	//5.���� rimit��true�ɂȂ�����false�ɂȂ�������J��Ԃ��āAMoveBairitu[arrNum]���|�����邩��v���C���[�Ɠ����悤�ɓ����Ȃ�����C��
	//5.���� ���̂܂܂ł��債�ĉe���Ȃ����������炱�̂܂܂ł���?
	//5.���� ���������ĉ����߂���hit���ɏ����Ă邩�炿���ƃv���C���[�݂����Ɉړ����Ȃ�?
	//5.���� addPosition�Ă�ł邩�炻��Ȃ��ƂȂ�
	
	

	if (typeid(*object) == typeid(Enemy)) 
	{
		//�ǂꂩ�����������A����
		if (hitEnemy)return;

		Enemy* e = static_cast<Enemy*>(object->getPtr());
		
		//���˂���Ă��画�薳��
		if (e->GetMyShot())return;

		for (int i = 0; i < 9; i++) 
		{
			rubberPtr[i]->setHitEnemy(true);
		}

		//�G��velocity�Ƒ��x���擾
		
		//�G�̃x�N�g��
		Vector3 eVel;
		Vector3 eSpe;
		e->GetVelocityAndSpeed(eVel, eSpe);

		//�G�𒆐S�Ɋ񂹂�
		if (pointNum != 5) 
		{
			enemyMoveVector = LibMath::otherVector(position, rubberPtr[4]->getSphereData()[0].position);
			e->AddPosition(enemyMoveVector * 0.05f);
		}

		//�S���ƃv���C���[�𓮂���
#pragma region �S��

		//rimit��true�ɂȂ�����false�ɂȂ�������J��Ԃ��āA
		//MoveBairitu[arrNum]���|�����邩��v���C���[�Ɠ����悤�ɓ����Ȃ�
		//�v���C���[��ǂ��z�����Ƃ͂Ȃ����炱�̂܂܂ł����Ȃ�?
		if (allRimitFlag)
		{
			//�G�𓮂��Ȃ�����
			//moveFlag��true��false���J��Ԃ�?�̂ŁA�������l�̔������炢�������f����ĂȂ�
			e->AddPosition(eVel*eSpe * -1);

			//�v���C���[�𓮂����x�N�g���Z�b�g
			playerMoveVector = eVel * eSpe;

			//�v���C���[�ƂƂ��ɃS��������
			Vector3 moveVec = playerMoveVector;
			position += moveVec;
			Library::setPosition(position, heapHandle, 0);
			sphereData[0].position = position;

			int arrNum = 0;
			for (int i = pointNum; i < 9; i++)
			{
				rubberPtr[i]->addPosition(moveVec  );
				arrNum++;
			}
			arrNum = 0;
			for (int i = pointNum - 2; i > -1; i--)
			{
				rubberPtr[i]->addPosition(moveVec);
				arrNum++;
			}

			//���E�̎��Ƀv���C���[�������������������
			Vector3 pMoveSpeed = { 0,0,0 };
			for (int i = 0; i < _countof(rubberPtr); i++) 
			{
				pMoveSpeed = playerVelocity[0] * playerSpeed[0];
				rubberPtr[i]->addPosition({ pMoveSpeed .x/2,pMoveSpeed .y/2,pMoveSpeed .z/2});
				pMoveSpeed = playerVelocity[1] * playerSpeed[1];
				rubberPtr[i]->addPosition({ pMoveSpeed .x/2,pMoveSpeed .y/2,pMoveSpeed .z/2});
			}
			//�G����������
			pMoveSpeed = playerVelocity[0] * playerSpeed[0];
			e->AddPosition(pMoveSpeed);
			pMoveSpeed = playerVelocity[1] * playerSpeed[1];
			e->AddPosition(pMoveSpeed);
		}

		//�ړ�
		if (!allRimitFlag) 
		{
			Vector3 moveVec = eVel * eSpe;
			position += moveVec;
			Library::setPosition(position, heapHandle, 0);
			sphereData[0].position = position;

			//���̓_��������
			int arrNum = 0;
			for (int i = pointNum; i < 9; i++) 
			{
				moveVec = eVel * eSpe * MoveBairitu[arrNum];
				rubberPtr[i]->addPosition(moveVec);
				arrNum++;
			}
			arrNum = 0;
			for (int i = pointNum - 2; i > -1; i--)
			{
				moveVec = eVel * eSpe * MoveBairitu[arrNum];
				rubberPtr[i]->addPosition(moveVec);
				arrNum++;
			}

			playerMoveVector = 0;
		}

		allRimitFlag = false;
#pragma endregion

		//�v���C���[�̂��_�b�V��������G�𓮂���(������΂�)
		//����̈ʒu(�������ĂȂ��Ƃ��̍��W)���ǂ̂��炢����Ă��邩�ňЗ͂�ς���
		if (dashFlag) 
		{
			e->ShotEnemy(normalPos - position);
		}
		
	}

}

void Rubber::setPlayerPosition(const Vector3& pos, const int& pNum)
{
	playerPos[pNum - 1] = pos;
}

void Rubber::setPlayerToPlayerVector(const Vector3& vector, const int& pNum)
{
	vecPToP[pNum - 1] = vector;
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

void Rubber::addPosition(const Vector3& vec)
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

Vector3 Rubber::getPlayerMoveVector()
{
	return playerMoveVector;
}

void Rubber::setPlayerVectorAndSpeed(const Vector3& vel, const Vector3& spe, const int& pNum)
{
	playerVelocity[pNum - 1] = vel;
	playerSpeed[pNum - 1] = spe;
	
	
}

void Rubber::resetRimitCount()
{
	rimitCount = 0;
}

void Rubber::setPlayerInputFlag(const bool& flag, const int& pNum)
{
	playerInputFlag[pNum - 1] = flag;
}

void Rubber::setLeavePlayerFlag(const bool& flag)
{
	leavePlayerFlag = flag;
}