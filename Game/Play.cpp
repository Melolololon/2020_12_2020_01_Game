#include "Play.h"
#include"ObjectManager.h"
#include"Player.h"
#include"Rubber.h"

#include"PolygonManager.h"


Play::Play()
{
	Library::setCamera({ 0,30,-10 }, { 0,0,0 }, { 0,-1,0 });
	Library::setCameraNearAndFar(1.0f, 1000.0f);

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

	for (int i = 1; i < 10; i++) 
	{
		ObjectManager::getInstance()->addObject(new Rubber(i));
	}
}

void Play::update()
{
	ObjectManager::getInstance()->update();
}

void Play::draw()
{
	ObjectManager::getInstance()->draw();
}

void Play::end()
{
}

std::string Play::nextScene()
{
	return "";
}

