#pragma once
#include"Object.h"

//ダメージを与えた時に表示する数値クラス
class DamageNumber :public Object
{
private:
	int damage; 
	int deadTimer;
	const int DeadTime = 60 * 1;

	pipeline pl;
public:
	DamageNumber(const Vector3& pos,const int& damageNum);
	~DamageNumber();

	void update();
	void draw();

};

