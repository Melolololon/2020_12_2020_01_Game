#include "Play.h"
#include"ObjectManager.h"
#include"Player.h"
#include"Rubber.h"
#include "Enemy.h"
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

	addEnemyTimer = 0;
}

void Play::update()
{
	//“G’Ç‰Áˆ—
	addEnemyTimer++;
	if (addEnemyTimer > ADD_ENEMY_TIME)
	{
		Enemy* enemy = Enemy::GetEnemy();
		enemies.push_back(enemy);
		ObjectManager::getInstance()->addObject(enemy);
		addEnemyTimer = 0;
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

