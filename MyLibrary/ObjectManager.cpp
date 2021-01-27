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
	checkCollision.board = false;
	checkCollision.lineSegment = false;
	checkCollision.plane = false;
	checkCollision.ray = false;
	checkCollision.sphere = false;
}

void ObjectManager::update() 
{
#pragma region update
	//�g��for���́A�r���ŃT�C�Y�ς���ƃ_��
	//�ϐ��p�ӂ���size����邩�A�ꎞ�I�ɕʂ̔z��ɓ���āAupdate�I����ɒǉ�
	std::vector<Object*>o = objects;
	for (auto& obj : o) 
	{
		obj->update();
	}

#pragma endregion

#pragma region collision

	CollisionFlag f1;
	CollisionFlag f2;
	int forCnt[2] = { 0,0 };

	//if�����Ɋ֐��Ă΂��ɂ��炩���ߎ擾�����ق�������
#pragma region ���Ƌ�

	if (checkCollision.sphere) 
	{
		for (auto& o1 : o)
		{
			for (auto& o2 : o)
			{
				f1 = o1->getCollisionFlag();
				f2 = o2->getCollisionFlag();

				//�����Ɣ�r�A�܂��͂ǂ��炩������m�F���Ȃ��Ă����ꍇ�A����
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
							o1->hit(o2, CollisionType::COLLISION_SPHERE);
						}
					}
				}

			}
		}
	}
#pragma endregion

#pragma region �����Ɣ�
	if (checkCollision.lineSegment && checkCollision.board) 
	{
		for (auto& o1 : o)
		{
			for (auto& o2 : o)
			{
				f1 = o1->getCollisionFlag();
				f2 = o2->getCollisionFlag();

				if (o1 == o2 || !f1.lineSegment || !f2.board)continue;

				for (auto& c1 : o1->getLineSegmentData())
				{
					for (auto& c2 : o2->getBoardData())
					{
						std::vector<Vector3>p(4);
						p[0] = c2.leftDownPos;
						p[1] = c2.leftUpPos;
						p[3] = c2.rightUpPos;
						p[2] = c2.rightDownPos;

						Vector3 hitPos = { 0,0,0 };
						if (LibMath::lineSegmentAndBoardCollision
						(
							c1.position[0],
							c1.position[1],
							c2.normal,
							c2.position,
							p,
							&hitPos

						))
						{
							o1->setLineSegmentDataHitPos(hitPos, forCnt[1]);
							o2->setBoardDataHitPos(hitPos, forCnt[0]);

							o1->hit(o2, CollisionType::COLLISION_LINESEGMENT);
							o2->hit(o1, CollisionType::COLLISION_BOARD);
						}
						forCnt[0]++;
					}
					forCnt[1]++;
					forCnt[0] = 0;
				}

				forCnt[0] = 0;
				forCnt[1] = 0;
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

#pragma region �ǉ��֐�


void ObjectManager::addObject(Object* object)
{
	objects.push_back(object);
}

#pragma endregion

void ObjectManager::setCollisionFlag3D(const CollisionFlag& type)
{
	checkCollision = type;
}

void ObjectManager::allDeleteObject()
{
	for (auto& o : objects) 
	{
		delete o;
	}
	objects.clear();
	objects.shrink_to_fit();

	
}
