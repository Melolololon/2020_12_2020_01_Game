#pragma once
#include<string>
#include<unordered_map>
#include"Library.h"
class PolygonManager//����e�N���X����ď����܂Ƃ߂�?(�T�E���h�Ƃ������������������낤��)(�V���O���g�������疳��)
{
private:
	PolygonManager();
	~PolygonManager();

	std::unordered_map<std::string, vertex> verts;
	std::unordered_map<std::string, heap> heaps;

public:
	PolygonManager(const PolygonManager& p) = delete;
	PolygonManager& operator=(const PolygonManager& p) = delete;

	static PolygonManager* getInstance();


	void addPolygonVertex(std::string keyName, int* vert);
	void deletePolygonVertex(std::string keyName);
	int* getPolygonVertex(std::string keyName);
	void clearPolygonVertex();//clear���Ǝw��ł��Ȃ��ăo�b�t�@�폜�ł��Ȃ�


	void addPolygonHeap(std::string keyName, int* heap);
	void deletePolygonHeap(std::string keyName);
	int* getPolygonHeap(std::string keyName);
	void clearPolygonHeap();

	void clear();
};

