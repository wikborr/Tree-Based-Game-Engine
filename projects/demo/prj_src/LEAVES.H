//project's leaf configuration file
#pragma once

#include <string>
#include <map>
#include <functional>
#include "Leaf.h"

//add every header in the project
#include "DemoGrass.h"

namespace LEAVES{
	std::map<std::string, std::function<Leaf*()>> mapLeaves();
}