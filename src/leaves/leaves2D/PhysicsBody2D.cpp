#include "leaves/leaves2D/PhysicsBody2D.h"

PhysicsBody2D::PhysicsBody2D(Leaf* parent){
	this->name = "PhysicsBody2D";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"bodyActive", {"bool", &(this->bodyActive)}},
	};
	this->propPointers.insert(props.begin(), props.end());
}

std::string PhysicsBody2D::leafOperation(){
	CollisionObject2D::leafOperation();
	return "";
}