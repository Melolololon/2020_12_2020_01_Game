#pragma once
#include"Object.h"

//�q���Ƃ̓����蔻�肪�Ȃ��I�u�W�F�N�g
class DamageObject :public Object
{
public:
	enum DamageObjectType
	{
		DAMAGE_OBJECT_SYOGEKIHA,
	};

private:
	DamageObjectType objectType;

	int heapNum;
	static const int CreateNum;
	static int createCount;

public:
	DamageObject(const Vector3& pos,const Vector3& vel,const Vector3& spe,const DamageObjectType& type);
	~DamageObject();

	static void loadModel();

	void update() override;
	//�`��
	void draw() override;

	void hit(Object* object, CollisionType collisionType)override;

	void* getPtr()override;
};
