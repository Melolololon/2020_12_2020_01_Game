#pragma once
#include"Library.h"
class Game final
{
private:
	Game();
	~Game();

	void initialize();
	void update();
	void draw();
	void end();


	pipeline billboardPL;

	vertex waterV;
	heap waterH;
public:
	Game(const Game&) = delete;
	Game& operator=(const Game) = delete;

	static Game* getInstance();
	void run();
};

