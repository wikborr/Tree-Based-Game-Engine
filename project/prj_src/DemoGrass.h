#pragma once

#include "leaves/Leaf2D.h"

class DemoGrass: public Leaf2D{
public:
	int bgAnimFrames = 1;

	void ready() override;
	void inputEvent() override;
	void update(double delta) override;
};
