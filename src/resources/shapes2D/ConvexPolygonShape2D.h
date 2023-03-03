#pragma once

#include "resources/Shape2D.h"

class ConvexPolygonShape2D: public Shape2D{
public:
	glm::vec2 collidesWith(glm::mat4 transform, std::vector<glm::vec2> polygonPoints, glm::vec2 velocity = glm::vec2(0.f)) override;
	glm::vec2 broadSize(glm::mat4 transform) override;
};