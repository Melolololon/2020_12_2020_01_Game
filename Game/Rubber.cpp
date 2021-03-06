#include "Rubber.h"
#include"Enemy.h"
#include"PolygonManager.h"

Rubber* Rubber::rubberPtr[9];
bool Rubber::dashFlag;
Vector3 Rubber::playerMoveVector;

//0 レフト 1 ライト
Vector3 Rubber::vecPToP[2];
Vector3 Rubber::playerPos[2];
Vector3 Rubber::playerVelocity[2];
Vector3 Rubber::playerSpeed[2];
bool Rubber::playerInputFlag[2];
float Rubber::playerRevDidtanceNumber;
float Rubber::playerMaxDistanceNumber;;

int Rubber::rimitCount;
bool Rubber::allRimitFlag; 

bool Rubber::leavePlayerFlag; 
bool Rubber::playerDashEnd;

Vector3 Rubber::enemyVelocity;

std::vector<Object*>Rubber::hitEnemys;

Rubber::Rubber(const int& pNum)
{
	enemyVelocity = 0;

	pointNum = pNum;

	velocity = 0;
	speed = 1.0f;
	isDead = false;

	collisionFlag.board = false;
	collisionFlag.ray = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = true;
	collisionFlag.plane = false;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 0.4f;

	vertexHandle = PolygonManager::getInstance()->getPolygonVertex("rubber");
	heapHandle = PolygonManager::getInstance()->getPolygonHeap("rubber");
	Library::setPosition(position, heapHandle, pointNum - 1);
	Library::setScale({ 0.5,0.5,0.5 }, heapHandle, pointNum - 1);
	enemyMoveVector = 0;
	hitEnemy = false;


	position = playerPos[0] + vecPToP[0] * (pointNum * 0.1);
	normalPos = position;
	maxMoveDistance = 2.0f;
}

Rubber::~Rubber()
{
	//Library::deleteVertexData(vertexHandle);
	//Library::deleteHeapData(heapHandle);
}

void Rubber::loadModel()
{
	vertex v;
	heap h;
	std::string material;

	/*Library::createManyVertex3DBox({ 2,2,2 }, &v);
	Library::createHeapData2({ 255,255,255,255 }, 9, &h);*/
	Library::loadOBJVertex("Resources/Obj/rubber.obj", true, true, &material, &v);
	Library::loadOBJMaterial("Resources/Obj/", material, 9, &h);
	PolygonManager::getInstance()->addPolygonVertex("rubber", v);
	PolygonManager::getInstance()->addPolygonHeap("rubber", h);
}

void Rubber::update()
{
	//リセット
	//staticなのになぜか初期化されん。spTestで試す
	playerMoveVector = 0;
	enemyVelocity = 0;

	

	//規定位置を取得
	normalPos = playerPos[0] + vecPToP[0] * (pointNum * 0.1);

	//当たってなかったら戻るように
	//自分の座標から規定位置のベクトル
	Vector3 myPosToNorPos = LibMath::otherVector(position, normalPos);
	if (!hitEnemy) 
	{
		float dis = LibMath::calcDistance3D(position, normalPos);
		//一定以上離れてたら元の位置に向かって移動
		//敵の数に応じて倍率変える(これないと複数引っかかった時プレイヤーからゴムがめっちゃ離れる)
		int mulNum = hitEnemys.size();
		if (mulNum <= 0)mulNum = 1;
		if (dis >= 0.45f)
		{
			position += myPosToNorPos * speed * mulNum;
		}
		else
			position = normalPos;
	}
	hitEnemys.clear();

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
	Library::setPosition(position, heapHandle, pointNum - 1);
	
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
	
	float pToPDis = LibMath::calcDistance3D(playerPos[0], playerPos[1]);
	maxMoveDistance = 2.0f;
	if (pToPDis >= playerRevDidtanceNumber) 
	{
		maxMoveDistance -= pToPDis / 7;
		maxMoveDistance = maxMoveDistance <= 0 ? 0 : maxMoveDistance;
	}

	if(pointNum != 1 && pointNum != 9)
	{
		if (rubberDis >= maxMoveDistance)
		{
			allRimitFlag = true;
		}
	}

	if (pointNum == 1 || pointNum == 9) 
	{
		if (rubberDis  >= maxMoveDistance + 2.0f)
		{
			allRimitFlag = true;
		}
	}

	if (rimitCount == 9)allRimitFlag = true;


	//ダッシュ時判定でかく(敵すり抜け防止)
	if (dashFlag)
	{
		sphereData[0].r = 0.8f;
	}
	else
	{
		sphereData[0].r = 0.4f;
	}


	//近かったら特定の点のサイズを変える
	if (LibMath::calcDistance3D(playerPos[0], playerPos[1]) < playerRevDidtanceNumber)
	{
		if (pointNum == 4 ||
			pointNum == 5 ||
			pointNum == 6) 
		{
			sphereData[0].r = 0.2f;
		}
	}
	else 
	{
		sphereData[0].r = 0.4f;
	}


	
}

void Rubber::draw()
{
	Library::setPipeline(PIPELINE_MATERIAL);
	Library::setSmoothingFlag(true);
	Library::drawGraphic(vertexHandle, heapHandle, pointNum - 1);
	Library::setPipeline(PIPELINE_NORMAL);
	Library::setSmoothingFlag(false);
}

void Rubber::hit(Object* object, CollisionType collisionType)
{
	//やること
	//1.敵が紐の中心によるように調整
	//1.続き ベクトル固定だとプレイヤーの位置が逆になった時に反対側行っちゃうから対策
	//1.続き pointNum5へのベクトルを求めて、そっちに移動させる?。(Enemyの速度早くなる)
	//1.続き 速度は問題なさそう。超斜めの時に突っ込んでも真ん中行くようにする?

	//3.紐の後ろ(引っ張られてるほうにいる敵)にいる敵は吹っ飛ばないように対策する (飛んでも問題ないのでは? 引っ張ってるわけだし物理的に問題ないと思われる)
	//3.続き　しかし、規定位置からどのくらい移動したかで判断するため、めっちゃ吹っ飛ぶ
	//3.続き ゴムと敵の位置が、ゴムと規定位置へのベクトルと大体逆だったら飛ばさなければいい?
	//3.続き 外積の左右判定使えば行ける?

	//4.ちゃんとした向きにダッシュしなかったときは、吹っ飛ばないようにする
	//4.続き エネミーのベクトルの角度と、プレイヤーのダッシュ方向が、一定の角度以上かつ未満だったらでOK?
	//4.続き 逆(内側にダッシュ)した場合の対策もしっかり(お互いが大体反対か調べる?)
	//4.続き 内側ダッシュ対策も!!
	//4.続き 内側ダッシュ対策はした。少しでも敵の方に向いてると吹っ飛ばないから少し緩めたが、まだシビア?
	//4.続き 内側ダッシュしたときどうするか考える

	//5.敵が引っかかってるときにゴムがエリア外に行くとプレイヤーエリア外に出る
	//5.続き プレイヤー動かしたあとに戻す処理呼んでないから

	//6.複数引っ掛けたらめちゃくちゃゴム伸びる

	//7.内側ダッシュしたら敵isDead = Trueにする



	if (typeid(*object) == typeid(Enemy)) 
	{


		//どれか一個当たったら、無視
		for (auto& e : hitEnemys) 
		{
			if (e == object)return;
		}
		hitEnemys.push_back(object);

		Enemy* e = static_cast<Enemy*>(object->getPtr());
		Vector3 eV, eS;
		e->GetVelocityAndSpeed(eV, eS);

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
		enemyVelocity = eVel;

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
			playerMoveVector += eVel * eSpe;

			//プレイヤーとともにゴムが動く
			Vector3 moveVec = eVel * eSpe;
			position += moveVec;
			Library::setPosition(position, heapHandle, pointNum - 1);
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

			//最大まで引っ張ってたら敵止まる
			if(LibMath::calcDistance3D(playerPos[0],playerPos[1]) >= playerMaxDistanceNumber - 1.0f &&
				!dashFlag)
			e->AddPosition(eV * eS * -1 * 2);

			//限界の時にプレイヤーが動いたら引っ張られる
			Vector3 pMoveSpeed = { 0,0,0 };
			for (int i = 0; i < _countof(rubberPtr); i++)
			{
				if (playerVelocity[0].x == 0 && playerVelocity[0].z == 0)
				{
					pMoveSpeed = playerVelocity[1] * playerSpeed[1];
					rubberPtr[i]->addPosition({ pMoveSpeed.x ,pMoveSpeed.y ,pMoveSpeed.z });
				}
				else if (playerVelocity[1].x == 0 && playerVelocity[1].z == 0)
				{
					pMoveSpeed = playerVelocity[0] * playerSpeed[0];
					rubberPtr[i]->addPosition({ pMoveSpeed.x ,pMoveSpeed.y ,pMoveSpeed.z });
				}
				else 
				{

					pMoveSpeed = playerVelocity[0] * playerSpeed[0];
					rubberPtr[i]->addPosition({ pMoveSpeed.x / 2,pMoveSpeed.y / 2,pMoveSpeed.z / 2 });
					pMoveSpeed = playerVelocity[1] * playerSpeed[1];
					rubberPtr[i]->addPosition({ pMoveSpeed.x / 2,pMoveSpeed.y / 2,pMoveSpeed.z / 2 });
				}
			}
			//敵も引っ張る


			if (playerVelocity[0].x == 0 && playerVelocity[0].z == 0)
			{
				pMoveSpeed = playerVelocity[1] * playerSpeed[1];
				e->AddPosition({ pMoveSpeed.x ,pMoveSpeed.y ,pMoveSpeed.z });
			}
			else if (playerVelocity[1].x == 0 && playerVelocity[1].z == 0)
			{
				pMoveSpeed = playerVelocity[0] * playerSpeed[0];
				e->AddPosition({ pMoveSpeed.x ,pMoveSpeed.y ,pMoveSpeed.z });
			}
			else
			{

				pMoveSpeed = playerVelocity[0] * playerSpeed[0];
				e->AddPosition({ pMoveSpeed.x / 2,pMoveSpeed.y / 2,pMoveSpeed.z / 2 });
				pMoveSpeed = playerVelocity[1] * playerSpeed[1];
				e->AddPosition({ pMoveSpeed.x / 2,pMoveSpeed.y / 2,pMoveSpeed.z / 2 });
			}
			
		}

		//移動
		if (!allRimitFlag) 
		{
			Vector3 moveVec = eVel * eSpe;
			position += moveVec;
			Library::setPosition(position, heapHandle, pointNum - 1);
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

		
		}

		allRimitFlag = false;
#pragma endregion

#pragma region 場外防止

		//敵エリア外に出さない
		if (position.x > 26 ||
			position.x < -26 ||
			position.z > 22 ||
			position.z < -22)
		{
			e->AddPosition(eV * eS * -1 * 2);
		}

		//ゴムも	
	/*	Vector3 otherRubPos;
		if (position.x > 23) 
		{

			setPosition({ 23,position.y,position.z });

			int arrNum = 0;
			for (int i = pointNum; i < 9; i++)
			{
				otherRubPos = rubberPtr[i]->getSphereData()[0].position;
				rubberPtr[i]->setPosition({ 23,otherRubPos.y,otherRubPos.z });
				arrNum++;
			}
			arrNum = 0;
			for (int i = pointNum - 2; i > -1; i--)
			{
				otherRubPos = rubberPtr[i]->getSphereData()[0].position;
				rubberPtr[i]->setPosition({ 23,otherRubPos.y,otherRubPos.z });
				arrNum++;
			}
		}

		if (position.x < -23) 
		{
			setPosition({ -23,position.y,position.z });
			int arrNum = 0;
			for (int i = pointNum; i < 9; i++)
			{
				otherRubPos = rubberPtr[i]->getSphereData()[0].position;
				rubberPtr[i]->setPosition({ -23,otherRubPos.y,otherRubPos.z });
				arrNum++;
			}
			arrNum = 0;
			for (int i = pointNum - 2; i > -1; i--)
			{
				otherRubPos = rubberPtr[i]->getSphereData()[0].position;
				rubberPtr[i]->setPosition({ -23,otherRubPos.y,otherRubPos.z });
				arrNum++;
			}
		}

		if (position.z > 19)
		{
			setPosition({ position.x,position.y,19 });
			int arrNum = 0;
			for (int i = pointNum; i < 9; i++)
			{
				otherRubPos = rubberPtr[i]->getSphereData()[0].position;
				rubberPtr[i]->setPosition({ otherRubPos.x,otherRubPos.y,15});
				arrNum++;
			}
			arrNum = 0;
			for (int i = pointNum - 2; i > -1; i--)
			{
				otherRubPos = rubberPtr[i]->getSphereData()[0].position;
				rubberPtr[i]->setPosition({ otherRubPos.x,otherRubPos.y,15 });
				arrNum++;
			}
		}

		if (position.z < -19)
		{
			setPosition({ position.x,position.y,-19 });
			int arrNum = 0;
			for (int i = pointNum; i < 9; i++)
			{
				otherRubPos = rubberPtr[i]->getSphereData()[0].position;
				rubberPtr[i]->setPosition({ otherRubPos.x,otherRubPos.y,-15 });
				arrNum++;
			}
			arrNum = 0;
			for (int i = pointNum - 2; i > -1; i--)
			{
				otherRubPos = rubberPtr[i]->getSphereData()[0].position;
				rubberPtr[i]->setPosition({ otherRubPos.x,otherRubPos.y,-15 });
				arrNum++;
			}
		}*/
#pragma endregion



		//4.ちゃんとした向きにダッシュしなかったときは、吹っ飛ばないようにする
		//ok 4.続き エネミーのベクトルの角度と、プレイヤーのダッシュ方向が、一定の角度以上かつ未満だったらでOK?
		//4.続き 逆(内側にダッシュ)した場合の対策もしっかり(お互いが大体反対か調べる?)
		//4.続き 同時方向ダッシュ対策も
		
		if (dashFlag) 
		{
			
			//数値だけ動かしてみる前の距離
			float distance[2];
			//後
			float moveDistance[2];
			Vector3 ePos = e->getSphereData()[0].position;

			distance[0] = LibMath::calcDistance3D(playerPos[0], ePos);
			moveDistance[0] = LibMath::calcDistance3D(playerPos[0] + playerVelocity[0] , ePos);

			distance[1] = LibMath::calcDistance3D(playerPos[1], ePos);
			moveDistance[1] = LibMath::calcDistance3D(playerPos[1] + playerVelocity[1] , ePos);

			
			if (distance[0] < moveDistance[0] + 0.05f&&
				distance[1] < moveDistance[1] + 0.05f &&
				!playerDashEnd)
			{
				//敵発射
				/*eS = normalPos - position;
				e->ShotEnemy(eV * -1, { abs((eS.x + eS.z) / 4),0, abs((eS.x + eS.z) / 4) });*/
				float speed = LibMath::calcDistance3D(normalPos, position) / 4;
				e->ShotEnemy(eV * -1, { speed ,0,speed });
			}


			//新しいほう
			//敵からプレイヤーのベクトルを求める
			//そのベクトルから、一定角度回転させた方向以内だったらショット成功

				
			playerDashEnd = true;
		}
		else 
		{
			playerDashEnd = false;
		}
		

		
	}

}

void Rubber::setPosition(const Vector3& pos) 
{
	position = pos;
	Library::setPosition(position, heapHandle, pointNum - 1);
	sphereData[0].position = position;
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
	Library::setPosition(position, heapHandle, pointNum - 1);
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

void Rubber::setPlayerRevDidtanceNumber(const float& num)
{
	playerRevDidtanceNumber = num;
}

void Rubber::setPlayerMaxDistanceNumber(const float& num)
{
	playerMaxDistanceNumber = num;
}

Vector3 Rubber::getEnemyVector() 
{
	return enemyVelocity;
}
