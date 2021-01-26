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
	static const int StageClearEnemyNum[5];

	bool clearFlag;
	int addEnemyCount;
public:
	Stage(Stage& s) = delete;
	Stage& operator=(Stage& s) = delete;
	static Stage* getInstance();

	void createSpriteLoadTexture();
	void initialize(const int& stageNum);
	void setPlayer(Player* p[]);
	void update();
	void draw();
	
	bool gettutorialFlag();

	bool getClearFlag();
};

