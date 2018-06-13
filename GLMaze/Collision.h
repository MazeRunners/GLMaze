#pragma once
#include "reactphysics3d.h"

#include "ConcaveMesh.h"

class Collision {
public:
	Collision();
	~Collision();
	void update(float x, float y, float z);
	bool testCollision();

private:
	rp3d::CollisionWorld* world;
	rp3d::WorldSettings* settings;
	rp3d::CollisionBody* cameraBody;
	rp3d::ProxyShape* cameraProxy;
	rp3d::SphereShape* camearShape;
	ConcaveMesh* maze;
};

