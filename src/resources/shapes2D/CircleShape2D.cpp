#include "resources/shapes2D/CircleShape2D.h"
#include "servers/ResourceManager.h"

glm::vec2 CircleShape2D::collidesWith(glm::mat4 transform, std::vector<glm::vec2> polygonPoints, glm::vec2 velocity){
	std::vector<glm::vec2> shapePoints = this->toPoints(transform);
	bool circle = false;
	if(polygonPoints.size()<3){
		if(polygonPoints.size()<2) return {0.f, 0.f};
		circle = true;
	}
	if(circle){
		float circDist = glm::distance(shapePoints.at(0), polygonPoints.at(0));
		float radSum = shapePoints.at(1).x + polygonPoints.at(1).x;
		if(circDist > radSum){
			return {0.f, 0.f};
		}
		else{
			glm::vec2 circVec = glm::normalize(polygonPoints.at(0) - shapePoints.at(0));
			return (radSum - circDist)*circVec;
		}
	}
	else{
		std::vector<glm::vec2> normals = utils::getPolygonNormals(shapePoints);
		glm::vec2 collisionVector = glm::vec2(0.f);
		for(const auto& i: normals){
			float min1 = FLT_MAX, max1 = -FLT_MAX, min2 = FLT_MAX, max2 = -FLT_MAX;
			float circAxisValue = glm::dot(i, polygonPoints.at(0));
			min1 = circAxisValue - polygonPoints.at(1).x;
			max1 = circAxisValue + polygonPoints.at(1).x;
			
			for(const auto& j: polygonPoints){
				float axisValue = glm::dot(i, j);
				if(axisValue < min2) min2 = axisValue;
				if(axisValue > max2) max2 = axisValue;
			}
			
			if((min1 - max2 > 0) || (min2 - max1 > 0)){
				return {0.f, 0.f};
			}
			else{
				glm::vec2 axisVec = i;
				if((max1 - min2) < (max2 - min1)) axisVec*= (max1 - min2);
				else axisVec*= (max2 - min1);
				if(collisionVector != glm::vec2(0.f)){
					if(abs(axisVec.x) + abs(axisVec.y) < abs(collisionVector.x) + abs(collisionVector.y)){
						collisionVector = axisVec;
					}
				}
				else{
					collisionVector = axisVec;
				}
			}
		}
		return collisionVector;
	}
}

std::vector<glm::vec2> CircleShape2D::toPoints(glm::mat4 transform){
	std::vector<glm::vec2> shapePoints;
	shapePoints.push_back(glm::vec2(transform*glm::vec4(0.f, 0.f, 0.f, 1.f)));
	glm::vec2 transRadius = glm::vec2(transform*glm::vec4(this->radius, 0.f, 0.f, 1.f));
	shapePoints.push_back(glm::vec2(glm::length(transRadius)));
	return shapePoints;
}

void CircleShape2D::unload(){
	ResourceManager::ins().unloadCircShape(this);
}

glm::vec2 CircleShape2D::broadSize(glm::mat4 transform){
	std::vector<glm::vec2> points = this->toPoints(transform);
	return glm::vec2(points.at(1).x*2);
}

glm::vec2 CircleShape2D::baseSize(){
	return glm::vec2(this->radius*2);
}