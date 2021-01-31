#pragma once
#include"Object.h"

//ヒモとの当たり判定がないオブジェクト
class DamageObject :public Object
{
public:
	enum DamageObjectType
	{
		DAMAGE_OBJECT_SYOGEKIHA,
	};

private:
	DamageObjectType objectType;
public:
	DamageObject(const Vector3& pos,const Vector3& vel,const Vector3& spe,const DamageObjectType& type);
	~DamageObject();

	void update() override;
	//描画
	void draw() override;

	void hit(Object* object, CollisionType collisionType)override;

	void* getPtr()override;
};

