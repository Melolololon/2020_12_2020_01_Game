#include "Game.h"
#include<Library.h>
#include"SceneManager.h"

#include"Title.h"
#include"StageSelect.h"
#include"Play.h"
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
	Library::initialize(1280, 720, { 155,155,255,255 });

#pragma region マネージャー初期化
	SceneManager::getInstace()->initialize();
	SceneManager::getInstace()->addScene("Title", new Title());
	SceneManager::getInstace()->addScene("StageSelect", new StageSelect());
	SceneManager::getInstace()->addScene("Play", new Play());
#pragma endregion

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
	SceneManager::getInstace()->draw();
}