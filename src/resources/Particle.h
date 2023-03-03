#pragma once

#include "Settings.h"
#include "leaves/leaves2D/Sprite.h"

class Particle{
public:
	int references = 0;

	Sprite* spriteLeaf = nullptr;
	glm::vec2 velocity = glm::vec2(0.f);
	float timeLeft = -1.f;

	Particle();
	~Particle();
};
