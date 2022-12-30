#include "leaves/leaves2D/Camera2D.h"

const float POSITION_EPSILON = 0.1;

Camera2D::Camera2D(Leaf* parent){
	this->name = "Camera2D";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"current", {"bool", &(this->current)}},
		{"offset", {"vec2", &(this->offset)}},
		{"zoom", {"vec2", &(this->zoom)}},
		{"limitUp", {"int", &(this->limitUp)}},
		{"limitDown", {"int", &(this->limitDown)}},
		{"limitLeft", {"int", &(this->limitLeft)}},
		{"limitRight", {"int", &(this->limitRight)}},
		{"dragMarginUp", {"float", &(this->dragMarginUp)}},
		{"dragMarginDown", {"float", &(this->dragMarginDown)}},
		{"dragMarginLeft", {"float", &(this->dragMarginLeft)}},
		{"dragMarginRight", {"float", &(this->dragMarginRight)}},
		{"smoothingEnabled", {"bool", &(this->smoothingEnabled)}},
		{"smoothingSpeed", {"float", &(this->smoothingSpeed)}},
	};
	this->propPointers.insert(props.begin(), props.end());

	this->lastTime = glfwGetTime();
}

std::string Camera2D::leafOperation(){
	Leaf2D::leafOperation();
	double newTime = glfwGetTime();
	float timeDelta = newTime - this->lastTime;
	this->lastTime = newTime;

	if(!cameraReady){
		Leaf* nextLeaf = this->parent;
		while(nextLeaf != nullptr && nextLeaf->getLeafType() != LEAF_TYPE_2D){
			nextLeaf = nextLeaf->parent;
		}
		if(nextLeaf != nullptr){
			this->cameraReady = true;
			this->actualPosition = static_cast<Leaf2D*>(nextLeaf)->globalPosition;
			this->smoothedPosition = static_cast<Leaf2D*>(nextLeaf)->globalPosition;
		}
	}
	else if(this->smoothingEnabled){
		if(abs(this->smoothedPosition.x - this->actualPosition.x) > POSITION_EPSILON || abs(this->smoothedPosition.y - this->actualPosition.y) > POSITION_EPSILON){
			glm::vec2 smoothDelta = (this->actualPosition - this->smoothedPosition)*timeDelta*this->smoothingSpeed;
			this->smoothedPosition += smoothDelta;
		}
	}
	return "";
}

void Camera2D::makeCurrent(){
	Leaf* root = this->getRoot();
	if(root->propPointers.count("current") > 0) static_cast<Camera2D*>(root)->current = false;
	std::vector<Leaf*> leaves = root->getAllChildren();
	for(auto& i: leaves){
		if(i->propPointers.count("current") > 0) static_cast<Camera2D*>(i)->current = false;
	}
	this->current = true;
}