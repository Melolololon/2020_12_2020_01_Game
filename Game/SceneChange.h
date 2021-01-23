#pragma once
#include"Library.h"
class SceneChange
{
private:
	sprite sceneChangeFadeSpr;
	Vector2 feadPos;
	Vector2 feadSize;

	int feadTimer;
	const int FeadTime = 60 * 1.5;
	bool feadFlag;

	SceneChange();
	~SceneChange();
public:
	SceneChange(SceneChange& s) = delete;
	SceneChange& operator=(SceneChange& s) = delete;
	static SceneChange* getInstance();

	void load();
	void initialize();
	void update();
	void draw();

	void trueFeadFlag();
	bool getSceneChangeFlag();
};

