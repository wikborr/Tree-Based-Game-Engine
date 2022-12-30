#pragma once

#include "leaves/Leaf2D.h"

//leaf for rendering a rectangle of one color
class Camera2D: public Leaf2D{
protected:
	double lastTime = 0.0;
public:
	Camera2D(Leaf* parent = nullptr);
	virtual std::string leafOperation();

	bool cameraReady = false;
	glm::vec2 actualPosition = glm::vec2(0.f);
	glm::vec2 smoothedPosition = glm::vec2(0.f);

	bool current = false;
	glm::vec2 offset = glm::vec2(0.f);
	glm::vec2 zoom = glm::vec2(1.f);

	//camera limits
	int limitUp = 10000000;
	int limitDown = -10000000;
	int limitLeft = -10000000;
	int limitRight = 10000000;

	float dragMarginUp = 0.f;
	float dragMarginDown = 0.f;
	float dragMarginLeft = 0.f;
	float dragMarginRight = 0.f;

	bool smoothingEnabled = false;
	float smoothingSpeed = 5.f;

	void makeCurrent();
};