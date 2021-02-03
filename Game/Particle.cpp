#include "Particle.h"
#include"PolygonManager.h"
#include"PipelineManager.h"
int Particle::createCount;
const int Particle::CreateNum = 100;
Particle::Particle(const Vector3& pos, const ParticleType& parType)
{
	this->particleType = parType;
	position = pos;
	velocity.x = Library::getRandomNumber(5) - 2;
	velocity.y = 3;
	velocity.z = Library::getRandomNumber(5) - 2;
	Vector3::normalize(velocity);
	speed = 0.1f;
	isDead = false;

	collisionFlag.board = false;
	collisionFlag.ray = false;
	collisionFlag.plane = false;
	collisionFlag.lineSegment = false;
	collisionFlag.sphere = false;

	vertexHandle = PolygonManager::getInstance()->getPolygonVertex("bakuhatuPar");
	heapHandle = PolygonManager::getInstance()->getPolygonHeap("bakuhatuPar");
	heapNum = createCount;
	createCount++;
	createCount = createCount >= CreateNum ? 0 : createCount;

	Library::setPosition(position, heapHandle, heapNum);
	scale = 1;

}

Particle::~Particle()
{
}

void Particle::loadModel()
{
	vertex v;
	heap h;
	Library::createBoard({ 0.75,0.75 }, dimention3D, &v);
	Library::createHeapData(L"Resources/Texture/bakuhatuPar.png", CreateNum, &h);
	PolygonManager::getInstance()->addPolygonVertex("bakuhatuPar", v);
	PolygonManager::getInstance()->addPolygonHeap("bakuhatuPar", h);
}

void Particle::update()
{
	velocity.y -= 0.05f;
	position += velocity * speed;
	Library::setPosition(position, heapHandle, heapNum);

	if (position.y <= -40)isDead = true;

	/*Vector3 subScale = { 0,0.025,0.025 };
	scale -= subScale;*/
	//Library::setScale(scale, heapHandle, heapNum);


}

void Particle::draw()
{
	if(particleType == PARTICLE_PLAYER_LEFT)
	{
		Library::setAddColor({ 0,0,200,0 }, heapHandle, heapNum);
		Library::setSubColor({ 255,0,0,0 }, heapHandle, heapNum);
	}
	else if (particleType == PARTICLE_PLAYER_RIGHT)
	{
		Library::setAddColor({ 60,0,0,0 }, heapHandle, heapNum);
		Library::setSubColor({ 0,0,0,0 }, heapHandle, heapNum);
	}
	else
	{
		Library::setAddColor({ 0,0,0,0 }, heapHandle, heapNum);
		Library::setSubColor({ 0,0,0,0 }, heapHandle, heapNum);
	}

	Library::setPipeline(PipelineManager::getInstance()->getPipeline("billboardPL"));
	Library::setIsBillboard(true, true, true);
	//Library::setMulColor({ 255,255,255,Color::alphaChangePar(alpha) }, heapHandle, heapNum);
	Library::drawGraphic(vertexHandle, heapHandle, heapNum);
	Library::setPipeline(PIPELINE_NORMAL);
	Library::setIsBillboard(false,false,false);
}
