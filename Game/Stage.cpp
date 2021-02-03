#include "Stage.h"
#include"ParentEnemy.h"
#include"Boss.h"
#include"Boss2.h"
#include"ObjectManager.h"
#include"PolygonManager.h"
#include"XInputManager.h"
#include"Player.h"


const int Stage::SkipTime = 60 * 0.7f;
const int Stage::NextPossibleTime = 60 * 2.0f;
const int Stage::StageStartTime = 60 * 5;


const Vector3 Stage::PlayCameraPos = { 0,35,-22 };
const Vector3 Stage::StartCameraPos = { 0,35 + 15,-22 + -15 };

//1ボス
const Vector3 Stage::Boss1StartCameraPos = { 0,2,2 };

//2ボス
const Vector3 Stage::Boss2StartCameraPos = { 0,4,2 };

Stage::Stage(){}
Stage::~Stage(){}
Stage* Stage::getInstance()
{
	static Stage s;
	return &s;
}

void Stage::loadModelAndTexture()
{

#pragma region モデル
	std::string material;
	Library::loadOBJVertex("Resources/Obj/fierd.obj", true, true, &material, &fierdVertexH[0]);
	Library::loadOBJMaterial("Resources/Obj/", material, 1, &fierdHeapH[0]);
	Library::setPosition({ 0,-21,0 }, fierdHeapH[0], 0);
	Library::setScale({ 22,20,16 }, fierdHeapH[0], 0);
	PolygonManager::getInstance()->addPolygonVertex("fierd", fierdVertexH[0]);
	PolygonManager::getInstance()->addPolygonHeap("fierd", fierdHeapH[0]);


	Library::loadOBJVertex("Resources/Obj/fierd2.obj", true, true, &material, &fierdVertexH[1]);
	Library::loadOBJMaterial("Resources/Obj/", material, 1, &fierdHeapH[1]);
	Library::setPosition({ 0,-2,0 }, fierdHeapH[1], 0);
	Library::setScale({ 5,1,5.7 }, fierdHeapH[1], 0);
	PolygonManager::getInstance()->addPolygonVertex("fierd2", fierdVertexH[1]);
	PolygonManager::getInstance()->addPolygonHeap("fierd2", fierdHeapH[1]);
#pragma endregion

	Library::createSprite(&stageNumSpr);
	Library::setSpriteScale({ 0.7,0.7 }, stageNumSpr);

	Library::createSprite(&tMesSpr);
	tMesTexKey[0] = Library::loadTexture(L"Resources/Texture/tMes1_Key.png");
	tMesTexKey[1] = Library::loadTexture(L"Resources/Texture/tMes2_Key.png");
	tMesTexKey[2] = Library::loadTexture(L"Resources/Texture/tMes3_Key.png");
	tMesTexPad[0] = Library::loadTexture(L"Resources/Texture/tMes1_Pad.png");
	tMesTexPad[1] = Library::loadTexture(L"Resources/Texture/tMes2_Pad.png");
	tMesTexPad[2] = Library::loadTexture(L"Resources/Texture/tMes3_Pad.png");

	Library::setSpriteScale({ 0.5,0.5 }, tMesSpr);


	Library::createSprite(&greenSpr);
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
	stageStartTimer = 0;

	clearFlag = false;
	addEnemyCount = 0;
	enemyDeadCount = -1;
	pureEnemyDeadCount = 0;



	switch (stage)
	{
	case 0:
		enemys.resize(1);
		enemys[0] = new ParentEnemy({ 0,0,20 },ParentEnemy::PENEMY_TARGET);
		break;

	case 1:
		enemys.resize(3);
		enemys[0] = new ParentEnemy({ -25,0,0 }, ParentEnemy::PENEMY_TARGET);
		enemys[1] = new ParentEnemy({25,0,0 }, ParentEnemy::PENEMY_TARGET);
		enemys[2] = new ParentEnemy({ 0,0,0 }, ParentEnemy::PENEMY_TARGET);
		break;

	case 2:
		enemys.resize(4);
		enemys[0] = new ParentEnemy({ -25,0,20 }, ParentEnemy::PENEMY_TARGET);
		enemys[1] = new ParentEnemy({ 25,0,-20 }, ParentEnemy::PENEMY_TARGET);
		enemys[2] = new ParentEnemy({ 25,0,0 }, ParentEnemy::PENEMY_TARGET);
		enemys[3] = new ParentEnemy({ -25,0,0 }, ParentEnemy::PENEMY_TARGET);
		break;

	case 3:
		enemys.resize(1);
		enemys[0] = new Boss();
		addEnemy();
		break;

	case 4:
		enemys.resize(6);
		enemys[0] = new ParentEnemy({ 25,0,0 },180);
		enemys[1] = new ParentEnemy({ -25,0,0 }, 270);
		enemys[2] = new ParentEnemy({ 0,0,20 }, 180);
		enemys[3] = new ParentEnemy({ 0,0,-20 }, 90);
		enemys[4] = new ParentEnemy({ -25,0,20 }, ParentEnemy::PENEMY_TARGET);
		enemys[5] = new ParentEnemy({ 25,0,20 }, ParentEnemy::PENEMY_TARGET);
		break;

	case 5:
		enemys.resize(7);
		enemys[0] = new ParentEnemy({ 25,0,20 }, 135);
		enemys[1] = new ParentEnemy({ 25,0,-20 }, 180 + 45);

		enemys[2] = new ParentEnemy({ -25,0,20 }, 45);
		enemys[3] = new ParentEnemy({ -25,0,-20 }, 270 + 45);


		enemys[5] = new ParentEnemy({ 0,0,20 }, ParentEnemy::PENEMY_TARGET);
		enemys[6] = new ParentEnemy({ 0,0,0 }, 90);
		break;

	case 6:
		enemys.resize(1);
		enemys[0] = new Boss2();
		addEnemy();
		break;
	}
	stageClearEnemyNum = enemys.size();

	Library::setCamera(StartCameraPos, { 0,0,-3 }, { 0,1,0 });
	Library::setCameraMatrixPoint(StartCameraPos, { 0,0,-3 }, { 0,1,0 });
	cameraPos = StartCameraPos;
	cameraAngle = { 0,110,0 };
	rotateAngle = 0;

	if (stage == 3)
	{
		cameraPos = Boss1StartCameraPos;
		cameraAngle = 0;
		Library::setCamera(cameraPos,enemys[0]->getSphereData()[0].position, { 0,1,0 });
		Library::setCameraMatrixPoint(cameraPos, { 0,0,-3 }, { 0,1,0 });
	}

	if (stage == 6)
	{
		cameraPos = Boss2StartCameraPos;
		cameraAngle = 0;
		Library::setCamera(cameraPos, {0,0,40}, { 0,1,0 });
		Library::setCameraMatrixPoint(cameraPos, { 0,0,-3 }, { 0,1,0 });
	}
}

void Stage::update()
{

	stageStartTimer++;

#pragma region カメラ処理
	if (stage != 3 && stage != 6) 
	{
		if (cameraPos.y > PlayCameraPos.y &&
			cameraPos.z < PlayCameraPos.z)
		{
			cameraPos.y -= 0.055f;
			cameraPos.z += 0.055f;
		}
		else
		{
			cameraPos = PlayCameraPos;
		}

		if (rotateAngle < 250)
		{
			cameraAngle.y += 1;
			rotateAngle += 1;
		}
		else
		{
			cameraAngle = { 0,0,0 };
		}

		Library::setCamera(cameraPos, { 0,0,-3 }, { 0,1,0 });
		Library::setCameraMatrixPoint(cameraPos, { 0,0,-3 }, { 0,1,0 });
		Library::setCameraAngle(cameraAngle, { 0,0,0 }, { 0,0,0 });
	}
	
	if(stage == 3)
	{
		if (stageStartTimer == StageStartTime)
			cameraPos = PlayCameraPos;

		if (stageStartTimer > StageStartTime)
			Library::setCamera(cameraPos, { 0,0,-3 }, { 0,1,0 });
		else
			Library::setCamera(cameraPos, enemys[0]->getSphereData()[0].position, { 0,1,0 });
	}

	if (stage == 6)
	{
		if (stageStartTimer == StageStartTime)
			cameraPos = PlayCameraPos;

		if (stageStartTimer > StageStartTime)
			Library::setCamera(cameraPos, { 0,0,-3 }, { 0,1,0 });
		else
			Library::setCamera(cameraPos, { 0,0,40 }, { 0,1,0 });
	}
#pragma endregion


	if (stageStartTimer < StageStartTime)
	{
		Player::setStopFlag(true);
		return;
	}
	else
	{
		Player::setStopFlag(false);
		stageStartTimer = StageStartTime + 1;
	}
#pragma region クリア処理

	if (ParentEnemy::GetDeadCount() >= stageClearEnemyNum) 
	{
		Player::setGameClearFlag(true);
		clearFlag = true;
	}

	if(stage == 3)
	{
		Boss* b = static_cast<Boss*>(enemys[0]->getPtr());

		if (b->getLifeZero()) 
		{
			cameraPos = Boss1StartCameraPos;

			Vector3 cameraTar = enemys[0]->getSphereData()[0].position;
			if (cameraTar.y <= 0)cameraTar.y = 0;
			Library::setCamera(cameraPos, cameraTar, { 0,1,0 });
		}
		if (b->getDropFlag()) 
		{
			Player::setGameClearFlag(true);
			clearFlag = true;
		}
	}

	if (stage == 6)
	{
		Boss2* b = static_cast<Boss2*>(enemys[0]->getPtr());
		if (b->getLifeZero())
		{
			cameraPos = Boss2StartCameraPos;
			Vector3 cameraTar = enemys[0]->getSphereData()[0].position;
			Library::setCamera(cameraPos, cameraTar, { 0,1,0 });

		}
		if (b->scaleZeroFlag())
		{
			Player::setGameClearFlag(true);
			clearFlag = true;
		}
	}

#pragma endregion



	if (!tutorialFlag) 
	{
		pureEnemyDeadCount = enemyDeadCount;
		enemyDeadCount = ParentEnemy::GetDeadCount();	
	}
	if (pureEnemyDeadCount == enemyDeadCount)return;

	switch (stage)
	{
	case 0:

		if (!tutorialFlag && enemyDeadCount == 0)
		{
			addEnemy();
		}

		break;

	case 1:

		if (enemyDeadCount == 0) 
		addEnemy();
		
		if (enemyDeadCount == 1)
			addEnemy();

		if (enemyDeadCount == 2)
			addEnemy();
		break;

	case 2:
		if (enemyDeadCount == 0)
		addEnemy();

		if (enemyDeadCount == 1)
			addEnemy();

		if (enemyDeadCount == 2) 
		{
			addEnemy();
			addEnemy();
		}
		break;

	case 3:
		break;

	case 4:
		if (enemyDeadCount == 0) 
		{
			addEnemy();
			addEnemy();
		}
		if (enemyDeadCount == 2)
		{
			addEnemy();
			addEnemy();
		}
		if (enemyDeadCount == 4)
		{
			addEnemy();
			addEnemy();
		}
		break;

	case 5:
		if (enemyDeadCount == 0)
		{
			addEnemy();
			addEnemy();
		}
		if (enemyDeadCount == 2)
		{
			addEnemy();
			addEnemy();
		}
		if (enemyDeadCount == 4)
		{
			addEnemy();
			addEnemy();
		}
		break;
	}


#pragma region チュートリアル
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

	if (tMesNum >= 3)
		tutorialFlag = false;

	if (skipTimer >= SkipTime) 
	{
		tutorialFlag = false;
	}
#pragma endregion
}

void Stage::draw()
{
	

	if(stage <= 3)
	Library::drawGraphic(fierdVertexH[0], fierdHeapH[0], 0);
	else 
	{
		Library::drawGraphic(fierdVertexH[1], fierdHeapH[1], 0);
	}
	if (tutorialFlag && stageStartTimer >= StageStartTime)
	{
		if (!XInputManager::getPadConnectFlag(1))
			Library::drawSprite({ 340,40 }, tMesSpr, &tMesTexKey[tMesNum]);
		else
			Library::drawSprite({ 340,40 }, tMesSpr, &tMesTexPad[tMesNum]);
	}

	//STAGE○○表示
	texture t = PolygonManager::getInstance()->getTexture("stageNum" + stage);
	if (stageStartTimer <= StageStartTime)
		Library::drawSprite({ 100,300 }, stageNumSpr, &t);
}

void Stage::end() 
{
	enemys.clear();
	enemys.shrink_to_fit();
}

void Stage::setPlayer(Player* p[]) 
{
	player[0] = p[0];
	player[1] = p[1];
}

bool Stage::getTutorialFlag() 
{
	return tutorialFlag;
}


bool Stage::getClearFlag()
{
	return clearFlag;
}


void Stage::addEnemy()
{
	ObjectManager::getInstance()->addObject(enemys[addEnemyCount]);
	addEnemyCount++;
}

