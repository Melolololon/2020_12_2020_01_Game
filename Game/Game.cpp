#include "Game.h"
#include<Library.h>
#include"SceneManager.h"
#include"ObjectManager.h"
#include"PipelineManager.h"

#include"Player.h"
#include"Rubber.h"
#include"Enemy.h"
#include"ParentEnemy.h"
#include"Boss.h"
#include"Boss2.h"

#include"Stage.h"
#include"Caution.h"
#include"Title.h"
#include"StageSelect.h"
#include"Play.h"
#include"DamageObject.h"
#include"Particle.h"
//テストコメント

Game::Game() {}

Game::~Game() {}

Game* Game::getInstance()
{
	static Game game;
	return &game;
}

void Game::run()
{
	initialize();
	Library::setFramesPerSecond60(true);
	while (1)
	{
		Library::roopStartProcess();
		update();
		draw();
		Library::roopEndProcess();
		if (Library::getIsEnd())break;
	}

	end();
}

void Game::initialize()
{
	Library::initialize(1280, 720, { 155,155,255,255 },L"CHAIN DOUBLE PLAYER");


	Library::setLightVector({ -0.5,-1,0.5});
	Library::setCameraNearAndFar(1.0f, 300.0f);
	Library::setRenderTargetScale({ 1.04,1.04,1 }, 0);


#pragma region パイプライン生成
	PipelineData pData;
	pData.alphaWrite = ALPHA_WIGHT_NONE;
	pData.blendMode = BLEND_NONE;
	pData.cullMode = CULL_BACK;
	pData.depthMode = DEPTH_TRUE;
	pData.drawMode = DRAW_SOLID;

	billboardPL = Library::createUserPipelineState
	(
		pData,
		{ L"LIB" ,"","" },
		{ L"LIB","","" },
		{ L"../MyLibrary/NotShadowPixelShader.hlsl","PSmain" ,"ps_5_0" },
		false
	);
	PipelineManager::getInstance()->addPipeline("billboardPL", billboardPL);
#pragma endregion

#pragma region 生成読み込み
	Stage::getInstance()->loadModelAndTexture();
	Player::loadModel();
	Rubber::loadModel();

	Boss::loadModel();
	ParentEnemy::loadModel();
	Enemy::loadModel();
	Boss2::loadModel();
	DamageObject::loadModel();
	Particle::loadModel();
#pragma endregion

#pragma region マネージャー初期化
	ObjectManager::getInstance()->initialize();

	CollisionFlag cFlag;
	cFlag.board = false;
	cFlag.lineSegment = false;
	cFlag.plane = false;
	cFlag.ray = false;
	cFlag.sphere = true;
	ObjectManager::getInstance()->setCollisionFlag3D(cFlag);

	SceneManager::getInstace()->initialize();
	//SceneManager::getInstace()->addScene("Caution", new Caution());
	SceneManager::getInstace()->addScene("Title", new Title());
	SceneManager::getInstace()->addScene("StageSelect", new StageSelect());
	SceneManager::getInstace()->addScene("Play", new Play());
#pragma endregion

	Library::createBoard({ 400,400 }, dimention3D, &waterV);
	Library::createHeapData(L"Resources/Texture/water.png", 1, &waterH);
	Library::setAngle({ 90,0,0 }, waterH, 0);
	Library::setPosition({ 0,-10,0 }, waterH, 0);
	
}


void Game::end()
{
#pragma region マネージャー終了

	SceneManager::getInstace()->end();
#pragma endregion


	Library::end();//絶対に最後に書く
}

void Game::update()
{
	SceneManager::getInstace()->update();
}

void Game::draw()
{
	Library::drawGraphic(waterV, waterH, 0);
	SceneManager::getInstace()->draw();
	
}