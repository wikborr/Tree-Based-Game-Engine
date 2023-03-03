#pragma once

#include "leaves/UILayer.h"

//leaf which makes all the child leaves appear at the back of the screen
class BGLayer: public UILayer{
public:
	BGLayer(Leaf* parent = nullptr);
	virtual std::string leafOperation() override;

	glm::vec2 startCameraPos = {0.f,0.f};
	glm::vec2 mirrorOffset = {0.f,0.f};
	glm::vec2 scrollScale = {0.f,0.f};
};