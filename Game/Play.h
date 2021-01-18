#pragma once
#include"Scene.h"
#include"Library.h"
#include"LibMath.h"
#include"Vector.h"
#include"Player.h"
class Play :public Scene
{
private:
	Player* player[2];
public:
	Play();
	~Play();

	void initialize();
	void update();
	void draw();
	void end();
	std::string nextScene();

};

