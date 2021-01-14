#pragma once
#include<string>
#include<unordered_map>
#include"Library.h"
class PolygonManager//これ親クラス作って処理まとめる?(サウンドとかも同じ処理書くだろうし)(シングルトンだから無理)
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
	void clearPolygonVertex();//clearだと指定できなくてバッファ削除できない


	void addPolygonHeap(std::string keyName, int* heap);
	void deletePolygonHeap(std::string keyName);
	int* getPolygonHeap(std::string keyName);
	void clearPolygonHeap();

	void clear();
};

