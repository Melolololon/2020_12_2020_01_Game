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



#pragma region �X�v���C�g
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
	


#pragma region �v���C���[�֌W


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
}

void Play::update()
{

#pragma region �|�[�Y����
	if (DirectInput::keyTrigger(DIK_ESCAPE) || XInputManager::buttonTrigger(XInputManager::XINPUT_START_BUTTON,1))
	{
		if(!Stage::getInstance()->gettutorialFlag())
		pauseFlag = pauseFlag == false ? true : false;
	}
	if (pauseFlag)return;
#pragma endregion



	//�G�ǉ�����
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


#pragma region �v���C���[�ɓG�̈ړ��ʑ��
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

#pragma region �Q�[����ԏ���

	if (!player[0] || !player[1]) 
		gameState = GAME_STATE_GAMEOVER;
	

	if (gameState != GAME_STATE_GAMEOVER && Stage::getInstance()->getClearFlag())
		gameState = GAME_STATE_CLEAR;

	if (gameState != GAME_STATE_NORMAL)
		sceneChangeTimer++;


#pragma endregion


#pragma region �V�[���J��

	SceneChange::getInstance()->update();

	if (sceneChangeTimer >= SceneChangeTime)
		SceneChange::getInstance()->trueFeadFlag();
	
	if (SceneChange::getInstance()->getSceneChangeFlag())isEnd = true;

#pragma endregion


}

void Play::draw()
{

	ObjectManager::getInstance()->draw();

#pragma region ���C�t	
	//�h��̗��������点�΃��C�t�h��Ȃ�
	//2D��3D�ō��W�n���Ⴄ����ϊ����Ȃ��Ɩ���
	
	if (player[0] && player[1]) 
	{
		Library::setSpriteAddColor({ 0,0,255,0 }, hpSpr[0]);
		Library::setSpriteAddColor({ 255,0,0,0 }, hpSpr[1]);
		Library::drawSprite({ 30,30 }, hpSpr[0], &hpTex);
		Library::drawSprite({ 30,100 }, hpSpr[1], &hpTex);

		Library::drawSpriteAnimation2({ 120,40 }, { 0,0 }, { (float)player[0]->getLife() * 50,50 }, pLifeSpr[0], &pLifeTex);
		Library::drawSpriteAnimation2({ 120,100 }, { 0,0 }, { (float)player[1]->getLife() * 50,50 }, pLifeSpr[1], &pLifeTex);
	}
#pragma endregion

	Stage::getInstance()->draw();

	//�V�[���J��
	SceneChange::getInstance()->draw();
}

void Play::end()
{
	ParentEnemy::ResetDeadCount();
	Stage::getInstance()->end();
	ObjectManager::getInstance()->allDeleteObject();
}

std::string Play::nextScene()
{
	return "StageSelect";
}

