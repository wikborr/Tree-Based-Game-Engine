#pragma once

#include "LeafTree.h"
#include "leaves/leaves2D/PhysicsBody2D.h"

//server singleton for calculating collisions
class Physics2D{
private:
	glm::vec4 getObjectBroadSize(CollisionObject2D* collisionObject);
public:
	//singleton method
	static Physics2D& ins();

	LeafTree* leafTree = nullptr;

	std::string init(LeafTree* leafTree);
	std::vector<PhysicsBody2D*> getCollidingBodies(CollisionObject2D* collisionObject);
	std::vector<CollisionEvent> getCollisionEvents(CollisionObject2D* collisionObject, glm::vec2 velocity = glm::vec2(0.f));
	void updateShapePositions(CollisionObject2D* collisionObject);
};