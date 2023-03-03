#include "leaves/leaves2D/Model3D.h"
#include "servers/ResourceManager.h"

Model3D::Model3D(Leaf* parent){
	this->name = "Model3D";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"position3d", {"vec3", &(this->position3d)}},
		{"scale3d", {"vec3", &(this->scale3d)}},
		{"rotationDegrees3d", {"vec3", &(this->rotationDegrees3d)}},
	};
	this->propPointers.insert(props.begin(), props.end());
}

Model3D::~Model3D(){

}

std::string Model3D::leafOperation(){
	Leaf2D::leafOperation();
	return "";
}