#include "Player.h"
#include"Enemy.h"

#include"PolygonManager.h"

Vector3 Player::leftPlayerPosition;
Vector3 Player::rightPlayerPosition;
Vector3 Player::leftPlayerVelocity;
Vector3 Player::rightPlayerVelocity;
Player::PlayerType Player::firstAddType;
Player* Player::firstAddPlayer;
bool Player::leavePlayer;
bool Player::deadPlayer;

Player::Player(const Vector3& pos, const PlayerType& playerType)
{
	if (firstAddType == PlayerType::NONE)
	{
		firstAddType = playerType;
		firstAddPlayer = this;
	}
	position = pos;
	velocity = 0;
	speed = NormalSpeed;
	isDead = false;

	sphereData.resize(1);
	sphereData[0].position = position;
	sphereData[0].r = 1.0f;

	Library::createManyVertex3DBox({ 2,2,2 }, &vertexHandle);

	if (playerType == PlayerType::LEFT)Library::createHeapData2({ 0,0,255,255 }, 1, &heapHandle);
	if (playerType == PlayerType::RIGHT)Library::createHeapData2({ 255,0,0,255 }, 1, &heapHandle);

	collisionFlag.board = false;
	collisionFlag.ray = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = true;
	collisionFlag.plane = false;

	this->playerType = playerType;

#pragma region ダッシュ
	dashOperPossibleFlag = false;
	dashOperTimeFlag = false;
	dashOperTimer = 0;
	isDash = false;


	dashTimer = 0;

	leftInputDirectionUpDown = InputDirection::INPUT_DIRECTION_CENTER;
	leftInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_CENTER;
	rightInputDirectionUpDown = InputDirection::INPUT_DIRECTION_CENTER;
	rightInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_CENTER;
#pragma endregion

	inputFlag = false;
#pragma region ゴム


	//インプットレイアウト設定
	Library::setInputLayout("POSITION", 3);
	Library::setInputLayout("NORMAL", 3);
	Library::setInputLayout("ADDPOSITION", 3);

#pragma endregion

	//ライフ
	life = 4;
	
	//無敵処理
	isMuteki = false;
	mutekiTimer = 0;

	kasanariTimer = 0;
	hitOtherPlayer = false;

	deadPlayer = false;
}


Player::~Player()
{
	Library::deleteVertexData(vertexHandle);
	Library::deleteHeapData(heapHandle);
}


void Player::initializeModel()
{
}

void Player::update()
{
	//片方死んだら早期リターン
	if (deadPlayer)return;

	if (!hitOtherPlayer)kasanariTimer = 0;
	hitOtherPlayer = false;

	inputFlag = false;
	leavePlayer = false;
	//一定以上引っ張ると引っ張る速度が落ちる(ゴム引っ張ってるから)
	//これにより、引っ張ると受け止めやすくなるが、ショットに時間がかかる
	//(引っ張るのも時間かかるし、敵がゴムを引っ張るのにも時間かかる)

#pragma region 移動処理

	float playerDistance;
	playerDistance = LibMath::calcDistance3D(leftPlayerPosition, rightPlayerPosition);

	Vector3 firstAddPlayerVector;//先に追加されたプレイヤーへのベクトル
	Vector3 reversVector;//上のベクトルの逆ベクトル
	if (playerType == PlayerType::LEFT)firstAddPlayerVector = LibMath::otherVector(leftPlayerPosition, rightPlayerPosition);
	if (playerType == PlayerType::RIGHT)firstAddPlayerVector = LibMath::otherVector(rightPlayerPosition, leftPlayerPosition);
	//自分からもう一体のプレイヤーへのベクトル(firstAddPlayerVectorの逆ベクトル)
	reversVector = firstAddPlayerVector * -1;

#pragma region 移動

	if (!isDash) 
	{
		velocity = 0;

		if (playerType == PlayerType::LEFT)
		{
			if (DirectInput::leftStickUp(30000) || 
				DirectInput::keyState(DIK_W) ||
				DirectInput::directionalButtonState(UpButton)) 
			{
				inputFlag = true;
				velocity.z++;
			}
			if (DirectInput::leftStickDown(30000) ||
				DirectInput::keyState(DIK_S) ||
				DirectInput::directionalButtonState(DownButton))
			{
				inputFlag = true;
				velocity.z--;
			}
			if (DirectInput::leftStickRight(30000) || 
				DirectInput::keyState(DIK_D) ||
				DirectInput::directionalButtonState(RightButton))
			{
				inputFlag = true;
				velocity.x++;
			}
			if (DirectInput::leftStickLeft(30000) ||
				DirectInput::keyState(DIK_A)||
				DirectInput::directionalButtonState(LeftButton))
			{
				inputFlag = true;
				velocity.x--;
			}

		}

		if (playerType == PlayerType::RIGHT)
		{
			if (DirectInput::rightStickUp(30000) ||
				DirectInput::keyState(DIK_UP) ||
				DirectInput::buttonState(YButton))
			{
				inputFlag = true;
				velocity.z++;
			}
			if (DirectInput::rightStickDown(30000) ||
				DirectInput::keyState(DIK_DOWN) ||
				DirectInput::buttonState(AButton))
			{

				inputFlag = true;
				velocity.z--;
			}
			if (DirectInput::rightStickRight(30000) ||
				DirectInput::keyState(DIK_RIGHT) ||
				DirectInput::buttonState(BButton))
			{
				inputFlag = true;
				velocity.x++;
			}
			if (DirectInput::rightStickLeft(30000) ||
				DirectInput::keyState(DIK_LEFT) ||
				DirectInput::buttonState(XButton))
			{
				inputFlag = true;
				velocity.x--;
			}
		}
	}
#pragma endregion

#pragma region ダッシュ

	if (!isDash ) 
	{
#pragma region スティックダッシュ

		if (playerType == PlayerType::LEFT && !dashOperTimeFlag)
		{
			if (DirectInput::leftStickUp(30000) ||
				DirectInput::keyState(DIK_W) ||
				DirectInput::directionalButtonState(UpButton))
			{
				dashOperTimeFlag = true;
				leftInputDirectionUpDown = InputDirection::INPUT_DIRECTION_UP;
			}
			if (DirectInput::leftStickDown(30000) ||
				DirectInput::keyState(DIK_S) ||
				DirectInput::directionalButtonState(DownButton))
			{
				dashOperTimeFlag = true;
				leftInputDirectionUpDown = InputDirection::INPUT_DIRECTION_DOWN;
			}
			if (DirectInput::leftStickRight(30000) ||
				DirectInput::keyState(DIK_D) ||
				DirectInput::directionalButtonState(RightButton))
			{
				dashOperTimeFlag = true;
				leftInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_RIGHT;
			}
			if (DirectInput::leftStickLeft(30000) ||
				DirectInput::keyState(DIK_A) ||
				DirectInput::directionalButtonState(LeftButton))
			{
				dashOperTimeFlag = true;
				leftInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_LEFT;
			}
		}
		if (playerType == PlayerType::RIGHT && !dashOperTimeFlag)
		{
			if (DirectInput::rightStickUp(30000) ||
				DirectInput::keyState(DIK_UP) ||
				DirectInput::buttonState(YButton))
			{
				dashOperTimeFlag = true;
				rightInputDirectionUpDown = InputDirection::INPUT_DIRECTION_UP;
			}
			if (DirectInput::rightStickDown(30000) ||
				DirectInput::keyState(DIK_DOWN) ||
				DirectInput::buttonState(AButton))
			{
				dashOperTimeFlag = true;
				rightInputDirectionUpDown = InputDirection::INPUT_DIRECTION_DOWN;
			}
			if (DirectInput::rightStickRight(30000) ||
				DirectInput::keyState(DIK_RIGHT) ||
				DirectInput::buttonState(BButton))
			{
				dashOperTimeFlag = true;
				rightInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_RIGHT;
			}
			if (DirectInput::rightStickLeft(30000) ||
				DirectInput::keyState(DIK_LEFT) ||
				DirectInput::buttonState(XButton))
			{
				dashOperTimeFlag = true;
				rightInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_LEFT;
			}
		}

		//入力可能時間内
		if (dashOperTimeFlag)
		{
			dashOperTimer++;
			//入力後、移動しなくなるまでパッドを中心に移動させたらダッシュ可能に
			if (velocity.x == 0 && velocity.y == 0 && velocity.z == 0) dashOperPossibleFlag = true;
		}
		if (dashOperTimer >= DashOperTime)
		{
			dashOperTimeFlag = false;
			dashOperPossibleFlag = false;
			dashOperTimer = 0;

			rightInputDirectionUpDown = InputDirection::INPUT_DIRECTION_CENTER;
			rightInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_CENTER;
			leftInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_CENTER;
			leftInputDirectionUpDown = InputDirection::INPUT_DIRECTION_CENTER;
		}

		//時間以内かつ入力可能時に入力されたら
		if (dashOperTimeFlag && dashOperPossibleFlag)
		{
			//再入力されたらダッシュ
			if (playerType == PlayerType::LEFT)
			{
				if (DirectInput::leftStickUp(30000) ||
					DirectInput::keyState(DIK_W) ||
					DirectInput::directionalButtonState(UpButton))
				{
					if(leftInputDirectionUpDown == InputDirection::INPUT_DIRECTION_UP)
					isDash = true;
				}
				if (DirectInput::leftStickDown(30000) ||
					DirectInput::keyState(DIK_S) ||
					DirectInput::directionalButtonState(DownButton))
				{
					if (leftInputDirectionUpDown == InputDirection::INPUT_DIRECTION_DOWN)
					isDash = true;
				}
				if (DirectInput::leftStickRight(30000) ||
					DirectInput::keyState(DIK_D) ||
					DirectInput::directionalButtonState(RightButton))
				{
					if (leftInputDirectionLeftRight == InputDirection::INPUT_DIRECTION_RIGHT)
					isDash = true;
				}
				if (DirectInput::leftStickLeft(30000) ||
					DirectInput::keyState(DIK_A) ||
					DirectInput::directionalButtonState(LeftButton))
				{
					if (leftInputDirectionLeftRight == InputDirection::INPUT_DIRECTION_LEFT)
					isDash = true;
				}
			}

			if (playerType == PlayerType::RIGHT)
			{
				if (DirectInput::rightStickUp(30000) ||
					DirectInput::keyState(DIK_UP) ||
					DirectInput::buttonState(YButton))
				{
					if (rightInputDirectionUpDown == InputDirection::INPUT_DIRECTION_UP)
					isDash = true;
				}
				if (DirectInput::rightStickDown(30000) ||
					DirectInput::keyState(DIK_DOWN) ||
					DirectInput::buttonState(AButton))
				{
					if (rightInputDirectionUpDown == InputDirection::INPUT_DIRECTION_DOWN)
					isDash = true;
				}
				if (DirectInput::rightStickRight(30000) ||
					DirectInput::keyState(DIK_RIGHT) ||
					DirectInput::buttonState(BButton))
				{
					if (rightInputDirectionLeftRight == InputDirection::INPUT_DIRECTION_RIGHT)
					isDash = true;
				}
				if (DirectInput::rightStickLeft(30000) ||
					DirectInput::keyState(DIK_LEFT) ||
					DirectInput::buttonState(XButton))
				{
					if (rightInputDirectionLeftRight == InputDirection::INPUT_DIRECTION_LEFT)
					isDash = true;
				}
			}
			if (isDash)
			{
				speed = MaxDashSpeed;
				dashOperTimeFlag = false;
				dashOperPossibleFlag = false;
				dashOperTimer = 0;

				rightInputDirectionUpDown = InputDirection::INPUT_DIRECTION_CENTER;
				rightInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_CENTER;
				leftInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_CENTER;
				leftInputDirectionUpDown = InputDirection::INPUT_DIRECTION_CENTER;
			}
		}
#pragma endregion

#pragma region ボタンダッシュ
		if (velocity.x != 0 ||
			velocity.z != 0) 
		{
			if (DirectInput::keyTrigger(DIK_SPACE) ||
				DirectInput::buttonTrigger(LBButton) ||
				DirectInput::buttonTrigger(RBButton)) 
			{
				isDash = true;


			}

			if (isDash)
			{
				speed = MaxDashSpeed;
				dashOperTimeFlag = false;
				dashOperPossibleFlag = false;
				dashOperTimer = 0;

				rightInputDirectionUpDown = InputDirection::INPUT_DIRECTION_CENTER;
				rightInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_CENTER;
				leftInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_CENTER;
				leftInputDirectionUpDown = InputDirection::INPUT_DIRECTION_CENTER;
			}
		}

#pragma endregion

	}



	if (isDash) 
	{
		dashTimer++;
		if (dashTimer >= DashTime) 
		{
			speed.x -= 0.2f;
			speed.y -= 0.2f;
			speed.z -= 0.2f;
		}
		if (speed.x <= NormalSpeed.x) 
		{
			speed = NormalSpeed;
			isDash = false;
			dashTimer = 0;
		}
	}

#pragma endregion

#pragma region ゴムによる引っ張られ処理

	//15以上離れたら引っ張られる
	if (playerDistance > PlayerRevDistance)
	{
		if(velocity.x == 0 && velocity.z == 0)
		position += firstAddPlayerVector * speed * 1.5;
		else
		position += firstAddPlayerVector * speed * 0.5;

		leavePlayer = true;

	}
#pragma endregion

#pragma region 通常の座標セット

	position += velocity * speed;
	

	if (playerType == PlayerType::LEFT)
	{
		leftPlayerPosition = position;
		leftPlayerVelocity = velocity;
	}
	if (playerType == PlayerType::RIGHT)
	{
		rightPlayerPosition = position;
		rightPlayerVelocity = velocity;
	}


#pragma endregion

#pragma region 離れるの防止
	//最初に追加したやつでは行わない
	//こう書かないと、片方が引っ張られても動かなくなる
	//(移動処理が片方で終わるため)
	//片方をspeed分戻すからこう書かないといけないだけで、
	//両方にspeedの半分分だけ戻せばいいのでは?
	//片方が動いてるとき、一方的に引っ張りたい(動いてるほうは戻されないようにしたい)ので、こうする
	if (firstAddType != playerType)
	{
		while (1)
		{
			playerDistance = LibMath::calcDistance3D(leftPlayerPosition, rightPlayerPosition);

			if (playerType == PlayerType::LEFT)firstAddPlayerVector = LibMath::otherVector(leftPlayerPosition, rightPlayerPosition);
			if (playerType == PlayerType::RIGHT)firstAddPlayerVector = LibMath::otherVector(rightPlayerPosition, leftPlayerPosition);
			//自分からもう一体のプレイヤーへのベクトル(firstAddPlayerVectorの逆ベクトル)
			reversVector = firstAddPlayerVector * -1;

			//プレイヤー同士の距離が限界地を超えていたら
			if (playerDistance > PlayerMaxDistance)
			{

				//座標を変更(動かなかったら引っ張る)
				if (playerType == PlayerType::LEFT)
				{
					//片方動いた
					if (velocity.x == 0 && velocity.z == 0)
						setPosition(position + firstAddPlayerVector * speed);

					//両方動いた
					if (velocity.x != 0 || velocity.z != 0)
					{
						if (rightPlayerVelocity.x != 0 || rightPlayerVelocity.z != 0)
						setPosition(position + firstAddPlayerVector * speed);
					}
				}
				if (playerType == PlayerType::RIGHT)
				{
					//片方動いた
					if (velocity.x == 0 && velocity.z == 0)
						setPosition(position + firstAddPlayerVector * speed);

					//両方動いた
					if (velocity.x != 0 || velocity.z != 0)
					{
						if (leftPlayerVelocity.x != 0 || leftPlayerVelocity.z != 0)
							setPosition(position + firstAddPlayerVector * speed);
					}
				}


				//最初に追加したプレイヤーの座標を変更
				if (firstAddType == PlayerType::LEFT)
				{
					//片方動いた
					if (leftPlayerVelocity.x == 0 && leftPlayerVelocity.z == 0)
						firstAddPlayer->setPosition(leftPlayerPosition + reversVector * speed);

					//両方動いた
					if (leftPlayerVelocity.x != 0 || leftPlayerVelocity.z != 0)
					{
						if (rightPlayerVelocity.x != 0 || rightPlayerVelocity.z != 0)
							firstAddPlayer->setPosition(leftPlayerPosition + reversVector * speed);
					}
				}
				if (firstAddType == PlayerType::RIGHT)
				{
					//片方動いた
					if (rightPlayerVelocity.x == 0 && rightPlayerVelocity.z == 0)
						firstAddPlayer->setPosition(rightPlayerPosition + reversVector * speed);

					//両方動いた
					if (rightPlayerVelocity.x != 0 || rightPlayerVelocity.z != 0)
					{
						if (leftPlayerVelocity.x != 0 || leftPlayerVelocity.z != 0)
							firstAddPlayer->setPosition(rightPlayerPosition + reversVector * speed);
					}
				}

				
			}
			else//限界値以内になったら移動終了 
			{
				break;
			}
		}
	}
#pragma endregion


	areaPush();
	Library::setPosition(position, heapHandle, 0);
	sphereData[0].position = position;

#pragma endregion

#pragma region ゴムに情報を送る
	if(playerType == PlayerType::LEFT)
	Rubber::setPlayerToPlayerVector(rightPlayerPosition - leftPlayerPosition, playerType);

	if (playerType == PlayerType::RIGHT)
		Rubber::setPlayerToPlayerVector(leftPlayerPosition - rightPlayerPosition, playerType);


	Rubber::setPlayerPosition(position,playerType);
	Rubber::setDashFlag(isDash);
	Rubber::setPlayerVectorAndSpeed(velocity, speed, playerType);
	Rubber::resetRimitCount();
	Rubber::setPlayerInputFlag(inputFlag,playerType);
	Rubber::setLeavePlayerFlag(leavePlayer);
	Rubber::setPlayerRevDidtanceNumber(PlayerRevDidtanceNumber);
	Rubber::setPlayerMaxDistanceNumber(PlayerMaxDistance);
#pragma endregion

	Enemy::SetPlayerPos(position, playerType);


#pragma region 無敵処理
	if (isMuteki)mutekiTimer++;
	if (mutekiTimer >= MutekiTime) 
	{
		isMuteki = false;
		mutekiTimer = 0;
	}
#pragma endregion

#pragma region ライフ処理
	if (life <= 0) 
	{
		isDead = true;
		deadPlayer = true;
		firstAddPlayer = nullptr;
		firstAddType = PlayerType::NONE;
	}
#pragma endregion

}

void Player::draw()
{
	if(!isMuteki)
	Library::drawGraphic(vertexHandle, heapHandle, 0);
	else 
	{
		if (mutekiTimer % 2 == 0) 
		{
			Library::drawGraphic(vertexHandle, heapHandle, 0);
		}
	}

}

void Player::hit(Object* object, CollosionType collisionType)
{
	if (typeid(*object) == typeid(Player))
	{
	/*	if (leftPlayerVelocity.x == 0 &&
			leftPlayerVelocity.z == 0 &&
			rightPlayerVelocity.x == 0 &&
			rightPlayerVelocity.z == 0)
			firstAddPlayer->addPosition({ 0,0,0.5f });*/

		//めり込み対策
		kasanariTimer++;
		if(kasanariTimer >= 60 * 0.2)
			firstAddPlayer->addPosition({ 0,0,0.5f });
		hitOtherPlayer = true;
		//他のプレイヤー押せるようにする?


		setPosition(position + velocity * -1 * speed);

	}

	if (isMuteki)return;
	if (typeid(*object) == typeid(Enemy)) 
	{
		//Enemy* e = static_cast<Enemy*>(object->getPtr());
		/*if (e->GetMyShot())return;*/
		life--;
		isMuteki = true;
	}

}

void Player::setPosition(Vector3 pos)
{
	position = pos;
	Library::setPosition(position, heapHandle, 0);
	sphereData[0].position = position;

	areaPush();

	if (playerType == PlayerType::LEFT)leftPlayerPosition = pos;
	if (playerType == PlayerType::RIGHT)rightPlayerPosition = pos;
}

void Player::addPosition(const Vector3& vec)
{
	position += vec;
	Library::setPosition(position, heapHandle, 0);
	sphereData[0].position = position;

	if (position.x >= 20) position.x -= vec.x;
	if (position.x <= -20) position.x += vec.x;
	if (position.z >= 15) position.z -= vec.z;
	if (position.z <= -15) position.z += vec.z;


	if (playerType == PlayerType::LEFT)leftPlayerPosition = position;
	if (playerType == PlayerType::RIGHT)rightPlayerPosition = position;
}

void Player::areaPush() 
{	
	//動いてないときに出る(押されたりして)可能性があるから、velocity使わない

	if (position.x > 20 || position.x < -20)velocity.x = 0;
	if (position.z > 15 || position.z < -15)velocity.z = 0;
	if (position.x > 20) position.x = 20;
	if (position.x < -20) position.x = -20;
	if (position.z > 15) position.z = 15;
	if (position.z < -15) position.z = -15;
	
	if (playerType == PlayerType::LEFT)leftPlayerPosition = position;
	if (playerType == PlayerType::RIGHT)rightPlayerPosition = position;


}

int Player::getLife()
{
	return life;
}


Vector3 Player::getVelocity()
{
	return velocity;
}

bool Player::getIsDash()
{
	return isDash;
}

bool Player::getDeadPlayer()
{
	return deadPlayer;
}

Player::PlayerType Player::getPlayerType()
{
	return playerType;
}
