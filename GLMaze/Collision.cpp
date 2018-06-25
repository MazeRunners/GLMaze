#include "Collision.h"

Collision::Collision() {
	settings = new rp3d::WorldSettings();
	world = new rp3d::CollisionWorld(*settings);

	rp3d::Vector3 initPosition(0.0, 0.0, 0.0);
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Transform initTransform(initPosition, initOrientation);
	cameraBody = world->createCollisionBody(initTransform);

	rp3d::decimal radius = rp3d::decimal(0.03);
	camearShape = new rp3d::SphereShape(radius);
	rp3d::Transform transform = rp3d::Transform::identity();
	cameraProxy = cameraBody->addCollisionShape(camearShape, transform);

	maze = new ConcaveMesh(world, "./resource/maze/maze.obj");

}

Collision::~Collision() {
	delete camearShape;
	delete maze;
	delete world;
	delete settings;
}

void Collision::updateCameraBody(float x, float y, float z) {
	rp3d::Vector3 position(x, y, z);
	rp3d::Quaternion orientation = rp3d::Quaternion::identity();
	rp3d::Transform newTransform(position, orientation);

	cameraBody->setTransform(newTransform);
}

bool Collision::testCollision() {
	return world->testOverlap(cameraBody, maze->getCollisionBody());
}
