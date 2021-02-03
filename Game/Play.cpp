#include "Play.h"
#include"ObjectManager.h"
#include"SceneChange.h"

#include"Player.h"
#include"Rubber.h"
#include "Enemy.h"
#include "ParentEnemy.h"
#include"PolygonManager.h"

#include"Stage.h"

#include"XInputManager.h"


const int Play::SceneChangeTime = 60 * 3;

Play::Play()
{



#pragma region スプライト
	Library::createSprite(&pLifeSpr[0]);
	Library::createSprite(&pLifeSpr[1]);
	Library::createSprite(&hpSpr[0]);
	Library::createSprite(&hpSpr[1]);
	pLifeTex = Library::loadTexture(L"Resources/Texture/playerLife.png");
	hpTex = Library::loadTexture(L"Resources/Texture/HP.png");


	Library::createSprite(&whiteSpr);
	Library::createSprite(&gameStateSpr);
	clearTex = Library::loadTexture(L"Resources/Texture/clear.png");


	Library::createSprite(&pauseSpr);
	Library::createSprite(&pauseStrSpr);
	pauseStrPadTex = Library::loadTexture(L"Resources/Texture/pauseStr_Pad.png");
	pauseStrKeyTex = Library::loadTexture(L"Resources/Texture/pauseStr_Key.png");
#pragma endregion

}


Play::~Play()
{
}

void Play::initialize()
{
	Enemy::allDead(false);


#pragma region プレイヤー関係


	player[0] = new Player({ -2,0,0 }, Player::PlayerType::LEFT);
	player[1] = new Player({ 2,0,0 }, Player::PlayerType::RIGHT);
	ObjectManager::getInstance()->addObject(player[0]);
	ObjectManager::getInstance()->addObject(player[1]);
	
	
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



	gameState = GAME_STATE_NORMAL;
	sceneChangeTimer = 0;
	whiteAlpha = 0.0f;


}

void Play::update()
{


#pragma region シーン遷移

	SceneChange::getInstance()->update();

	if (sceneChangeTimer >= SceneChangeTime)
		SceneChange::getInstance()->trueFeadFlag();

	if (SceneChange::getInstance()->getSceneChangeFlag())isEnd = true;

#pragma endregion


#pragma region ポーズ処理
	if (DirectInput::keyTrigger(DIK_ESCAPE) || XInputManager::buttonTrigger(XInputManager::XINPUT_START_BUTTON,1))
	{
		if(!Stage::getInstance()->getTutorialFlag())
		pauseFlag = pauseFlag == false ? true : false;
	}
	if (pauseFlag)
	{
		if(DirectInput::keyTrigger(DIK_RETURN) ||
			XInputManager::buttonTrigger(XInputManager::XINPUT_BACK_BUTTON,1))
			SceneChange::getInstance()->trueFeadFlag();

		return;
	}
#pragma endregion



	//敵追加処理
	//addEnemyTimer++;
	//if (addEnemyTimer > ADD_ENEMY_TIME)
	//{
	//	Enemy* enemy = Enemy::GetEnemy();
	//	enemies.push_back(enemy);
	//	ObjectManager::getInstance()->addObject(enemy);
	//	addEnemyTimer = -300;
	//}

	Stage::getInstance()->update();
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
	//Player::setEnemyVector(Rubber::getEnemyVector());

#pragma endregion

	if(gameState == GAME_STATE_NORMAL)
	ObjectManager::getInstance()->isDeadCheck();
	if(Player::getDeadPlayer())
	{
		player[0] = nullptr;
		player[1] = nullptr;
	}

#pragma region ゲーム状態処理

	if (gameState == GAME_STATE_NORMAL)
	{
		if (!player[0] || !player[1])
		{
			gameState = GAME_STATE_GAMEOVER;
		}
		else
		if (Stage::getInstance()->getClearFlag())
		{
			whiteAlpha = 100.0f;
			gameState = GAME_STATE_CLEAR;
		}
	}
	if (gameState != GAME_STATE_NORMAL) 
	{
		whiteAlpha -= 3.0f;
		whiteAlpha = whiteAlpha <= 0 ? 0 : whiteAlpha;

		sceneChangeTimer++;
	}



#pragma endregion



}

void Play::draw()
{

	ObjectManager::getInstance()->draw();

#pragma region ライフ	
	//揺れの乱数分ずらせばライフ揺れない
	//2Dと3Dで座標系が違うから変換しないと無理
	
	if (player[0] && player[1]) 
	{
		Library::setSpriteAddColor({ 0,0,255,0 }, hpSpr[0]);
		Library::setSpriteAddColor({ 255,0,0,0 }, hpSpr[1]);
		Library::drawSprite({ 30,30 }, hpSpr[0], &hpTex);
		Library::drawSprite({ 30,100 }, hpSpr[1], &hpTex);

		Library::drawSpriteAnimation2({ 120,30 }, { 0,0 }, { (float)player[0]->getLife() * 50,50 }, pLifeSpr[0], &pLifeTex);
		Library::drawSpriteAnimation2({ 120,90 }, { 0,0 }, { (float)player[1]->getLife() * 50,50 }, pLifeSpr[1], &pLifeTex);
	}
#pragma endregion

	Stage::getInstance()->draw();

#pragma region クリアゲームオーバー表示
	
	if (gameState == GAME_STATE_CLEAR)
		Library::drawSprite({ 160,200 }, gameStateSpr, &clearTex);
	/*if (gameState == GAME_STATE_GAMEOVER)
		Library::drawSprite({ 200,200 }, gameStateSpr, &clearTex);*/
	Library::drawBox({ 0,0 }, { 1280,720 }, { 155, 155, 155, Color::alphaChangePar(whiteAlpha) }, whiteSpr);

#pragma endregion

#pragma region ポーズ
	if(pauseFlag)
	{
		Library::drawBox({ 0,0 }, { 1280,720 }, { 0,0,0,155 }, pauseSpr);

		if (XInputManager::getPadConnectFlag(1))
		Library::drawSprite({ 0,0 }, pauseStrSpr, &pauseStrPadTex);
		else
			Library::drawSprite({ 0,0 }, pauseStrSpr, &pauseStrKeyTex);
	}
#pragma endregion


	//シーン遷移
	SceneChange::getInstance()->draw();
}

void Play::end()
{
	ParentEnemy::ResetDeadCount();
	Stage::getInstance()->end();
	ObjectManager::getInstance()->allDeleteObject();

	Library::stopLoadSound("boss", false);
}

std::string Play::nextScene()
{
	return "StageSelect";
}

