#include "resources/shapes2D/LineShape2D.h"
#include "servers/ResourceManager.h"

const float LINE_DELTA = 0.1f;

glm::vec2 LineShape2D::collidesWith(glm::mat4 transform, std::vector<glm::vec2> polygonPoints, glm::vec2 velocity){
	return ConvexPolygonShape2D::collidesWith(transform, polygonPoints, velocity);
}

std::vector<glm::vec2> LineShape2D::toPoints(glm::mat4 transform){
	std::vector<glm::vec2> shapePoints;
	shapePoints.push_back(this->linePoints.second + LINE_DELTA);
	shapePoints.push_back(this->linePoints.first + LINE_DELTA);
	shapePoints.push_back(this->linePoints.first - LINE_DELTA);
	shapePoints.push_back(this->linePoints.second - LINE_DELTA);
	for(auto& i: shapePoints){
		i = glm::vec2(transform*glm::vec4(i, 0.f, 1.f));
	}
	return shapePoints;
}

void LineShape2D::unload(){
	ResourceManager::ins().unloadLineShape(this);
}

glm::vec2 LineShape2D::broadSize(glm::mat4 transform){
	return ConvexPolygonShape2D::broadSize(transform);
}

glm::vec2 LineShape2D::baseSize(){
	return glm::vec2(abs(this->linePoints.second.x - this->linePoints.second.x), abs(this->linePoints.second.y - this->linePoints.second.y));
}