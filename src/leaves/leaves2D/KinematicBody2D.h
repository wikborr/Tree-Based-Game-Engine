#pragma once

#include "leaves/leaves2D/PhysicsBody2D.h"

//leaf for moving bodies
class KinematicBody2D: public PhysicsBody2D{
private:
	std::vector<CollisionEvent> collisionEvents;
	glm::vec2 lastOverlapVector = glm::vec2(0.f);
	glm::vec2 lastVecSign = glm::vec2(0.f);
public:
	KinematicBody2D(Leaf* parent = nullptr);
	virtual std::string leafOperation() override;

	BodyType bodyType() override;
	std::vector<CollisionEvent> getCollisionEvents();

	glm::vec2 moveAndCollide(glm::vec2 velocity);
	bool isOnFloor();
	bool isOnCeiling();
	bool isOnWall();
};