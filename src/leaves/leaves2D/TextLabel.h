#pragma once

#include "leaves/Leaf2D.h"
#include "resources/Font.h"

//leaf for text rendering
class TextLabel: public Leaf2D{
private:
	std::string prevText = "";
	unsigned prevTextSize = 18u;
	glm::ivec2 totalTextureSize = glm::ivec2(0);
	void updateText();
public:
	TextLabel(Leaf* parent = nullptr);
	virtual ~TextLabel();
	virtual std::string leafOperation() override;

	std::string text = "";
	unsigned textSize = 18u;
	float percentVisible = 1.f;
	glm::vec3 color = glm::vec3(1.f);
	unsigned outline = 0;
	glm::vec3 outLineColor = glm::vec3(0.f);
	Font* font = nullptr;

	std::vector<Character*> stringTextures;
};