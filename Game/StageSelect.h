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
	int stageNum;
	sprite stageNumSpr[5];
	Vector2 stageNumPos[5];
	int stageNumChangeTimer;
	const int StageNumChangeTime = 60 * 0.8f;
	StageNumUpDown stageNumUpDown;
	float moveStageNumPos;
	sprite stageNumRubberSpr[4];

public:
	StageSelect();
	~StageSelect();

	void initialize();
	void update();
	void draw();
	void end();
	std::string nextScene();
};

