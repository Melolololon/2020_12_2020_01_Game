#include "Play.h"
#include"ObjectManager.h"
#include"SceneChange.h"

#include"Player.h"
#include"Rubber.h"
#include "Enemy.h"
#include "ParentEnemy.h"
#include"PolygonManager.h"

#include"Stage.h"


const int Play::SceneChangeTime = 60 * 3;

Play::Play()
{
	Library::setCamera({ 0,35,-22 }, { 0,0,-3 }, { 0,-1,0 });
	Library::setLightVector({ -0.25,-0.75,0 });
	Library::setCameraNearAndFar(1.0f, 1000.0f);

#pragma region モデル
	std::string material;
	Library::loadOBJVertex("Resources/Obj/fierd.obj", true, true, &material, &fierdVertexH);
	Library::loadOBJMaterial("Resources/Obj/", material, 1, &fierdHeapH);
#pragma endregion

#pragma region スプライト
	Library::createSprite(&pLifeSpr[0]);
	Library::createSprite(&pLifeSpr[1]);
	Library::createSprite(&hpSpr[0]);
	Library::createSprite(&hpSpr[1]);
	pLifeTex = Library::loadTexture(L"Resources/Texture/playerLife.png");
	hpTex = Library::loadTexture(L"Resources/Texture/HP.png");
#pragma endregion

}


Play::~Play()
{
}

void Play::initialize()
{
#pragma region プレイヤー関係


	player[0] = new Player({ -2,0,0 }, Player::PlayerType::LEFT);
	player[1] = new Player({ 2,0,0 }, Player::PlayerType::RIGHT);
	ObjectManager::getInstance()->addObject(player[0]);
	ObjectManager::getInstance()->addObject(player[1]);
	
	Rubber* rP[9];
	for (int i = 1; i < 10; i++)
	{
		rP[i - 1] = new Rubber(i);
		ObjectManager::getInstance()->addObject(rP[i - 1]);
	}
	Rubber::setRubberPtr(rP);

	Stage::getInstance()->setPlayer(player);
#pragma endregion




	addEnemyTimer = 0;

	pauseFlag = false;


#pragma region 地形関係
	Library::setPosition({ 0,-21,0 }, fierdHeapH, 0);
	Library::setScale({ 22,20,16 }, fierdHeapH, 0);
#pragma endregion

	gameState = GAME_STATE_NORMAL;
	sceneChangeTimer = 0;
}

void Play::update()
{
#pragma region ポーズ処理
	if (DirectInput::keyTrigger(DIK_ESCAPE) || DirectInput::buttonTrigger(StartButton))
	{
		if(!Stage::getInstance()->gettutorialFlag())
		pauseFlag = pauseFlag == false ? true : false;
	}
	if (pauseFlag)return;
#pragma endregion

	Stage::getInstance()->update();

	//敵追加処理
	//addEnemyTimer++;
	//if (addEnemyTimer > ADD_ENEMY_TIME)
	//{
	//	Enemy* enemy = Enemy::GetEnemy();
	//	enemies.push_back(enemy);
	//	ObjectManager::getInstance()->addObject(enemy);
	//	addEnemyTimer = -300;
	//}

	ObjectManager::getInstance()->update();

	//for (int i = 0; i < enemies.size(); i++)
	//{
		//if (enemies[i]->GetUpdateVelocityTimer() > UPDATE_VELOCITY_TIME)
	//	{
	//		Vector3 playerPos = player[enemies[i]->GetTargetTypeAsInt()]->getSphereData()[0].position;
	//		enemies[i]->UpdateVelocity(playerPos);
	//	}
	//}


#pragma region プレイヤーに敵の移動量代入
	if (player[0] && player[1]) 
	{
		player[0]->addPosition(Rubber::getPlayerMoveVector());
		player[1]->addPosition(Rubber::getPlayerMoveVector());
	}

#pragma endregion

	if(gameState == GAME_STATE_NORMAL)
	ObjectManager::getInstance()->isDeadCheck();
	if(Player::getDeadPlayer())
	{
		player[0] = nullptr;
		player[1] = nullptr;
	}

#pragma region ゲーム状態処理

	if (!player[0] || !player[1]) 
		gameState = GAME_STATE_GAMEOVER;
	

	if (gameState != GAME_STATE_GAMEOVER && Stage::getInstance()->getClearFlag())
		gameState = GAME_STATE_CLEAR;

	if (gameState != GAME_STATE_NORMAL)
		sceneChangeTimer++;


#pragma endregion


#pragma region シーン遷移

	SceneChange::getInstance()->update();

	if (sceneChangeTimer >= SceneChangeTime)
		SceneChange::getInstance()->trueFeadFlag();
	
	if (SceneChange::getInstance()->getSceneChangeFlag())isEnd = true;

#pragma endregion


}

void Play::draw()
{

	ObjectManager::getInstance()->draw();

#pragma region 地形関係
	Library::drawGraphic(fierdVertexH, fierdHeapH, 0);
#pragma endregion

#pragma region ライフ	
	if (player[0] && player[1]) 
	{
		Library::setSpriteAddColor({ 0,0,255,0 }, hpSpr[0]);
		Library::setSpriteAddColor({ 255,0,0,0 }, hpSpr[1]);
		Library::drawSprite({ 10,10 }, hpSpr[0], &hpTex);
		Library::drawSprite({ 10,70 }, hpSpr[1], &hpTex);

		Library::drawSpriteAnimation2({ 90,10 }, { 0,0 }, { (float)player[0]->getLife() * 50,50 }, pLifeSpr[0], &pLifeTex);
		Library::drawSpriteAnimation2({ 90,70 }, { 0,0 }, { (float)player[1]->getLife() * 50,50 }, pLifeSpr[1], &pLifeTex);
	}
#pragma endregion

	Stage::getInstance()->draw();

	//シーン遷移
	SceneChange::getInstance()->draw();
}

void Play::end()
{
	ParentEnemy::ResetDeadCount();

	ObjectManager::getInstance()->allDeleteObject();
}

std::string Play::nextScene()
{
	return "StageSelect";
}

