#pragma once

#include "leaves/Leaf2D.h"
#include "resources/Shape2D.h"

//leaf to help with collision objects
class CollisionShape2D: public Leaf2D{
public:
	CollisionShape2D(Leaf* parent = nullptr);
	virtual ~CollisionShape2D();
	virtual std::string leafOperation() override;

	bool disabled = false;
	bool oneWayCollision = false;
	Shape2D* shape = nullptr;
};