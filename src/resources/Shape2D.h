#pragma once

#include "Settings.h"

class Shape2D{
public:
	int references = 0;
	
	virtual glm::vec2 collidesWith(glm::mat4 transform, std::vector<glm::vec2> polygonPoints, glm::vec2 velocity = glm::vec2(0.f)) = 0;
	virtual std::vector<glm::vec2> toPoints(glm::mat4 transform) = 0;
	virtual void unload() = 0;

	virtual glm::vec2 broadSize(glm::mat4 transform) = 0;
	virtual glm::vec2 baseSize() = 0;
};