#pragma once

#include "Leaf.h"

//leaf which makes all the child leaves appear at the top of the screen
class UILayer: public Leaf{
public:
	UILayer(Leaf* parent = nullptr);
	virtual std::string leafOperation() override;

	int layer = 1;
	bool visible = true;
};