#include "resources/shapes2D/PolygonShape2D.h"
#include "servers/ResourceManager.h"

glm::vec2 PolygonShape2D::collidesWith(glm::mat4 transform, std::vector<glm::vec2> polygonPoints, glm::vec2 velocity){
	return ConvexPolygonShape2D::collidesWith(transform, polygonPoints, velocity);
}

std::vector<glm::vec2> PolygonShape2D::toPoints(glm::mat4 transform){
	std::vector<glm::vec2> shapePoints = this->points;
	for(auto& i: shapePoints){
		i = glm::vec2(transform*glm::vec4(i, 0.f, 1.f));
	}
	return shapePoints;
}

void PolygonShape2D::unload(){
	ResourceManager::ins().unloadPolyShape(this);
}

glm::vec2 PolygonShape2D::broadSize(glm::mat4 transform){
	return ConvexPolygonShape2D::broadSize(transform);
}

glm::vec2 PolygonShape2D::baseSize(){
	return {0.f, 0.f};
}