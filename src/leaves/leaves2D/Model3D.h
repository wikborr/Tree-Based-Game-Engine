#pragma once

#include "leaves/Leaf2D.h"

//leaf for rendering 3D objects in 2D environment
class Model3D: public Leaf2D{
public:
	Model3D(Leaf* parent = nullptr);
	virtual ~Model3D();
	virtual std::string leafOperation() override;

	glm::vec3 position3d = glm::vec3(0.f, 0.f, -0.5f);
	glm::vec3 scale3d = glm::vec3(1.f);
	glm::vec3 rotationDegrees3d = glm::vec3(0.f);
	//model
};