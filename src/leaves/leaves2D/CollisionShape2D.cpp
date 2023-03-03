#include "leaves/leaves2D/CollisionShape2D.h"

CollisionShape2D::CollisionShape2D(Leaf* parent){
	this->name = "CollisionShape2D";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"disabled", {"bool", &(this->disabled)}},
		{"oneWayCollision", {"bool", &(this->oneWayCollision)}},
		{"rectangleShape2D", {"rectangleShape2D", &(this->shape)}},
		{"circleShape2D", {"circleShape2D", &(this->shape)}},
		{"polygonShape2D", {"polygonShape2D", &(this->shape)}},
		{"lineShape2D", {"lineShape2D", &(this->shape)}},
	};
	this->propPointers.insert(props.begin(), props.end());
}

CollisionShape2D::~CollisionShape2D(){
	this->shape->unload();
}

std::string CollisionShape2D::leafOperation(){
	Leaf2D::leafOperation();
	return "";
}