#pragma once

#include "leaves/Leaf2D.h"

//basic leaf for physics operations
class CollisionObject2D: public Leaf2D{
public:
	CollisionObject2D(Leaf* parent = nullptr);
	virtual std::string leafOperation() override;

	int collisionLayer = 1;
	int collisionMask = 1;
};

struct CollisionEvent{
	CollisionObject2D* body;
	glm::vec2 overlapVector;
};