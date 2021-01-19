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


	position = playerPos + vecPToP * (pointNum * 0.1);
	normalPos = position;
}

Rubber::~Rubber()
{
}

void Rubber::update()
{
	//規定位置を取得
	normalPos = playerPos + vecPToP * (pointNum * 0.1);

	//当たってなかったら戻るように
	if (!hitEnemy) 
	{
		//自分の座標から規定位置のベクトル
		Vector3 myPosToNorPos = LibMath::otherVector(position, normalPos);

		float dis = LibMath::calcDistance3D(position, normalPos);
		//一定以上離れてたら元の位置に向かって移動
		if (dis >= 0.45f)
		{
			position += myPosToNorPos * speed;
		}
		else
			position = normalPos;
	}
	//position = normalPos;

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
	//敵は紐の中心によるように調整

	if (typeid(*object) == typeid(Enemy)) 
	{
		Enemy* e = static_cast<Enemy*>(object->getPtr());
		
		//発射されてたら判定無視
		if (e->GetMyShot())return;

		hitEnemy = true;

		//敵のvelocityと速度を取得
		
		//敵のベクトル
		Vector3 eVel;
		Vector3 eSpe;
		e->GetVelocityAndSpeed(eVel, eSpe);

		//ゴムとプレイヤーを動かす
#pragma region ゴム
		//限界だったら移動しない

		//動くかどうかフラグ
		bool moveFlag = true;
		//配列番号
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

		//一定距離あったらゴムを動かさない
		if (LibMath::calcDistance3D(normalPos, position) >= MaxMoveDistance[maxMoveDistanceMoveNum])
			moveFlag = false;
		
		//ゴムが動かない場合、敵を固定
		if (!moveFlag)
			e->SetStopFlag(true);


		//移動
		if (moveFlag) 
		{
			Vector3 moveVec = eVel * eSpe;
			position += moveVec;
			Library::setPosition(position, heapHandle, 0);
			sphereData[0].position = position;

			//近くの点も動かす
			int arrNum = 0;
			for (int i = pointNum + 1; i < 9; i++) 
			{
				moveVec = eVel * eSpe * MoveBairitu[arrNum];
				rubberPtr[i]->movePosition(moveVec);
				rubberPtr[i]->setHitEnemy(true);
				arrNum++;
			}
			arrNum = 0;
			for (int i = pointNum - 1; i > -1; i--)
			{
				moveVec = eVel * eSpe * MoveBairitu[arrNum];
				rubberPtr[i]->movePosition(moveVec);
				rubberPtr[i]->setHitEnemy(true);
				arrNum++;
			}

			//固定しない
			e->SetStopFlag(false);
		}
#pragma endregion

		//プレイヤー
#pragma region プレイヤー

#pragma endregion



		//プレイヤーのがダッシュしたら敵を動かす(吹っ飛ばす)
		//既定の位置(当たってないときの座標)よりどのくらい離れているかで威力を変える
		if (dashFlag) 
		{
			e->SetVelocity(normalPos - position);
		}
		
		//紐の後ろ(引っ張られてるほうにいる敵)にいる敵は吹っ飛ばないように対策する 
	    //引っ張り具合に応じて、敵遅くする

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
