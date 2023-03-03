#include "leaves/leaves2D/StaticBody2D.h"

StaticBody2D::StaticBody2D(Leaf* parent){
	this->name = "StaticBody2D";
	std::map<std::string, std::pair<std::string, void*>> props = {
	};
	this->propPointers.insert(props.begin(), props.end());
}

BodyType StaticBody2D::bodyType(){
	return STATIC_BODY;
}

std::string StaticBody2D::leafOperation(){
	PhysicsBody2D::leafOperation();
	return "";
}