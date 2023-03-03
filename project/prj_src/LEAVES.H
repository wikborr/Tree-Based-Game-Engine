//project's leaf configuration file
#pragma once

#include "Leaf.h"

//add every header in the project
#include "DemoGrass.h"
#include "MainCharacter.h"
#include "Enemy.h"

namespace LEAVES{
	std::map<std::string, std::function<Leaf*()>> mapLeaves();
}