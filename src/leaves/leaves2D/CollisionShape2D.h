#pragma once

#include "leaves/Leaf2D.h"

//leaf for collision interaction
class CollisionShape2D: public Leaf2D{
public:
	CollisionShape2D(Leaf* parent = nullptr);
	virtual std::string leafOperation();

	
};