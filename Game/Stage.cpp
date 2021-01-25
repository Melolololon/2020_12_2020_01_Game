#include "Stage.h"

Stage::Stage(){}
Stage::~Stage(){}
Stage* Stage::getInstance()
{
	static Stage s;
	return &s;
}

void Stage::initialize(const int& stageNum)
{
	stage = stageNum;
}

void Stage::update()
{
	switch (stage)
	{
	case 0:
		break;
	}
}

void Stage::draw()
{
}
