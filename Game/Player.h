#pragma once
#include"Object.h"
#include"LibMath.h"
#include"Vector.h"
class Player : public Object
{
public:
	enum PlayerType
	{
		NONE,
		LEFT,
		RIGHT,
	};

private:

	enum InputDirection
	{
		INPUT_DIRECTION_CENTER,
		INPUT_DIRECTION_UP,
		INPUT_DIRECTION_DOWN,
		INPUT_DIRECTION_LEFT,
		INPUT_DIRECTION_RIGHT,
	};

	static PlayerType firstAddType;
	static Vector3 leftPlayerPosition;
	static Vector3 rightPlayerPosition;
	static Vector3 leftPlayerVelocity;
	static Vector3 rightPlayerVelocity;
	static Player* firstAddPlayer;

	//�����̃v���C���[�^�C�v(L��R��)
	PlayerType playerType;

#pragma region �_�b�V��
	//����������2����͂���ƃ_�b�V��

	//���͕���
	InputDirection leftInputDirectionUpDown;
	InputDirection leftInputDirectionLeftRight;
	InputDirection rightInputDirectionUpDown;
	InputDirection rightInputDirectionLeftRight;

	//�_�b�V�����͉\���ǂ���(�X�e�B�b�N�����ʒu�܂Ŗ߂��Ȃ���true�ɂȂ�Ȃ�)
	bool dashOperPossibleFlag;
	//���͎�t���Ԃ��ǂ���
	bool dashOperTimeFlag;
	//�_�b�V�����邽�߂̓��ڂ̃X�e�B�b�N���͎�t����
	int dashOperTimer;
	const int DashOperTime = 60.0f * 0.2f;

	//�_�b�V������
	int dashTimer;
	const int DashTime = 60.0f * 0.25f;

	//�_�b�V�����Ă邩�ǂ���
	bool isDash;
	//�ō��_�b�V���X�s�[�h
	const Vector3 MaxDashSpeed = { 1.0f,1.0f,1.0f };
	//�ʏ�X�s�[�h
	const Vector3 NormalSpeed = { 0.2f,0.2f,0.2f };
#pragma endregion


	//�ō�����(playerMaxDistanceNumber�ȏ�͗���邱�Ƃ��ł��Ȃ�)
	const float PlayerMaxDistanceNumber = 20.0f;
	//�����Е��̃v���C���[�ւ̗͂���������
	const float PlayerRevDidtanceNumber = 7.5f;
	const float PlayerMaxDistance = LibMath::calcDistance3D({ PlayerMaxDistanceNumber,0,0 }, { 0,0,0 });//25��max
	const float PlayerRevDistance = LibMath::calcDistance3D({ PlayerRevDidtanceNumber,0,0 }, { 0,0,0 });//15��rev

#pragma region �R


	vertex lineVertexHandle;
	heap lineHeapHandle;

	//�X�v���C���̐���_�̍��W
	Vector3 linePos[9];

	struct LineVertex 
	{
		//���W�@�@���@�G���U�ꂽ���Ƃɂ��ړ���
		Vector3 pos, normal, addPos;
	};
	//���_�f�[�^
	std::vector<LineVertex>lineVertexData;

#pragma endregion


public:


	Player( const Vector3& pos, const PlayerType& playerType);
	~Player();

	void update();
	void draw();
	void hit(Object* object, CollosionType collisionType)override;

	void setPosition(Vector3 pos);
};

