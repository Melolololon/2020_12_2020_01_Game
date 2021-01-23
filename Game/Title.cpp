#include "Title.h"
#include"SceneChange.h"

Title::Title()
{
	SceneChange::getInstance()->load();
	SceneChange::getInstance()->initialize();

	Library::createSprite(&titleSpr);
	Library::createSprite(&pushSpr);

	titleTex = Library::loadTexture(L"Resources/Texture/titleKari.png");
	pushTex = Library::loadTexture(L"Resources/Texture/pushKari.png");
}

Title::~Title()
{

}

void Title::initialize()
{
	pushPos = { 500,500 };
	pushScale = { 1,1};
	pushScaleChangeTimer = 0;
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
		pushPos = { 500,500 };
		pushScale = { 1,1 };
		pushScaleChangeTimer = 0;
	}

#pragma region シーン遷移

	if (DirectInput::keyTrigger(DIK_SPACE)||
		DirectInput::buttonTrigger(AButton))
		SceneChange::getInstance()->trueFeadFlag();
	
	SceneChange::getInstance()->update();
	
	if (SceneChange::getInstance()->getSceneChangeFlag())isEnd = true;

#pragma endregion

}

void Title::draw()
{
	Library::drawSprite({ 100,50 }, titleSpr,&titleTex);
	Library::setSpriteScale(pushScale, pushSpr);
	Library::drawSprite(pushPos, pushSpr,&pushTex);

	//シーン遷移
	SceneChange::getInstance()->draw();
}

void Title::end()
{
}

std::string Title::nextScene()
{
	return "StageSelect";
}
