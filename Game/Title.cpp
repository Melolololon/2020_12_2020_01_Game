#include "Title.h"
#include"SceneChange.h"
#include"XInputManager.h"
#include"PolygonManager.h"
#include"ObjectManager.h"
#include"Player.h"
#include"Rubber.h"
Title::Title()
{
	SceneChange::getInstance()->load();
	SceneChange::getInstance()->initialize();

	Library::createSprite(&titleSpr);
	Library::createSprite(&pushSpr);

	titleTex = Library::loadTexture(L"Resources/Texture/title.png");
	pushTex = Library::loadTexture(L"Resources/Texture/titleRubberSpr.png");

}

Title::~Title()
{

}

void Title::initialize()
{
	pushPos = { 270,500 };
	pushScale = { 1,1};
	pushScaleChangeTimer = 0;


	Library::setCamera({ 0,35,-22 }, { 0,-3,0 }, { 0,1,0 });
	Library::setCameraMatrixPoint({ 0,35,-22 }, { 0,-3,0 }, { 0,1,0 });
	cameraAngle = 0;

	
	ObjectManager::getInstance()->addObject(new Player({ -4,0,0 }, Player::PlayerType::LEFT));
	ObjectManager::getInstance()->addObject(new Player({ 4,0,0 }, Player::PlayerType::RIGHT));
	Player::setStopFlag(true);
	Rubber* rP[9];
	for (int i = 1; i < 10; i++)
	{
		rP[i - 1] = new Rubber(i);
		ObjectManager::getInstance()->addObject(rP[i - 1]);
	}
	Rubber::setRubberPtr(rP);
}

void Title::update()
{
	pushScaleChangeTimer++;
	if(pushScaleChangeTimer < PushScaleChangeTime / 2)
	{
		pushScale.x += 0.025f / 4;
		pushPos.x -= 10 / 4;
	}
	else 
	{
		pushScale.x -= 0.025f / 4;
		pushPos.x += 10 / 4;
	}
	if (pushScaleChangeTimer == PushScaleChangeTime) 
	{
		pushPos = { 270,500 };
		pushScale = { 1,1 };
		pushScaleChangeTimer = 0;
	}
	ObjectManager::getInstance()->update();

#pragma region ƒV[ƒ“‘JˆÚ

	if (DirectInput::keyTrigger(DIK_SPACE)||
		DirectInput::keyTrigger(DIK_RETURN) ||
		XInputManager::buttonTrigger(XInputManager::XINPUT_A_BUTTON,1) ||
		XInputManager::buttonTrigger(XInputManager::XINPUT_START_BUTTON,1))
		SceneChange::getInstance()->trueFeadFlag();
	
	SceneChange::getInstance()->update();
	
	if (SceneChange::getInstance()->getSceneChangeFlag())isEnd = true;

#pragma endregion


	Library::setCameraAngle(cameraAngle, { 0,0,0 }, { 0,0,0 });
	cameraAngle.y += 0.5f;
	cameraAngle.y = cameraAngle.y >= 360 ? 0 : cameraAngle.y;
}

void Title::draw()
{
	vertex v = PolygonManager::getInstance()->getPolygonVertex("fierd");
	heap h = PolygonManager::getInstance()->getPolygonVertex("fierd");
	Library::drawGraphic(v, h, 0);
	ObjectManager::getInstance()->draw();

	Library::drawSprite({ 380,140 }, titleSpr,&titleTex);
	Library::setSpriteScale(pushScale, pushSpr);
	Library::drawSprite(pushPos, pushSpr,&pushTex);

	//ƒV[ƒ“‘JˆÚ
	SceneChange::getInstance()->draw();

}

void Title::end()
{
	Player::setStopFlag(false);
	ObjectManager::getInstance()->allDeleteObject();
}

std::string Title::nextScene()
{
	return "StageSelect";
}
