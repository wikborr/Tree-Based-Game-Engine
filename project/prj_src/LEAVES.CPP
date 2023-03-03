#include "LEAVES.H"

std::map<std::string, std::function<Leaf*()>> LEAVES::mapLeaves(){
	return {
		{"DemoGrass", []()->Leaf*{return new DemoGrass;}},
		{"MainCharacter", []()->Leaf*{return new MainCharacter;}},
		{"Enemy", []()->Leaf*{return new Enemy;}},
	};
}
