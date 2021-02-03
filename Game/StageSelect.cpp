#include "StageSelect.h"
#include"SceneChange.h"
#include"SceneManager.h"
#include"PolygonManager.h"

#include"Play.h"

#include"Stage.h"

#include"XInputManager.h"

const int StageSelect::TitleBackTime = 60 * 30;

StageSelect::StageSelect()
{
	for (int i = 0; i < _countof(stageNumBoardSpr); i++)
		Library::createSprite(&stageNumBoardSpr[i]);
	for (int i = 0; i < _countof(stageNumRubberSpr); i++)
		Library::createSprite(&stageNumRubberSpr[i]);

	stageNumBoardTex[0] = Library::loadTexture(L"Resources/Texture/numBoard1.png");
	stageNumBoardTex[1] = Library::loadTexture(L"Resources/Texture/numBoard2.png");
	stageNumBoardTex[2] = Library::loadTexture(L"Resources/Texture/numBoard3.png");
	stageNumBoardTex[3] = Library::loadTexture(L"Resources/Texture/numBoard4Boss.png");
	stageNumBoardTex[4] = Library::loadTexture(L"Resources/Texture/numBoard5.png");
	stageNumBoardTex[5] = Library::loadTexture(L"Resources/Texture/numBoard6.png");
	stageNumBoardTex[6] = Library::loadTexture(L"Resources/Texture/numBoard7Boss.png");
	for (int i = 0; i < _countof(stageNumBoardTex); i++)
		PolygonManager::getInstance()->addTexture("stageNum" + i, stageNumBoardTex[i]);

	stageNumRubberTex = Library::loadTexture(L"Resources/Texture/rubberSpr.png");

	stage = 0;
	Library::createSprite(&selectStrSpr);
	selectStrTex = Library::loadTexture(L"Resources/Texture/select.png");
}

StageSelect::~StageSelect()
{
}

void StageSelect::initialize()
{
	float count = -stage;
	for (int i = 0; i < _countof(stageNumPos); i++) 
	{
		//stageNumPos[i] = { 380 + 700 * (float)i ,260 };
		stageNumPos[i] = { 380  + 700 * count ,260 };
		count++;
	}
	stageNumChangeTimer = 0;
	stageNumUpDown = STAGE_NUM_NOT_CHANGE;
	moveStageNumPos = 0;

	Library::setCamera({ 0,35,-22 }, { 0,-3,0 }, { 0,1,0 });
	Library::setCameraMatrixPoint({ 0,35,-22 }, { 0,-3,0 }, { 0,1,0 });
	cameraAngle = 0;

	titleBack = false;
	titleBackTimer = 0;


	Library::playLoadSound("stage");
}

void StageSelect::update()
{
#pragma region ステージ切り替え
	if (stageNumUpDown == STAGE_NUM_NOT_CHANGE)
	{
		if (DirectInput::keyState(DIK_LEFT) ||
			XInputManager::buttonState(XInputManager::XINPUT_LEFT_BUTTON,1) ||
			XInputManager::leftStickLeft(30000,1))
		{
			if (stage > 0) 
			{
				stage--;
				stageNumUpDown = STAGE_NUM_DOWN;
			}
		}

		if (DirectInput::keyState(DIK_RIGHT) ||
			XInputManager::buttonState(XInputManager::XINPUT_RIGHT_BUTTON, 1) ||
			XInputManager::leftStickRight(30000, 1))
		{
			if (stage < 6) 
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

	Library::setCameraAngle(cameraAngle, { 0,0,0 }, { 0,0,0 });
	cameraAngle.y += 0.5f;
	cameraAngle.y = cameraAngle.y >= 360 ? 0 : cameraAngle.y;


#pragma region シーン遷移



	if (stageNumUpDown == STAGE_NUM_NOT_CHANGE)
	{
		if (DirectInput::keyTrigger(DIK_SPACE) ||
			DirectInput::keyTrigger(DIK_RETURN) ||
			XInputManager::buttonTrigger(XInputManager::XINPUT_A_BUTTON,1) ||
			XInputManager::buttonTrigger(XInputManager::XINPUT_START_BUTTON, 1))
			SceneChange::getInstance()->trueFeadFlag();

		if (DirectInput::keyTrigger(DIK_ESCAPE) ||
			XInputManager::buttonTrigger(XInputManager::XINPUT_BACK_BUTTON, 1) ||
			XInputManager::buttonTrigger(XInputManager::XINPUT_B_BUTTON, 1))
		{
			titleBack = true;
			SceneChange::getInstance()->trueFeadFlag();
		}

	}
	SceneChange::getInstance()->update();
	if (SceneChange::getInstance()->getSceneChangeFlag())isEnd = true;

#pragma endregion

	if (!DirectInput::keyState(DIK_LEFT) &&
		!DirectInput::keyState(DIK_RIGHT) &&
		!XInputManager::buttonState(XInputManager::XINPUT_LEFT_BUTTON, 1) &&
		!XInputManager::leftStickLeft(30000, 1) &&
		!XInputManager::buttonState(XInputManager::XINPUT_RIGHT_BUTTON, 1) &&
		!XInputManager::leftStickRight(30000, 1) &&
		!DirectInput::keyState(DIK_SPACE) &&
		!DirectInput::keyState(DIK_RETURN) && 
		!DirectInput::keyState(DIK_ESCAPE) &&
		!XInputManager::buttonState(XInputManager::XINPUT_A_BUTTON, 1) &&
		!XInputManager::buttonState(XInputManager::XINPUT_B_BUTTON, 1) &&
		!XInputManager::buttonState(XInputManager::XINPUT_START_BUTTON, 1) &&
		!XInputManager::buttonState(XInputManager::XINPUT_BACK_BUTTON, 1) 
		)
		titleBackTimer++;
	else
		titleBackTimer = 0;

	if(titleBackTimer >= TitleBackTime)
	{
		titleBack = true;
		SceneChange::getInstance()->trueFeadFlag();
	}
}

void StageSelect::draw()
{
	vertex v;
	heap h;
	if (stage <= 3) 
	{
		v = PolygonManager::getInstance()->getPolygonVertex("fierd");
		h = PolygonManager::getInstance()->getPolygonVertex("fierd");
	}
	else
	{
		v = PolygonManager::getInstance()->getPolygonVertex("fierd2");
		h = PolygonManager::getInstance()->getPolygonVertex("fierd2");

	}
	Library::drawGraphic(v, h, 0);

	Library::drawSprite({ 250,100 }, selectStrSpr, &selectStrTex);

	for (int i = 0; i < _countof(stageNumRubberSpr); i++)
		Library::drawBox({ stageNumPos[i].x ,stageNumPos[i].y + 110 }, { stageNumPos[i + 1].x - stageNumPos[i].x,30 }, { 233,233,233,255 }, stageNumRubberSpr[i]);

	for (int i = 0; i < _countof(stageNumBoardSpr); i++)
		Library::drawSprite(stageNumPos[i],  stageNumBoardSpr[i],&stageNumBoardTex[i]);
	


	//シーン遷移
	SceneChange::getInstance()->draw();


}

void StageSelect::end()
{
	Library::setCameraAngle({ 0,0,0 }, { 0,0,0 }, { 0,0,0 });
}

std::string StageSelect::nextScene()
{
	if (titleBack)return "Title";


	Stage::getInstance()->initialize(stage);
	return "Play";
}
