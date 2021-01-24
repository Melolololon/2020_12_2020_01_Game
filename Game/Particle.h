#pragma once
#include"Object.h"
class Particle :public Object
{
public:
	enum ParticleType
	{
		PARTICLE_ENEMY_DEAD,
	};

private:

	ParticleType particleType;
	int deadTimer;
	const int DeadTime = 60 * 1;
public:
	Particle(const Vector3& pos,const ParticleType& parType);
	~Particle();

	void update();
	void draw();

};

