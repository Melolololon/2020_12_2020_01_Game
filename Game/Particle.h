#pragma once
#include"Object.h"
class Particle :public Object
{
public:
	enum ParticleType
	{
		PARTICLE_ENEMY_DEAD,
		PARTICLE_PLAYER_LEFT,
		PARTICLE_PLAYER_RIGHT,

	};

private:

	ParticleType particleType;
	Vector3 scale;
	int heapNum;


	static int createCount;
	static const int CreateNum;
	
public:
	Particle(const Vector3& pos,const ParticleType& parType);
	~Particle();
	static void loadModel();

	void update();
	void draw();

};

