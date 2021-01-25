#include "StageSelect.h"
#include"SceneChange.h"
#include"SceneManager.h"

#include"Play.h"

#include"Stage.h"

StageSelect::StageSelect()
{
	for (int i = 0; i < _countof(stageNumBoardSpr); i++)
		Library::createSprite(&stageNumBoardSpr[i]);
	for (int i = 0; i < _countof(stageNumRubberSpr); i++)
		Library::createSprite(&stageNumRubberSpr[i]);

	stageNumBoardTex[0] = Library::loadTexture(L"Resources/Texture/numBoard1.png");
	stageNumBoardTex[1] = Library::loadTexture(L"Resources/Texture/numBoard2.png");
	stageNumBoardTex[2] = Library::loadTexture(L"Resources/Texture/numBoard3.png");
	stageNumBoardTex[3] = Library::loadTexture(L"Resources/Texture/numBoard4.png");
	stageNumBoardTex[4] = Library::loadTexture(L"Resources/Texture/numBoard5.png");

	stageNumRubberTex = Library::loadTexture(L"Resources/Texture/rubberSpr.png");

	stage = 0;
}

StageSelect::~StageSelect()
{
}

void StageSelect::initialize()
{
	for (int i = 0; i < _countof(stageNumPos); i++)
		stageNumPos[i] = { 380 + 700 * (float)i,260 };
	stageNumChangeTimer = 0;
	stageNumUpDown = STAGE_NUM_NOT_CHANGE;
	moveStageNumPos = 0;
}

void StageSelect::update()
{
#pragma region ステージ切り替え
	if (stageNumUpDown == STAGE_NUM_NOT_CHANGE)
	{
		if (DirectInput::keyState(DIK_LEFT) ||
			DirectInput::directionalButtonState(LeftButton) ||
			DirectInput::leftStickLeft(30000))
		{
			if (stage > 0) 
			{
				stage--;
				stageNumUpDown = STAGE_NUM_DOWN;
			}
		}

		if (DirectInput::keyState(DIK_RIGHT) ||
			DirectInput::directionalButtonState(RightButton) ||
			DirectInput::leftStickRight(30000))
		{
			if (stage < 4) 
			{
				stage++;
				stageNumUpDown = STAGE_NUM_UP;
			}
		}
	}

	//インクリメント
	if (stageNumUpDown != STAGE_NUM_NOT_CHANGE) 
	{
		stageNumChangeTimer++;

		if (stageNumChangeTimer < StageNumChangeTime / 2 &&
			moveStageNumPos < 700)
		{
			if (stageNumUpDown == STAGE_NUM_UP)
				for (int i = stage - 1; i > -1; i--)
					stageNumPos[i].x -= 35;

			if (stageNumUpDown == STAGE_NUM_DOWN)
				for (int i = stage + 1; i < _countof(stageNumPos); i++)
					stageNumPos[i].x += 35;

			moveStageNumPos += 35;
		}

		if (stageNumChangeTimer >= StageNumChangeTime / 2 &&
			moveStageNumPos < 700 * 2)
		{
			if (stageNumUpDown == STAGE_NUM_UP)
				for (int i = stage; i < _countof(stageNumPos); i++)
					stageNumPos[i].x -= 35;

			if (stageNumUpDown == STAGE_NUM_DOWN)
				for (int i = stage; i > -1; i--)
					stageNumPos[i].x += 35;

			moveStageNumPos += 35;
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
			DirectInput::keyTrigger(DIK_RETURN) ||
			DirectInput::buttonTrigger(AButton) ||
			DirectInput::buttonTrigger(StartButton))
			SceneChange::getInstance()->trueFeadFlag();
	}
	SceneChange::getInstance()->update();
	if (SceneChange::getInstance()->getSceneChangeFlag())isEnd = true;

#pragma endregion
}

void StageSelect::draw()
{
	for (int i = 0; i < _countof(stageNumRubberSpr); i++)
		Library::drawBox({ stageNumPos[i].x ,stageNumPos[i].y + 110 }, { stageNumPos[i + 1].x - stageNumPos[i].x,30 }, { 233,233,233,255 }, stageNumRubberSpr[i]);

	for (int i = 0; i < _countof(stageNumBoardSpr); i++)
		Library::drawSprite(stageNumPos[i],  stageNumBoardSpr[i],&stageNumBoardTex[i]);


	//シーン遷移
	SceneChange::getInstance()->draw();


}

void StageSelect::end()
{
	Stage::getInstance()->initialize(stage);
}

std::string StageSelect::nextScene()
{
	return "Play";
}
