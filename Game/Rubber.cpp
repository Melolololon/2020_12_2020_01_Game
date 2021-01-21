#include "Rubber.h"
#include"Enemy.h"

Rubber* Rubber::rubberPtr[9];
bool Rubber::dashFlag;
Vector3 Rubber::playerMoveVector;

//0 レフト 1 ライト
Vector3 Rubber::vecPToP[2];
Vector3 Rubber::playerPos[2];
Vector3 Rubber::playerVelocity[2];
Vector3 Rubber::playerSpeed[2];
bool Rubber::playerInputFlag[2];

int Rubber::rimitCount;
bool Rubber::allRimitFlag; 

bool Rubber::leavePlayerFlag; 
bool Rubber::playerDashEnd;

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
	sphereData[0].r = 0.4f;

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
	//リセット
	//staticなのになぜか初期化されん。spTestで試す
	playerMoveVector = 0;

	//規定位置を取得
	normalPos = playerPos[0] + vecPToP[0] * (pointNum * 0.1);

	//当たってなかったら戻るように
	//自分の座標から規定位置のベクトル
	Vector3 myPosToNorPos = LibMath::otherVector(position, normalPos);
	if (!hitEnemy) 
	{
		float dis = LibMath::calcDistance3D(position, normalPos);
		//一定以上離れてたら元の位置に向かって移動
		if (dis >= 0.45f)
		{
			position += myPosToNorPos * speed;
		}
		else
			position = normalPos;
	}

	//入力してないかつ、プレイヤーが一定以上離れていたら。プレイヤーと同じように移動
	//(離れてるときに敵にぶつかって、引っ張るのやめてもついてくるように)
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




	//距離を測るための基準となる座標
	Vector3 v;
	//vに代入
	if (pointNum == 1) v = playerPos[0];
	if (pointNum == 2) v = rubberPtr[0]->getSphereData()[0].position;
	if (pointNum == 3) v = rubberPtr[1]->getSphereData()[0].position;
	if (pointNum == 4) v = rubberPtr[2]->getSphereData()[0].position;
	if (pointNum == 5) v = rubberPtr[3]->getSphereData()[0].position;
	if (pointNum == 6) v = rubberPtr[6]->getSphereData()[0].position;;
	if (pointNum == 7) v = rubberPtr[7]->getSphereData()[0].position;;
	if (pointNum == 8) v = rubberPtr[8]->getSphereData()[0].position;;
	if (pointNum == 9) v = playerPos[1];

	//一定距離あったら限界
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
		if (rubberDis  >= MaxMoveDistance + 2.0f)
		{
			allRimitFlag = true;
		}
	}

	if (rimitCount == 9)allRimitFlag = true;

	//地下かったら特定の点のサイズを変える
	if (LibMath::calcDistance3D(playerPos[0], playerPos[1]) < 10.0f) 
	{
		if (pointNum == 4 ||
			pointNum == 5 ||
			pointNum == 6) 
		{
			sphereData[0].r = 0.01f;
		}
	}

	
}

void Rubber::draw()
{
	Library::drawGraphic(vertexHandle, heapHandle, 0);
}

void Rubber::hit(Object* object, CollosionType collisionType)
{
	//やること
	//1.敵が紐の中心によるように調整
	//1.続き ベクトル固定だとプレイヤーの位置が逆になった時に反対側行っちゃうから対策
	//1.続き pointNum5へのベクトルを求めて、そっちに移動させる?。(Enemyの速度早くなる)
	//1.続き 速度は問題なさそう。超斜めの時に突っ込んでも真ん中行くようにする?

	//3.紐の後ろ(引っ張られてるほうにいる敵)にいる敵は吹っ飛ばないように対策する (飛んでも問題ないのでは? 引っ張ってるわけだし物理的に問題ないと思われる)
	//3.続き　しかし、規定位置からどのくらい移動したかで判断するため、めっちゃ吹っ飛ぶ
	//3.続き ゴムと敵の位置が、ゴムと規定位置へのベクトルと大体逆だったら飛ばさなければいい?

	//4.ちゃんとした向きにダッシュしなかったときは、吹っ飛ばないようにする
	//4.続き エネミーのベクトルの角度と、プレイヤーのダッシュ方向が、一定の角度以上かつ未満だったらでOK?
	//4.続き 逆(内側にダッシュ)した場合の対策もしっかり(お互いが大体反対か調べる?)
	//4.続き 内側ダッシュ対策も!!
	//4.続き 内側ダッシュ対策はした。少しでも敵の方に向いてると吹っ飛ばないから少し緩めたが、まだシビア?

	if (typeid(*object) == typeid(Enemy)) 
	{
		//どれか一個当たったら、無視
		if (hitEnemy)return;

		Enemy* e = static_cast<Enemy*>(object->getPtr());
		
		//発射されてたら判定無視
		if (e->GetMyShot())return;

		for (int i = 0; i < 9; i++) 
		{
			rubberPtr[i]->setHitEnemy(true);
		}

		//敵のvelocityと速度を取得
		
		//敵のベクトル
		Vector3 eVel;
		Vector3 eSpe;
		e->GetVelocityAndSpeed(eVel, eSpe);

		//敵を中心に寄せる
		if (pointNum != 5) 
		{
			enemyMoveVector = LibMath::otherVector(position, rubberPtr[4]->getSphereData()[0].position);
			e->AddPosition(enemyMoveVector * 0.05f);
		}

		//ゴムとプレイヤーを動かす
#pragma region ゴム

		//rimitがtrueになったりfalseになったりを繰り返して、
		//MoveBairitu[arrNum]が掛けられるからプレイヤーと同じように動かない
		//プレイヤーを追い越すことはないからこのままでも問題ない?
		if (allRimitFlag)
		{
			//敵を動かなくする
			//moveFlagがtrueとfalseを繰り返す?ので、実質数値の半分くらいしか反映されてない
			//敵の移動なくしちゃう
			//e->AddPosition(eVel*eSpe * -1);

			//プレイヤーを動かすベクトルセット
			playerMoveVector = eVel * eSpe;

			//プレイヤーとともにゴムが動く
			Vector3 moveVec = playerMoveVector;
			position += moveVec;
			Library::setPosition(position, heapHandle, 0);
			sphereData[0].position = position;

			int arrNum = 0;
			for (int i = pointNum; i < 9; i++)
			{
				rubberPtr[i]->addPosition(moveVec);
				arrNum++;
			}
			arrNum = 0;
			for (int i = pointNum - 2; i > -1; i--)
			{
				rubberPtr[i]->addPosition(moveVec);
				arrNum++;
			}

			//限界の時にプレイヤーが動いたら引っ張られる
			Vector3 pMoveSpeed = { 0,0,0 };
			for (int i = 0; i < _countof(rubberPtr); i++) 
			{
				pMoveSpeed = playerVelocity[0] * playerSpeed[0];
				rubberPtr[i]->addPosition({ pMoveSpeed .x/2,pMoveSpeed .y/2,pMoveSpeed .z/2});
				pMoveSpeed = playerVelocity[1] * playerSpeed[1];
				rubberPtr[i]->addPosition({ pMoveSpeed .x/2,pMoveSpeed .y/2,pMoveSpeed .z/2});
			}
			//敵も引っ張る
			pMoveSpeed = playerVelocity[0] * playerSpeed[0];
			e->AddPosition(pMoveSpeed);
			pMoveSpeed = playerVelocity[1] * playerSpeed[1];
			e->AddPosition(pMoveSpeed);
		}

		//移動
		if (!allRimitFlag) 
		{
			Vector3 moveVec = eVel * eSpe;
			position += moveVec;
			Library::setPosition(position, heapHandle, 0);
			sphereData[0].position = position;

			//他の点も動かす
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

		//4.ちゃんとした向きにダッシュしなかったときは、吹っ飛ばないようにする
		//ok 4.続き エネミーのベクトルの角度と、プレイヤーのダッシュ方向が、一定の角度以上かつ未満だったらでOK?
		//4.続き 逆(内側にダッシュ)した場合の対策もしっかり(お互いが大体反対か調べる?)
		//4.続き 同時方向ダッシュ対策も
		if (dashFlag) 
		{
			Vector3 eV, eS;
			e->GetVelocityAndSpeed(eV, eS);

		
			//数値だけ動かしてみる前の距離
			float distance[2];
			//後
			float moveDistance[2];
			Vector3 ePos = e->getSphereData()[0].position;

			distance[0] = LibMath::calcDistance3D(playerPos[0], ePos);
			moveDistance[0] = LibMath::calcDistance3D(playerPos[0] + playerVelocity[0], ePos);

			distance[1] = LibMath::calcDistance3D(playerPos[1], ePos);
			moveDistance[1] = LibMath::calcDistance3D(playerPos[1] + playerVelocity[1], ePos);

			//仮に動かした後のほうが離れているかつ、
			//ダッシュした瞬間だったら入る
			if (distance[0] < moveDistance[0] + 0.05f&&
				distance[1] < moveDistance[1] + 0.05f &&
				!playerDashEnd)
			{
				//敵発射
				eS = normalPos - position;
				e->ShotEnemy(eV * -1, { abs((eS.x + eS.z) / 4),0, abs((eS.x + eS.z) / 4) });
			}
				
			playerDashEnd = true;
		}
		else 
		{
			playerDashEnd = false;
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