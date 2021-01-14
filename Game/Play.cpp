#include "Play.h"
#include"ObjectManager.h"
#include"Player.h"

#include"PolygonManager.h"

Vector3 Play::scrollSpeed;

Play::Play()
{
	Library::setCamera({ 0,30,-10 }, { 0,0,0 }, { 0,-1,0 });
	Library::setCameraNearAndFar(1.0f, 1000.0f);

	scrollSpeed = { 0,0,15.0f };

	vertex vertexHandle;
	heap heapHandle;/*
	Library::createManyVertex3DBox({ 20,20,20 }, &vertexHandle);
	Library::createHeapData2({ 255,0,0,255 }, 7000, &heapHandle);
	PolygonManager::getInstance()->addPolygonVertex("Obstacle", vertexHandle);
	PolygonManager::getInstance()->addPolygonHeap("Obstacle", heapHandle);*/
}


Play::~Play()
{
}

void Play::initialize()
{
	ObjectManager::getInstance()->addObject(new Player({ -2,0,0 }, Player::PlayerType::LEFT));
	ObjectManager::getInstance()->addObject(new Player({ 2,0,0 }, Player::PlayerType::RIGHT));

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

Vector3 Play::getScrollSpeed() 
{
	return scrollSpeed;
}

