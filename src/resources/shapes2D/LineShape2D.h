#pragma once

#include "resources/shapes2D/ConvexPolygonShape2D.h"

class LineShape2D: public ConvexPolygonShape2D{
public:
	std::pair<glm::vec2, glm::vec2> linePoints = {{0.f, 0.f},{1.f, 1.f}};

	glm::vec2 collidesWith(glm::mat4 transform, std::vector<glm::vec2> polygonPoints, glm::vec2 velocity = glm::vec2(0.f)) override;
	std::vector<glm::vec2> toPoints(glm::mat4 transform) override;
	void unload() override;
	glm::vec2 broadSize(glm::mat4 transform) override;
	glm::vec2 baseSize() override;
};