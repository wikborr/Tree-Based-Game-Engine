#pragma once

#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <map>
#include <array>
#include <functional>
#include <stack>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>
#include <ft2build.h>
#include FT_FREETYPE_H 
//#include <AL/al.h>
//#include <AL/alc.h>

namespace utils{
	template<typename NewType, typename OldType>
	std::vector<NewType> vector2vector(const std::vector<OldType>& oldVector){
		std::vector<NewType> newVector;
		for(const auto& i: oldVector){
			newVector.push_back(static_cast<NewType>(i));
		}
		return newVector;
	}

	std::vector<glm::vec2> getPolygonNormals(std::vector<glm::vec2> polygonPoints);
}