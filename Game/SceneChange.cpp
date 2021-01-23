#include "SceneChange.h"

SceneChange::SceneChange(){}
SceneChange::~SceneChange(){}

SceneChange* SceneChange::getInstance()
{
	static SceneChange s;
	return &s;
}

void SceneChange::load()
{
	Library::createSprite(&sceneChangeFadeSpr);
}

void SceneChange::initialize()
{
	feadTimer = 0;
	feadFlag = false;
	feadPos = { 1280 / 2,720 / 2 };
	feadSize = { 0,0 };
}

void SceneChange::update()
{
	if (!feadFlag)return;

	if (feadTimer < FeadTime/2) 
	{
		feadPos.x -= 20.0f * 5;
		feadPos.y -= 2.5f * 5;

		feadSize.x += 40 * 5;
		feadSize.y += 5 * 5;
	}
	else
	{

		feadPos.x += 20.0f * 5;
		feadPos.y += 2.5f * 5;

		feadSize.x -= 40 * 5;
		feadSize.y -= 5 * 5;
	}

	feadTimer++;
	if (feadTimer == FeadTime)
	{
		feadFlag = false;
		feadTimer = 0;
		feadPos = { 1280 / 2,720 / 2 };
		feadSize = { 0,0 };
	}
}

void SceneChange::draw()
{
	//スプライト表示
	Library::drawBox(feadPos, feadSize, { 0,0,0,255 }, sceneChangeFadeSpr);
}


void SceneChange::trueFeadFlag()
{
	feadFlag = true;
}

bool SceneChange::getSceneChangeFlag()
{
	if (feadTimer == FeadTime / 2)return true;
	return false;
}
