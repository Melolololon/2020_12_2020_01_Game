#include "Game.h"
#include<Library.h>
#include"SceneManager.h"

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
	Library::initialize(1280, 720, { 0,0,0,255 });

#pragma region �}�l�[�W���[������
	SceneManager::getInstace()->initialize();
	SceneManager::getInstace()->addScene("Play", new Play());
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