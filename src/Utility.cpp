#include "Utility.h"

namespace utils{
	std::vector<glm::vec2> getPolygonNormals(std::vector<glm::vec2> polygonPoints){
		std::vector<glm::vec2> normalVector;
		for(int i = 0; i < polygonPoints.size() - 1; i++){
			glm::vec2 normal = glm::normalize(glm::vec2(-(polygonPoints.at(i+1).y - polygonPoints.at(i).y), polygonPoints.at(i+1).x - polygonPoints.at(i).x));
			if(normal.x < 0 || normal.y < 0) normal *= -1.f;
			if(std::find(normalVector.begin(), normalVector.end(), normal) == normalVector.end()){
				normalVector.push_back(normal);
			}
		}
		if(polygonPoints.size() > 2){
			glm::vec2 normal = glm::normalize(glm::vec2(-(polygonPoints.at(0).y - polygonPoints.at(polygonPoints.size() - 1).y), polygonPoints.at(0).x - polygonPoints.at(polygonPoints.size() - 1).x));
			if(normal.x < 0 || normal.y < 0) normal *= -1.f;
			if(std::find(normalVector.begin(), normalVector.end(), normal) == normalVector.end()){
				normalVector.push_back(normal);
			}
		}
		return normalVector;
	}
}