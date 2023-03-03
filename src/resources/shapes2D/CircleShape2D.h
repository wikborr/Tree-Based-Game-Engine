#pragma once

#include "resources/Shape2D.h"

class CircleShape2D: public Shape2D{
public:
	float radius = 1.f;

	glm::vec2 collidesWith(glm::mat4 transform, std::vector<glm::vec2> polygonPoints, glm::vec2 velocity = glm::vec2(0.f)) override;
	std::vector<glm::vec2> toPoints(glm::mat4 transform) override;
	void unload() override;

	glm::vec2 broadSize(glm::mat4 transform) override;
	glm::vec2 baseSize() override;
};