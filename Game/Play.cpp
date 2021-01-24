#include "Play.h"
#include"ObjectManager.h"
#include"SceneChange.h"

#include"Player.h"
#include"Rubber.h"
#include "Enemy.h"
#include"PolygonManager.h"

//テスト用
#include"DamageNumber.h"

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


}


Play::~Play()
{
}

void Play::initialize()
{
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

	addEnemyTimer = 0;

	pauseFlag = false;
}

void Play::update()
{
#pragma region ポーズ処理
	if (DirectInput::keyTrigger(DIK_ESCAPE) || DirectInput::buttonTrigger(StartButton))
		pauseFlag = pauseFlag == false ? true : false;

	if (pauseFlag)return;
#pragma endregion



	//敵追加処理
	addEnemyTimer++;
	if (addEnemyTimer > ADD_ENEMY_TIME)
	{
		Enemy* enemy = Enemy::GetEnemy();
		enemies.push_back(enemy);
		ObjectManager::getInstance()->addObject(enemy);
		addEnemyTimer = -300;
	}

	ObjectManager::getInstance()->update();
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->GetUpdateVelocityTimer() > UPDATE_VELOCITY_TIME)
		{
			Vector3 playerPos = player[enemies[i]->GetTargetTypeAsInt()]->getSphereData()[0].position;
			enemies[i]->UpdateVelocity(playerPos);
		}
	}


#pragma region プレイヤーに敵の移動量代入
	player[0]->addPosition(Rubber::getPlayerMoveVector());
	player[1]->addPosition(Rubber::getPlayerMoveVector());
#pragma endregion

	ObjectManager::getInstance()->isDeadCheck();

#pragma region 地形関係
	Library::setPosition({ 0,-21,0 }, fierdHeapH, 0);
	Library::setScale({ 22,20,16}, fierdHeapH, 0);
#pragma endregion


#pragma region シーン遷移

	//if (DirectInput::keyTrigger(DIK_SPACE))SceneChange::getInstance()->trueFeadFlag();

	SceneChange::getInstance()->update();

	if (SceneChange::getInstance()->getSceneChangeFlag())isEnd = true;

#pragma endregion


}

void Play::draw()
{
	ObjectManager::getInstance()->draw();

#pragma region 地形関係
	Library::drawGraphic(fierdVertexH, fierdHeapH, 0);
#pragma endregion


	//シーン遷移
	SceneChange::getInstance()->draw();
}

void Play::end()
{
}

std::string Play::nextScene()
{
	return "Title";
}

