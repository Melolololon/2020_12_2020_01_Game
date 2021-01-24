#include "Particle.h"

Particle::Particle(const Vector3& pos, const ParticleType& parType)
{
	position = pos;
	isDead = false;

	deadTimer = 0;
}

Particle::~Particle()
{
}

void Particle::update()
{
}

void Particle::draw()
{
}
