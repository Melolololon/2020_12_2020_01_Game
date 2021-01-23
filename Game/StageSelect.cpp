#include "StageSelect.h"
#include"SceneChange.h"

StageSelect::StageSelect()
{
	for (int i = 0; i < _countof(stageNumSpr); i++)
		Library::createSprite(&stageNumSpr[i]);
	for (int i = 0; i < _countof(stageNumRubberSpr); i++)
		Library::createSprite(&stageNumRubberSpr[i]);


}

StageSelect::~StageSelect()
{
}

void StageSelect::initialize()
{
	stageNum = 0;
	for (int i = 0; i < _countof(stageNumPos); i++)
		stageNumPos[i] = { 460 + 400 * (float)i,320 };
	stageNumChangeTimer = 0;
	stageNumUpDown = STAGE_NUM_NOT_CHANGE;
	moveStageNumPos = 0;
}

void StageSelect::update()
{
#pragma region ステージ切り替え
	if (stageNumUpDown == STAGE_NUM_NOT_CHANGE)
	{
		if (DirectInput::keyTrigger(DIK_LEFT) ||
			DirectInput::directionalButtonTrigger(LeftButton))
		{
			if (stageNum > 0) 
			{
				stageNum--;
				stageNumUpDown = STAGE_NUM_DOWN;
			}
		}

		if (DirectInput::keyTrigger(DIK_RIGHT) ||
			DirectInput::directionalButtonTrigger(RightButton))
		{
			if (stageNum < 4) 
			{
				stageNum++;
				stageNumUpDown = STAGE_NUM_UP;
			}
		}
	}

	//インクリメント
	if (stageNumUpDown != STAGE_NUM_NOT_CHANGE) 
	{
		stageNumChangeTimer++;

		if (stageNumChangeTimer < StageNumChangeTime / 2 &&
			moveStageNumPos < 400)
		{
			if (stageNumUpDown == STAGE_NUM_UP)
				for (int i = stageNum - 1; i > -1; i--)
					stageNumPos[i].x -= 20;

			if (stageNumUpDown == STAGE_NUM_DOWN)
				for (int i = stageNum + 1; i < _countof(stageNumPos); i++)
					stageNumPos[i].x += 20;

			moveStageNumPos += 20;
		}

		if (stageNumChangeTimer >= StageNumChangeTime / 2 &&
			moveStageNumPos < 800)
		{
			if (stageNumUpDown == STAGE_NUM_UP)
				for (int i = stageNum; i < _countof(stageNumPos); i++)
					stageNumPos[i].x -= 20;

			if (stageNumUpDown == STAGE_NUM_DOWN)
				for (int i = stageNum; i > -1; i--)
					stageNumPos[i].x += 20;

			moveStageNumPos += 20;
		}

		if (stageNumChangeTimer >= StageNumChangeTime)
		{
			stageNumChangeTimer = 0;
			moveStageNumPos = 0;
			stageNumUpDown = STAGE_NUM_NOT_CHANGE;
		}
	}
	
#pragma endregion


#pragma region シーン遷移
	if (stageNumUpDown == STAGE_NUM_NOT_CHANGE)
	{
		if (DirectInput::keyTrigger(DIK_SPACE) || 
			DirectInput::buttonTrigger(AButton))
			SceneChange::getInstance()->trueFeadFlag();
	}
	SceneChange::getInstance()->update();
	if (SceneChange::getInstance()->getSceneChangeFlag())isEnd = true;

#pragma endregion
}

void StageSelect::draw()
{
	for (int i = 0; i < _countof(stageNumRubberSpr); i++)
		Library::drawBox({ stageNumPos[i].x ,stageNumPos[i].y + 60 }, { stageNumPos[i + 1].x - stageNumPos[i].x,30 }, { 233,233,233,255 }, stageNumRubberSpr[i]);

	for (int i = 0; i < _countof(stageNumSpr); i++)
		Library::drawBox(stageNumPos[i], { 300,150 }, { 255,0,0,255 }, stageNumSpr[i]);


	//シーン遷移
	SceneChange::getInstance()->draw();


}

void StageSelect::end()
{
}

std::string StageSelect::nextScene()
{
	return "Play";
}
