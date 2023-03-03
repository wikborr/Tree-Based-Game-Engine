#pragma once

#include "leaves/leaves2D/Area2D.h"

class Enemy: public Area2D{
public:
	bool firstUpdateComplete = false;
	int activated = 0;
	
	void ready() override;
	void update(double delta) override;
};