#pragma once
#include"Library.h"
#include <Scene.h>
class Caution :
	public Scene
{
private:
	sprite mesSpr;
	texture mesTex;

	sprite whiteBackSpr;

	int nextSceneTimer;
	static const int NextSceneTime;
public:
	Caution();
	~Caution();

	void initialize();
	void update();
	void draw();
	void end();
	std::string nextScene();
};

