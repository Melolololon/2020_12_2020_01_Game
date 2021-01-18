#include "Player.h"


Vector3 Player::leftPlayerPosition;
Vector3 Player::rightPlayerPosition;
Vector3 Player::leftPlayerVelocity;
Vector3 Player::rightPlayerVelocity;
Player::PlayerType Player::firstAddType;
Player* Player::firstAddPlayer;

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

	if (playerType == PlayerType::LEFT)Library::createHeapData2({ 0,255,255,255 }, 1, &heapHandle);
	if (playerType == PlayerType::RIGHT)Library::createHeapData2({ 255,0,0,255 }, 1, &heapHandle);

	collisionFlag.board = false;
	collisionFlag.lay = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = true;

	this->playerType = playerType;

#pragma region �_�b�V��
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


#pragma region �S��


	//�C���v�b�g���C�A�E�g�ݒ�
	Library::setInputLayout("POSITION", 3);
	Library::setInputLayout("NORMAL", 3);
	Library::setInputLayout("ADDPOSITION", 3);

#pragma endregion

}


Player::~Player()
{
}

void Player::update()
{
	//���ȏ��������ƈ������鑬�x��������(�S�����������Ă邩��)
	//����ɂ��A��������Ǝ󂯎~�߂₷���Ȃ邪�A�V���b�g�Ɏ��Ԃ�������
	//(��������̂����Ԃ����邵�A�G���S������������̂ɂ����Ԃ�����)

#pragma region �ړ�����

	float playerDistance;
	playerDistance = LibMath::calcDictance3D(leftPlayerPosition, rightPlayerPosition);

	Vector3 firstAddPlayerVector;//��ɒǉ����ꂽ�v���C���[�ւ̃x�N�g��
	Vector3 reversVector;//��̃x�N�g���̋t�x�N�g��
	if (playerType == PlayerType::LEFT)firstAddPlayerVector = LibMath::otherVector(leftPlayerPosition, rightPlayerPosition);
	if (playerType == PlayerType::RIGHT)firstAddPlayerVector = LibMath::otherVector(rightPlayerPosition, leftPlayerPosition);
	//�������������̂̃v���C���[�ւ̃x�N�g��(firstAddPlayerVector�̋t�x�N�g��)
	reversVector = firstAddPlayerVector * -1;

#pragma region �ړ�

	if (!isDash) 
	{
		velocity = 0;

		if (playerType == PlayerType::LEFT)
		{
			if (DirectInput::leftStickUp(30000) || DirectInput::keyState(DIK_W)) 
				velocity.z++; 
			if (DirectInput::leftStickDown(30000) || DirectInput::keyState(DIK_S)) 
				velocity.z--;
			if (DirectInput::leftStickRight(30000) || DirectInput::keyState(DIK_D)) 
				velocity.x++;
			if (DirectInput::leftStickLeft(30000) || DirectInput::keyState(DIK_A)) 
				velocity.x--; 

		}

		if (playerType == PlayerType::RIGHT)
		{
			if (DirectInput::rightStickUp(30000) || DirectInput::keyState(DIK_UP))
				velocity.z++;
			if (DirectInput::rightStickDown(30000) || DirectInput::keyState(DIK_DOWN)) 
				velocity.z--;
			if (DirectInput::rightStickRight(30000) || DirectInput::keyState(DIK_RIGHT))
				velocity.x++;
			if (DirectInput::rightStickLeft(30000) || DirectInput::keyState(DIK_LEFT)) 
				velocity.x--;
		}
	}
#pragma endregion

#pragma region �_�b�V��

	if (!isDash ) 
	{
		if (playerType == PlayerType::LEFT && !dashOperTimeFlag)
		{
			if (DirectInput::leftStickUp(32767) || DirectInput::keyState(DIK_W))
			{
				dashOperTimeFlag = true;
				leftInputDirectionUpDown = InputDirection::INPUT_DIRECTION_UP;
			}
			if (DirectInput::leftStickDown(32767) || DirectInput::keyState(DIK_S))
			{
				dashOperTimeFlag = true;
				leftInputDirectionUpDown = InputDirection::INPUT_DIRECTION_DOWN;
			}
			if (DirectInput::leftStickRight(32767) || DirectInput::keyState(DIK_D))
			{
				dashOperTimeFlag = true;
				leftInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_RIGHT;
			}
			if (DirectInput::leftStickLeft(32767) || DirectInput::keyState(DIK_A))
			{
				dashOperTimeFlag = true;
				leftInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_LEFT;
			}
		}
		if (playerType == PlayerType::RIGHT && !dashOperTimeFlag)
		{
			if (DirectInput::rightStickUp(32767) || DirectInput::keyState(DIK_UP))
			{
				dashOperTimeFlag = true;
				rightInputDirectionUpDown = InputDirection::INPUT_DIRECTION_UP;
			}
			if (DirectInput::rightStickDown(32767) || DirectInput::keyState(DIK_DOWN))
			{
				dashOperTimeFlag = true;
				rightInputDirectionUpDown = InputDirection::INPUT_DIRECTION_DOWN;
			}
			if (DirectInput::rightStickRight(32767) || DirectInput::keyState(DIK_RIGHT))
			{
				dashOperTimeFlag = true;
				rightInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_RIGHT;
			}
			if (DirectInput::rightStickLeft(32767) || DirectInput::keyState(DIK_LEFT))
			{
				dashOperTimeFlag = true;
				rightInputDirectionLeftRight = InputDirection::INPUT_DIRECTION_LEFT;
			}
		}

		//���͉\���ԓ�
		if (dashOperTimeFlag)
		{
			dashOperTimer++;
			//���͌�A�ړ����Ȃ��Ȃ�܂Ńp�b�h�𒆐S�Ɉړ���������_�b�V���\��
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

		//���Ԉȓ������͉\���ɓ��͂��ꂽ��
		if (dashOperTimeFlag && dashOperPossibleFlag)
		{
			//�ē��͂��ꂽ��_�b�V��
			if (playerType == PlayerType::LEFT)
			{
				if (DirectInput::leftStickUp(32767) || DirectInput::keyState(DIK_W))
				{
					if(leftInputDirectionUpDown == InputDirection::INPUT_DIRECTION_UP)
					isDash = true;
				}
				if (DirectInput::leftStickDown(32767) || DirectInput::keyState(DIK_S))
				{
					if (leftInputDirectionUpDown == InputDirection::INPUT_DIRECTION_DOWN)
					isDash = true;
				}
				if (DirectInput::leftStickRight(32767) || DirectInput::keyState(DIK_D))
				{
					if (leftInputDirectionLeftRight == InputDirection::INPUT_DIRECTION_RIGHT)
					isDash = true;
				}
				if (DirectInput::leftStickLeft(32767) || DirectInput::keyState(DIK_A))
				{
					if (leftInputDirectionLeftRight == InputDirection::INPUT_DIRECTION_LEFT)
					isDash = true;
				}
			}

			if (playerType == PlayerType::RIGHT)
			{
				if (DirectInput::rightStickUp(32767) || DirectInput::keyState(DIK_UP))
				{
					if (rightInputDirectionUpDown == InputDirection::INPUT_DIRECTION_UP)
					isDash = true;
				}
				if (DirectInput::rightStickDown(32767) || DirectInput::keyState(DIK_DOWN))
				{
					if (rightInputDirectionUpDown == InputDirection::INPUT_DIRECTION_DOWN)
					isDash = true;
				}
				if (DirectInput::rightStickRight(32767) || DirectInput::keyState(DIK_RIGHT))
				{
					if (rightInputDirectionLeftRight == InputDirection::INPUT_DIRECTION_RIGHT)
					isDash = true;
				}
				if (DirectInput::rightStickLeft(32767) || DirectInput::keyState(DIK_LEFT))
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

#pragma region �S���ɂ���������ꏈ��

	//15�ȏ㗣�ꂽ�����������
	if (playerDistance > PlayerRevDistance)
	{
		position += firstAddPlayerVector * speed * 0.5;
	}
#pragma endregion

#pragma region �ʏ�̍��W�Z�b�g

	position += velocity * speed;
	Library::setPosition(position, heapHandle, 0);
	sphereData[0].position = position;

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

#pragma region �����̖h�~
	//�ŏ��ɒǉ�������ł͍s��Ȃ�
	//���������Ȃ��ƁA�Е������������Ă������Ȃ��Ȃ�
	//(�ړ��������Е��ŏI��邽��)
	//�Е���speed���߂����炱�������Ȃ��Ƃ����Ȃ������ŁA
	//������speed�̔����������߂��΂����̂ł�?
	//�Е��������Ă�Ƃ��A����I�Ɉ������肽��(�����Ă�ق��͖߂���Ȃ��悤�ɂ�����)�̂ŁA��������
	if (firstAddType != playerType)
	{
		while (1)
		{
			playerDistance = LibMath::calcDictance3D(leftPlayerPosition, rightPlayerPosition);

			if (playerType == PlayerType::LEFT)firstAddPlayerVector = LibMath::otherVector(leftPlayerPosition, rightPlayerPosition);
			if (playerType == PlayerType::RIGHT)firstAddPlayerVector = LibMath::otherVector(rightPlayerPosition, leftPlayerPosition);
			//�������������̂̃v���C���[�ւ̃x�N�g��(firstAddPlayerVector�̋t�x�N�g��)
			reversVector = firstAddPlayerVector * -1;

			//�v���C���[���m�̋��������E�n�𒴂��Ă�����
			if (playerDistance > PlayerMaxDistance)
			{

				//���W��ύX(�����Ȃ��������������)
				if (playerType == PlayerType::LEFT)
				{
					//�Е�������
					if (velocity.x == 0 && velocity.z == 0)
						setPosition(position + firstAddPlayerVector * speed);

					//����������
					if (velocity.x != 0 || velocity.z != 0)
					{
						if (rightPlayerVelocity.x != 0 || rightPlayerVelocity.z != 0)
						setPosition(position + firstAddPlayerVector * speed);
					}
				}
				if (playerType == PlayerType::RIGHT)
				{
					//�Е�������
					if (velocity.x == 0 && velocity.z == 0)
						setPosition(position + firstAddPlayerVector * speed);

					//����������
					if (velocity.x != 0 || velocity.z != 0)
					{
						if (leftPlayerVelocity.x != 0 || leftPlayerVelocity.z != 0)
							setPosition(position + firstAddPlayerVector * speed);
					}
				}


				//�ŏ��ɒǉ������v���C���[�̍��W��ύX
				if (firstAddType == PlayerType::LEFT)
				{
					//�Е�������
					if (leftPlayerVelocity.x == 0 && leftPlayerVelocity.z == 0)
						firstAddPlayer->setPosition(leftPlayerPosition + reversVector * speed);

					//����������
					if (leftPlayerVelocity.x != 0 || leftPlayerVelocity.z != 0)
					{
						if (rightPlayerVelocity.x != 0 || rightPlayerVelocity.z != 0)
							firstAddPlayer->setPosition(leftPlayerPosition + reversVector * speed);
					}
				}
				if (firstAddType == PlayerType::RIGHT)
				{
					//�Е�������
					if (rightPlayerVelocity.x == 0 && rightPlayerVelocity.z == 0)
						firstAddPlayer->setPosition(rightPlayerPosition + reversVector * speed);

					//����������
					if (rightPlayerVelocity.x != 0 || rightPlayerVelocity.z != 0)
					{
						if (leftPlayerVelocity.x != 0 || leftPlayerVelocity.z != 0)
							firstAddPlayer->setPosition(rightPlayerPosition + reversVector * speed);
					}
				}

				
			}
			else//���E�l�ȓ��ɂȂ�����ړ��I�� 
			{
				break;
			}
		}
	}
#pragma endregion


#pragma endregion

}

void Player::draw()
{
	Library::drawGraphic(vertexHandle, heapHandle, 0);
}

void Player::hit(Object* object, CollosionType collisionType)
{
	if (typeid(*object) == typeid(Player))
	{
		setPosition(position + velocity * -1 * speed);
	}
}

void Player::setPosition(Vector3 pos)
{
	position = pos;
	Library::setPosition(position, heapHandle, 0);
	sphereData[0].position = position;

	if (playerType == PlayerType::LEFT)leftPlayerPosition = pos;
	if (playerType == PlayerType::RIGHT)rightPlayerPosition = pos;
}
