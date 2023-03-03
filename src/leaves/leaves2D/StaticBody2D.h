#pragma once

#include "leaves/leaves2D/PhysicsBody2D.h"

//leaf for static bodies
class StaticBody2D: public PhysicsBody2D{
public:
	StaticBody2D(Leaf* parent = nullptr);
	virtual std::string leafOperation() override;

	BodyType bodyType() override;
};