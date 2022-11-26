#pragma once

#include "leaves/Leaf2D.h"

class DemoGrass: public Leaf2D{
public:
	bool move_up = false;
	bool move_down = false;
	bool move_left = false;
	bool move_right = false;
	bool rotate_left = false;
	bool rotate_right = false;
	bool scale_up = false;
	bool scale_down = false;

	double velocity = 50;
	
	void ready();
	void inputEvent();
	void update(double delta);
};
