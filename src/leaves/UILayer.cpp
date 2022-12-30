#include "leaves/UILayer.h"

UILayer::UILayer(Leaf* parent){
	this->name = "UILayer";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"layer", {"int", &(this->layer)}},
		{"visible", {"bool", &(this->visible)}},
	};
	this->propPointers.insert(props.begin(), props.end());
}

std::string UILayer::leafOperation(){
	return "";
}