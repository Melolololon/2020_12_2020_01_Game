#include "Caution.h"
#include"SceneChange.h"

const int Caution::NextSceneTime = 60 * 3;

Caution::Caution()
{
	Library::createSprite(&whiteBackSpr);
	Library::createSprite(&mesSpr);
	mesTex = Library::loadTexture(L"Resources/Texture/cauTex.png");


}
Caution::~Caution(){}

void Caution::initialize()
{
	nextSceneTimer = 0;

}
void Caution::update()
{
	nextSceneTimer++;
	if(nextSceneTimer >= NextSceneTime)
		SceneChange::getInstance()->trueFeadFlag();

	SceneChange::getInstance()->update();

	if (SceneChange::getInstance()->getSceneChangeFlag())
		isEnd = true;
}

void Caution::draw()
{
	Library::drawBox({ 0,0 }, { 1280,720 }, { 255,255,255,255 }, whiteBackSpr);
	Library::drawSprite({ 160,230 }, mesSpr, &mesTex);
	SceneChange::getInstance()->draw();
}
void Caution::end(){}
std::string Caution::nextScene()
{
	return "Title";
}
