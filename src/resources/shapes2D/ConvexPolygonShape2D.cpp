#include "resources/shapes2D/ConvexPolygonShape2D.h"

glm::vec2 ConvexPolygonShape2D::collidesWith(glm::mat4 transform, std::vector<glm::vec2> polygonPoints, glm::vec2 velocity){
	glm::vec2 absVel = glm::normalize(glm::abs(velocity));
	std::vector<glm::vec2> shapePoints = this->toPoints(transform);
	bool circle = false;
	if(polygonPoints.size()<3){
		if(polygonPoints.size()<2) return {0.f, 0.f};
		circle = true;
	}
	std::vector<glm::vec2> normals = utils::getPolygonNormals(shapePoints);
	if(!circle){
		std::vector<glm::vec2> normals2 = utils::getPolygonNormals(polygonPoints);
		normals.insert(normals.end(), normals2.begin(), normals2.end());
	}
	glm::vec2 collisionVector = glm::vec2(0.f);
	bool collisionVectorSet = false;
	for(const auto& i: normals){
		float min1 = FLT_MAX, max1 = -FLT_MAX, min2 = FLT_MAX, max2 = -FLT_MAX;
		for(const auto& j: shapePoints){
			float axisValue = glm::dot(i, j);
			if(axisValue < min1) min1 = axisValue;
			if(axisValue > max1) max1 = axisValue;
		}
		if(circle){
			float circAxisValue = glm::dot(i, polygonPoints.at(0));
			min2 = circAxisValue - polygonPoints.at(1).x;
			max2 = circAxisValue + polygonPoints.at(1).x;
		}
		else{
			for(const auto& j: polygonPoints){
				float axisValue = glm::dot(i, j);
				if(axisValue < min2) min2 = axisValue;
				if(axisValue > max2) max2 = axisValue;
			}
		}
		if((min1 - max2 > 0) || (min2 - max1 > 0)){
			return {0.f, 0.f};
		}
		else{
			glm::vec2 axisVec = i;
			float dist = 0;
			if((max1 - min2) < (max2 - min1)) dist = (max1 - min2);
			else dist = (max2 - min1);
			axisVec *= dist;
			if(collisionVectorSet){
				if(abs(axisVec.x) + abs(axisVec.y) < abs(collisionVector.x) + abs(collisionVector.y)){
					collisionVector = axisVec;
				}
			}
			else{
				collisionVector = axisVec;
				collisionVectorSet = true;
			}
			
		}
	}
	//better collision response (to implement)
	/* if(!circle){
		glm::vec2 perpVector = glm::normalize(glm::vec2(velocity.y, -velocity.x));
		float min1 = FLT_MAX, max1 = -FLT_MAX, min2 = FLT_MAX, max2 = -FLT_MAX;
		for(const auto& i: shapePoints){
			float axisValue = glm::dot(perpVector, i);
			if(axisValue < min1) min1 = axisValue;
			if(axisValue > max1) max1 = axisValue;
		}
		for(const auto& i: polygonPoints){
			float axisValue = glm::dot(perpVector, i);
			if(axisValue < min2) min2 = axisValue;
			if(axisValue > max2) max2 = axisValue;
		}

		std::vector<glm::vec2> possibleVectors;
		for(const auto& i: shapePoints){
		}

	} */
	return collisionVector;
}

glm::vec2 ConvexPolygonShape2D::broadSize(glm::mat4 transform){
	std::vector<glm::vec2> points = this->toPoints(transform);
	float minX = FLT_MAX, maxX = -FLT_MAX, minY = FLT_MAX, maxY = -FLT_MAX;
	for(const auto& i: points){
		if(i.x < minX) minX = i.x;
		if(i.x > maxX) maxX = i.x;
		if(i.y < minY) minY = i.y;
		if(i.y > maxY) maxY = i.y;
	}
	return glm::vec2(maxX - minX, maxY - minY);
}