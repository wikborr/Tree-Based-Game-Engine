#pragma once

#include "resources/shapes2D/ConvexPolygonShape2D.h"

class RectangleShape2D: public ConvexPolygonShape2D{
public:
	glm::vec2 size = glm::vec2(1.f);

	glm::vec2 collidesWith(glm::mat4 transform, std::vector<glm::vec2> polygonPoints, glm::vec2 velocity = glm::vec2(0.f)) override;
	std::vector<glm::vec2> toPoints(glm::mat4 transform) override;
	void unload() override;
	glm::vec2 broadSize(glm::mat4 transform) override;
	glm::vec2 baseSize() override;
};