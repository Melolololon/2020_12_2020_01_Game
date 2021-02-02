#pragma once
#include"Library.h"
#include <Scene.h>
class StageSelect :
	public Scene
{
private:
	enum StageNumUpDown 
	{
		STAGE_NUM_NOT_CHANGE,
		STAGE_NUM_UP,
		STAGE_NUM_DOWN,

	};
	
	//ステージ関係
	int stage;
	sprite stageNumBoardSpr[7];
	texture stageNumBoardTex[7];
	Vector2 stageNumPos[7];
	int stageNumChangeTimer;
	const int StageNumChangeTime = 60 * 0.8f;
	StageNumUpDown stageNumUpDown;
	float moveStageNumPos;
	sprite stageNumRubberSpr[6];
	texture stageNumRubberTex;


	Vector3 cameraAngle;
	sprite greenSpr;

	bool titleBack;
	int titleBackTimer;
	static const int TitleBackTime;

	sprite selectStrSpr;
	texture selectStrTex;
public:
	StageSelect();
	~StageSelect();

	void initialize();
	void update();
	void draw();
	void end();
	std::string nextScene();
};

