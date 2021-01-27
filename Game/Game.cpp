#include "Game.h"
#include<Library.h>
#include"SceneManager.h"
#include"ObjectManager.h"
#include"PipelineManager.h"

#include"Player.h"
#include"Rubber.h"

#include"Stage.h"
#include"Caution.h"
#include"Title.h"
#include"StageSelect.h"
#include"Play.h"
//�e�X�g�R�����g

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
	Library::initialize(1280, 720, { 155,155,255,255 });

#pragma region �}�l�[�W���[������
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

#pragma region �p�C�v���C������
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
		{ L"../MyLibrary/NotShadowPixelShader.hlsl","PSmain" ,"ps_5_0"},
		false
	);
	PipelineManager::getInstance()->addPipeline("billboardPL", billboardPL);
#pragma endregion

#pragma region �����ǂݍ���
	Stage::getInstance()->createSpriteLoadTexture();
	Player::loadModel();
	Rubber::loadModel();
#pragma endregion

}


void Game::end()
{
#pragma region �}�l�[�W���[�I��

	SceneManager::getInstace()->end();
#pragma endregion


	Library::end();//��΂ɍŌ�ɏ���
}

void Game::update()
{
	SceneManager::getInstace()->update();
}

void Game::draw()
{
	SceneManager::getInstace()->draw();
}