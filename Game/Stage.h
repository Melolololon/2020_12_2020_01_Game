#pragma once
#include"Library.h"
#include"Player.h"

//ステージクラス。敵の配置などのステージごとに違うものはここに書く
class Stage
{
private:
	Stage();
	~Stage();

	//ステージ番号
	int stage;
	sprite stageNumSpr;

	//チュートリアルメッセージ
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

	//次のメッセージへ進められるようになる時間
	int nextPossibleTimer;
	static const int NextPossibleTime;

	//クリアに必要な討伐数
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

