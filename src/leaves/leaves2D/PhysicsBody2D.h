#pragma once

#include "leaves/leaves2D/CollisionObject2D.h"

enum BodyType{
	STATIC_BODY,
	KINEMATIC_BODY
};

//base leaf for physics objects
class PhysicsBody2D: public CollisionObject2D{
public:
	PhysicsBody2D(Leaf* parent = nullptr);
	virtual std::string leafOperation() override;

	bool bodyActive = true;
	virtual BodyType bodyType() = 0;
};