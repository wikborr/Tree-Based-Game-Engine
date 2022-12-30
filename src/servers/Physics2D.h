#pragma once

#include "LeafTree.h"

//server singleton for calculating collisions
class Physics2D{
public:
	//singleton method
	static Physics2D& ins();
};