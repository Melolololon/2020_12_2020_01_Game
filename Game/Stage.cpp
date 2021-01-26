#include "Stage.h"
#include"ParentEnemy.h"
#include"ObjectManager.h"
#include"PolygonManager.h"
#include"XInputManager.h"

const int Stage::SkipTime = 60 * 0.7f;
const int Stage::NextPossibleTime = 60 * 2.0f;
const int Stage::StageStartTime = 60 * 3;
const  const int Stage::StageClearEnemyNum[5] = { 1,3,4,4,4 };

Stage::Stage(){}
Stage::~Stage(){}
Stage* Stage::getInstance()
{
	static Stage s;
	return &s;
}

void Stage::createSpriteLoadTexture()
{
	Library::createSprite(&stageNumSpr);
	Library::setSpriteScale({ 0.7,0.7 }, stageNumSpr);

	Library::createSprite(&tMesSpr);
	tMesTex[0] = Library::loadTexture(L"Resources/Texture/tMes1_Key.png");
	tMesTex[1] = Library::loadTexture(L"Resources/Texture/tMes2_Key.png");
	tMesTex[2] = Library::loadTexture(L"Resources/Texture/tMes3_Key.png");
	tMesTex[3] = Library::loadTexture(L"Resources/Texture/tMes1_Pad.png");
	tMesTex[4] = Library::loadTexture(L"Resources/Texture/tMes2_Pad.png");
	tMesTex[5] = Library::loadTexture(L"Resources/Texture/tMes3_Pad.png");

	Library::setSpriteScale({ 0.5,0.5 }, tMesSpr);
}

void Stage::initialize(const int& stageNum)
{
	stage = stageNum;
	
	if(!XInputManager::getPadConnectFlag(1))
	tMesNum = 0;
	else
	tMesNum = 3;


	if (stageNum == 0)
		tutorialFlag = true;
	else
		tutorialFlag = false;
	skipTimer = 0;
	nextPossibleTimer = 0;
	leftInput = false;
	rightInput = false;
	stageStartTimer = 0;

	clearFlag = false;
	addEnemyCount = 0;
}

void Stage::update()
{
	if(stage >= 1)clearFlag = true;

	switch (stage)
	{
	case 0:


		if (addEnemyCount == 0 && !tutorialFlag) 
		{
			ObjectManager::getInstance()->addObject(new ParentEnemy({ 0,0,20 }));
			addEnemyCount++;
		}

		break;
	}

	if (ParentEnemy::GetDeadCount() >= StageClearEnemyNum[stage])
	{
		clearFlag = true;
	}


#pragma region チュートリアル
	stageStartTimer++;
	if (!tutorialFlag && stageStartTimer <= StageStartTime)return;
	nextPossibleTimer++;
	nextPossibleTimer = nextPossibleTimer >= 60 * 5 ? 60 * 5 : nextPossibleTimer;

	
	if (DirectInput::keyRelease(DIK_RETURN) ||
		XInputManager::buttonRelease(XInputManager::XINPUT_START_BUTTON,1))
	{
		nextPossibleTimer = 0;
		tMesNum++;
	}
	


	if (DirectInput::keyState(DIK_RETURN) ||
		XInputManager::buttonState(XInputManager::XINPUT_START_BUTTON,1))
		skipTimer++;

	if (XInputManager::getPadConnectFlag(1))
	{
		if (tMesNum >= 6)
			tutorialFlag = false;
	}
	else
	{
		if (tMesNum >= 3)
			tutorialFlag = false;
	}

	if (skipTimer >= SkipTime) 
	{
		tutorialFlag = false;
	}
#pragma endregion
}

void Stage::draw()
{
	if (tutorialFlag && stageStartTimer >= StageStartTime)
		Library::drawSprite({ 340,40 }, tMesSpr, &tMesTex[tMesNum]);
	
	//STAGE○○表示
	texture t = PolygonManager::getInstance()->getTexture("stageNum" + stage);
	if (stageStartTimer <= StageStartTime)
		Library::drawSprite({ 100,300 }, stageNumSpr, &t);

	
}

void Stage::setPlayer(Player* p[]) 
{
	player[0] = p[0];
	player[1] = p[1];
}

bool Stage::gettutorialFlag() 
{
	return tutorialFlag;
}


bool Stage::getClearFlag()
{
	return clearFlag;
}
