#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "servers/FileReader.h"

namespace utils{
	template<typename NewType, typename OldType>
	std::vector<NewType> vector2vector(const std::vector<OldType>& oldVector){
		std::vector<NewType> newVector;
		for(const auto& i: oldVector){
			newVector.push_back(static_cast<NewType>(i));
		}
		return newVector;
	}
}