#pragma once
#include"Library.h"
#include"Scene.h"
class Title :public Scene
{
private:
	sprite titleSpr;
	texture titleTex;

public:
	Title();
	~Title();

	void initialize();
	void update();
	void draw();
	void end();
	std::string nextScene();
};

