#pragma once
#include"Object.h"
#include"CollisionType.h"
#include<vector>
class ObjectManager
{
private:
	ObjectManager();
	~ObjectManager();

	std::vector<Object*>objects;

	CollisionFlag checkCollision;
public:
	ObjectManager(const ObjectManager& obj) = delete;
	ObjectManager& operator=(const ObjectManager& obj) = delete;

	static ObjectManager* getInstance();

	void initialize();
	void update();
	void draw();
	void isDeadCheck();
	void end();

	void setCollisionFlag3D(const CollisionFlag& type);

	void addObject(Object* object);


	void allDeleteObject();
};

