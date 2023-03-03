#pragma once

#include "leaves/leaves2D/KinematicBody2D.h"

class MainCharacter: public KinematicBody2D{
public:
	bool move_up = false;
	bool move_down = false;
	bool move_left = false;
	bool move_right = false;
	bool cameraUp = false;
	bool cameraDown = false;
	bool cameraLeft = false;
	bool cameraRight = false;
	bool rotate_left = false;
	bool rotate_right = false;
	bool speedUp = false;

	int walkingAnimFrames = 0;
	bool jumping = false;

	const double moveVelocity = 50;
	const double gravity = 2;
	const double jumpVelocity = 3000;

	glm::vec2 velocity = glm::vec2(0.f);
	bool noInput = false;
	
	void ready() override;
	void inputEvent() override;
	void update(double delta) override;

	void collision();
};
