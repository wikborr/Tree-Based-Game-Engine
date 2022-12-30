#include "leaves/BGLayer.h"

BGLayer::BGLayer(Leaf* parent){
	this->name = "BGLayer";
	this->layer = -1;
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"startCameraPos", {"vec2", &(this->startCameraPos)}},
		{"mirrorOffset", {"vec2", &(this->mirrorOffset)}},
		{"scrollScale", {"vec2", &(this->scrollScale)}},
	};
	this->propPointers.insert(props.begin(), props.end());
}

std::string BGLayer::leafOperation(){
	return "";
}