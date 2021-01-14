#include "ObjectManager.h"
#include"CollisionType.h"
#include"LibMath.h"


ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
}

ObjectManager* ObjectManager::getInstance()
{
	static ObjectManager inst;
	return &inst;
}

void ObjectManager::initialize() 
{
}

void ObjectManager::update() 
{
#pragma region update
	//拡張for文は、途中でサイズ変えるとダメ
	//変数用意してsize入れるか、一時的に別の配列に入れて、update終了後に追加
	std::vector<Object*>o = objects;
	for (auto& obj : o) 
	{
		obj->update();
	}

#pragma endregion

#pragma region collision

	//if文毎に関数呼ばずにあらかじめ取得したほうがいい
#pragma region 球と球
	Object::CollisionFlag f1;
	Object::CollisionFlag f2;

	for (auto& o1 : objects)
	{
		for (auto& o2 : objects)
		{
			f1 = o1->getCollisionFlag();
			f2 = o2->getCollisionFlag();

			//自分と比較、またはどちらかが判定確認しなくていい場合、無視
			if (o1 == o2 || !f1.sphere || !f2.sphere)continue;

			for (auto& c1 : o1->getSphereData())
			{
				for (auto& c2 : o2->getSphereData())
				{
					if (LibMath::sphereCollision
					(
						c1.position,
						c1.r,
						c2.position,
						c2.r
					))
					{
						o1->hit(o2, CollosionType::COLLISION_SPHERE);
					}
				}
			}

		}
	}

#pragma endregion


#pragma endregion

}

void ObjectManager::draw() 
{
	for (auto& o : objects)
	{
		o->draw();
	}

}

void ObjectManager::isDeadCheck()
{
	int size = objects.size();
	for (int i = 0; i < size; i++)
	{
		if (objects[i]->getIsDead())
		{
			delete objects[i];
			objects.erase(objects.begin() + i);
			i--;
			size--;
		}
	}
	objects.shrink_to_fit();

}

void ObjectManager::end() 
{
	allDeleteObject();
}

#pragma region 追加関数


void ObjectManager::addObject(Object* object)
{
	objects.push_back(object);
}

#pragma endregion


void ObjectManager::allDeleteObject()
{
	for (auto& o : objects) 
	{
		delete o;
	}
	objects.clear();
	objects.shrink_to_fit();

	
}
