#pragma once

#include "leaves/Leaf2D.h"

//leaf for rendering a rectangle of one color
class ColorRect: public Leaf2D{
public:
	ColorRect(Leaf* parent = nullptr);
	virtual std::string leafOperation() override;

	glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
	float width = 1.0;
	float height = 1.0;
};