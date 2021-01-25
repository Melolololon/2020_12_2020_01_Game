#include "Stage.h"
#include"ParentEnemy.h"
#include"ObjectManager.h"

const int Stage::SkipTime = 60 * 0.7f;
const int Stage::NextPossibleTime = 60 * 2.0f;
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
	Library::createSprite(&tMesSpr);
	tMesTex[0] = Library::loadTexture(L"Resources/Texture/tMes1.png");
	tMesTex[1] = Library::loadTexture(L"Resources/Texture/tMes2.png");
	tMesTex[2] = Library::loadTexture(L"Resources/Texture/tMes3.png");
	tMesTex[3] = Library::loadTexture(L"Resources/Texture/tMes4.png");

	Library::setSpriteScale({ 0.5,0.5 }, tMesSpr);
}

void Stage::initialize(const int& stageNum)
{
	stage = stageNum;
	tMesNum = 0;

	if (stageNum == 0)
		tutorialFlag = true;
	else
		tutorialFlag = false;
	skipTimer = 0;
	nextPossibleTimer = 0;
	leftInput = false;
	rightInput = false;


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

	if (!tutorialFlag)return;
	nextPossibleTimer++;
	nextPossibleTimer = nextPossibleTimer >= 60 * 5 ? 60 * 5 : nextPossibleTimer;

	if (tMesNum == 0 ||
		tMesNum == 3)
	{
		if (DirectInput::keyRelease(DIK_RETURN) ||
			DirectInput::buttonRelease(StartButton))
		{
			nextPossibleTimer = 0;
			tMesNum++;
		}
	}

	if (tMesNum == 1)
	{
		if (nextPossibleTimer >= NextPossibleTime)
		{
			if (player[0]->getVelocity().x != 0 ||
				player[0]->getVelocity().z != 0) 
			{
				if (player[0]->getPlayerType() == Player::PlayerType::LEFT)
					leftInput = true;

				if (player[0]->getPlayerType() == Player::PlayerType::RIGHT)
					rightInput = true;
			}

			if(player[1]->getVelocity().x != 0 ||
				player[1]->getVelocity().z != 0)
			{
				if (player[1]->getPlayerType() == Player::PlayerType::LEFT)
					leftInput = true;

				if (player[1]->getPlayerType() == Player::PlayerType::RIGHT)
					rightInput = true;
			}

			if(leftInput && rightInput)
			{
				nextPossibleTimer = 0;
				tMesNum++;
			}
		}
	}
	if (tMesNum == 2)
	{
		if (nextPossibleTimer >= NextPossibleTime)
		{
			if (player[0]->getIsDash() ||
				player[1]->getIsDash())
			{
				nextPossibleTimer = 0;
				tMesNum++;
			}
		}
	}


	if (DirectInput::keyState(DIK_RETURN) ||
		DirectInput::buttonState(StartButton))
		skipTimer++;

	if (tMesNum >= 4 ||
		skipTimer >= SkipTime)
		tutorialFlag = false;
#pragma endregion
}

void Stage::draw()
{
	if (tutorialFlag) 
		Library::drawSprite({ 340,40 }, tMesSpr, &tMesTex[tMesNum]);
	
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
