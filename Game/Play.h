#pragma once
#include"Scene.h"
#include"Library.h"
#include"LibMath.h"
#include"Vector.h"
class Play :public Scene
{
private:
	static Vector3 scrollSpeed;

public:
	Play();
	~Play();

	void initialize();
	void update();
	void draw();
	void end();
	std::string nextScene();

	static Vector3 getScrollSpeed();
};

