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
	std::unordered_map<std::string, texture> textures;
	std::unordered_map<std::string, point>points;

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


	void addTexture(std::string keyName, texture texture);
	void deleteTexture(std::string keyName);
	texture getTexture(std::string keyName);
	void clearTexture();


	void addPolygonPoint(std::string keyName, point point);
	void deletePolygonPoint(std::string keyName);
	point getPolygonPoint(std::string keyName);
	void clearPolygonPoint();

	void clear();
};

