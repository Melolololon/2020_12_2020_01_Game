#pragma once
#include"Vector.h"


enum CollosionType 
{
	COLLISION_RECT,
	COLLISION_CIRCLE,
	COLLISION_SPHERE,
	COLLISION_BOARD,
	COLLISION_LINESEGMENT,
	COLLISION_LAY,

};

#pragma region 2D

struct RectData
{
	//éläpå`ÇÃç∂è„
	Vector2 position;
	//ï”ÇÃí∑Ç≥
	Vector2 size;
};

struct CircleData
{
	Vector2 position;
	float r;
};

#pragma endregion

#pragma region 3D

struct SphereData
{
	Vector3 position;

	//îºåa
	float r;
};

struct BoardData
{
	Vector3 normal;
	float distance;
};

struct LineSegmentData
{
	Vector3 position[2];
};

struct LayData 
{
	Vector3 pos;
	Vector3 direction;
};

#pragma endregion

