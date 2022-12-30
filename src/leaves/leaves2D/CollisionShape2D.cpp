#include "leaves/leaves2D/CollisionShape2D.h"

CollisionShape2D::CollisionShape2D(Leaf* parent){
	this->name = "CollisionShape2D";
	std::map<std::string, std::pair<std::string, void*>> props = {
		
	};
	this->propPointers.insert(props.begin(), props.end());
}