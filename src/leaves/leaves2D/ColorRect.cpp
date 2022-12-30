#include "leaves/leaves2D/ColorRect.h"

ColorRect::ColorRect(Leaf* parent){
	this->name = "ColorRect";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"color", {"vec3", &(this->color)}},
		{"width", {"float", &(this->width)}},
		{"height", {"float", &(this->height)}},
	};
	this->propPointers.insert(props.begin(), props.end());
}

std::string ColorRect::leafOperation(){
	Leaf2D::leafOperation();
	return "";
}