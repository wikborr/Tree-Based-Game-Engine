#include "CollisionObject2D.h"

CollisionObject2D::CollisionObject2D(Leaf* parent){
	this->name = "CollisionObject2D";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"collisionLayer", {"int", &(this->collisionLayer)}},
		{"collisionMask", {"int", &(this->collisionMask)}},
	};
	this->propPointers.insert(props.begin(), props.end());
}

std::string CollisionObject2D::leafOperation(){
	Leaf2D::leafOperation();
	return "";
}