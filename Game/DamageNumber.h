#pragma once
#include"Object.h"

//�_���[�W��^�������ɕ\�����鐔�l�N���X
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

