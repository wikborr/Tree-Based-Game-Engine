#pragma once

#include "Leaf.h"
#include "resources/Shader.h"

//base class for game objects on screen
class Leaf2D: public Leaf{
public:
	Leaf2D(Leaf* parent = nullptr);
	virtual ~Leaf2D();
	virtual std::string leafOperation() override;
	virtual LeafType getLeafType() override;
	
	//global attributes
	glm::vec2 globalPosition = glm::vec2(0.0);
	glm::mat4 globalTransform = glm::mat4(1.0);

	//local attributes
	glm::vec2 position = glm::vec2(0.0);
	float rotationDegrees = 0.0;
	glm::vec2 scale = glm::vec2(1.0);
	glm::mat4 transform = glm::mat4(1.0);

	unsigned zIndex = 1;
	bool visible = true;
	glm::vec4 modulate = glm::vec4(1.0);
	glm::vec4 selfModulate = glm::vec4(1.0);
	Shader* leafShader = nullptr;

};