#pragma once
#include"Library.h"
#include"Player.h"

//�X�e�[�W�N���X�B�G�̔z�u�Ȃǂ̃X�e�[�W���ƂɈႤ���̂͂����ɏ���
class Stage
{
private:
	Stage();
	~Stage();

	//�X�e�[�W�ԍ�
	int stage;
	sprite stageNumSpr;

	//�`���[�g���A�����b�Z�[�W
	sprite tMesSpr;
	texture tMesTexKey[3];
	texture tMesTexPad[3];
	int tMesNum;
	bool tutorialFlag;
	int skipTimer;
	static const int SkipTime;
	bool leftInput;
	bool rightInput;
	int stageStartTimer;
	static const int StageStartTime;

	Player* player[2];

	//���̃��b�Z�[�W�֐i�߂���悤�ɂȂ鎞��
	int nextPossibleTimer;
	static const int NextPossibleTime;

	//�N���A�ɕK�v�ȓ�����
	int stageClearEnemyNum;

	bool clearFlag;

	int enemyDeadCount;
	int pureEnemyDeadCount;
	std::vector<Object*> enemys;
	int addEnemyCount;
	void addEnemy();


#pragma region ���f��

	vertex fierdVertexH[2];
	heap fierdHeapH[2];

#pragma endregion

	sprite greenSpr;

	//�J����
	static const Vector3 StartCameraPos;
	Vector3 cameraPos;
	Vector3 cameraAngle;
	float rotateAngle;
	
	//�v���C���̎��_
	static const Vector3 PlayCameraPos;

	//�{�X��̍ŏ��̎��_
	//1�{�X
	static const Vector3 Boss1StartCameraPos;
	//�{�X2
	static const Vector3 Boss2StartCameraPos;
public:
	Stage(Stage& s) = delete;
	Stage& operator=(Stage& s) = delete;
	static Stage* getInstance();

	void loadModelAndTexture();
	void initialize(const int& stageNum);
	void setPlayer(Player* p[]);
	void update();
	void draw();
	void end();

	bool getTutorialFlag();

	bool getClearFlag();

};

