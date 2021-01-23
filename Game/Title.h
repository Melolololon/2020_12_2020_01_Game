#pragma once
#include"Library.h"
#include"Scene.h"
class Title :public Scene
{
private:
	sprite titleSpr;
	sprite pushSpr;

	texture titleTex;
	texture pushTex;

	Vector2 pushPos;
	Vector2 pushScale;
	int pushScaleChangeTimer;
	const int PushScaleChangeTime = 60 * 4;

public:
	Title();
	~Title();

	void initialize();
	void update();
	void draw();
	void end();
	std::string nextScene();
};

