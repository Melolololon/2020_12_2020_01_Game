#pragma once
#include"Object.h"
#include"LibMath.h"
#include"Vector.h"
#include"Rubber.h"
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

	Vector3 angle;

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
	const float PlayerRevDidtanceNumber = 10.0f;
	const float PlayerMaxDistance = LibMath::calcDistance3D({ PlayerMaxDistanceNumber,0,0 }, { 0,0,0 });//25��max
	const float PlayerRevDistance = LibMath::calcDistance3D({ PlayerRevDidtanceNumber,0,0 }, { 0,0,0 });//15��rev

	//���͂������ǂ���
	bool inputFlag;

	//���ȏ�(����Ɉ���������܂�)����Ă��邩
	static bool leavePlayer;

	//�̗�
	int life;

	//���G����
	bool isMuteki;
	int mutekiTimer;
	const int MutekiTime = 60 * 1;

	//�d�Ȃ�h�~
	int kasanariTimer;
	bool hitOtherPlayer;

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

	/// <summary>
	/// �G���A�ɖ߂�
	/// </summary>
	void areaPush();

	//�^�C�g���Ƀv���C���[�\��������?
	//�R�L�΂�����k�߂��肳����?
	//�`���[�g���A��1/24�ɍ��

	//�ǂ������Е����񂾂��ǂ���
	static bool deadPlayer;

	//���֌W
	/*static vertex arrowVertexH;
	static heap arrowHeapH;
	static Vector3 hitEnemyVector;*/
public:


	Player( const Vector3& pos, const PlayerType& playerType);
	~Player();
	void update();
	void draw();
	void hit(Object* object, CollisionType collisionType)override;

	void setPosition(Vector3 pos);
	static void setEnemyVector(const Vector3& vec);

	void addPosition(const Vector3& vec);

	int getLife();

	Vector3 getVelocity();
	bool getIsDash();

	/// <summary>
	/// ���f���A�X�v���C�g����
	/// </summary>
	static void loadModel();

	static bool getDeadPlayer();

	PlayerType getPlayerType();
	

};

