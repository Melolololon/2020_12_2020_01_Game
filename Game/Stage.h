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
	int stageClearEnemyNum;

	bool clearFlag;

	int enemyDeadCount;
	int pureEnemyDeadCount;
	std::vector<Object*> enemys;
	int addEnemyCount;
	void addEnemy();


#pragma region モデル

	vertex fierdVertexH[2];
	heap fierdHeapH[2];

#pragma endregion

	sprite greenSpr;

	//カメラ
	static const Vector3 StartCameraPos;
	Vector3 cameraPos;
	Vector3 cameraAngle;
	float rotateAngle;
	
	//プレイ中の視点
	static const Vector3 PlayCameraPos;

	//ボス戦の最初の視点
	//1ボス
	static const Vector3 Boss1StartCameraPos;
	//ボス2
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

