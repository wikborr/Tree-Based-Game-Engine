#include "resources/shapes2D/RectangleShape2D.h"
#include "servers/ResourceManager.h"

glm::vec2 RectangleShape2D::collidesWith(glm::mat4 transform, std::vector<glm::vec2> polygonPoints, glm::vec2 velocity){
	return ConvexPolygonShape2D::collidesWith(transform, polygonPoints, velocity);
}

std::vector<glm::vec2> RectangleShape2D::toPoints(glm::mat4 transform){
	std::vector<glm::vec2> shapePoints;
	glm::vec2 sizePoints = glm::vec2(this->size.x/2, this->size.y/2);
	shapePoints.push_back(glm::vec2(sizePoints.x, sizePoints.y));
	shapePoints.push_back(glm::vec2(-sizePoints.x, sizePoints.y));
	shapePoints.push_back(glm::vec2(-sizePoints.x, -sizePoints.y));
	shapePoints.push_back(glm::vec2(sizePoints.x, -sizePoints.y));
	for(auto& i: shapePoints){
		i = glm::vec2(transform*glm::vec4(i, 0.f, 1.f));
	}
	return shapePoints;
}

void RectangleShape2D::unload(){
	ResourceManager::ins().unloadRectShape(this);
}

glm::vec2 RectangleShape2D::broadSize(glm::mat4 transform){
	return ConvexPolygonShape2D::broadSize(transform);
}

glm::vec2 RectangleShape2D::baseSize(){
	return this->size;
}