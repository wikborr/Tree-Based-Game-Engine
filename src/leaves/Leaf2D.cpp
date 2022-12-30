#include "leaves/Leaf2D.h"
#include "servers/ResourceManager.h"
#include "leaves/UILayer.h"
#include "leaves/BGLayer.h"
#include <stack>

LeafType Leaf2D::getLeafType(){
	return LEAF_TYPE_2D;
}

Leaf2D::Leaf2D(Leaf* parent){
	this->name = "Leaf2D";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"position", {"vec2", &(this->position)}},
		{"rotationDegrees", {"float", &(this->rotationDegrees)}},
		{"scale", {"vec2", &(this->scale)}},
		{"transform", {"mat4", &(this->transform)}},
		{"zIndex", {"unsigned", &(this->zIndex)}},
		{"visible", {"bool", &(this->visible)}},
		{"modulate", {"vec4", &(this->modulate)}},
		{"selfModulate", {"vec4", &(this->selfModulate)}},
		{"leafShader", {"shader", &(this->leafShader)}},
	};
	this->propPointers.insert(props.begin(), props.end());
}

Leaf2D::~Leaf2D(){
	ResourceManager::ins().unloadShader(this->leafShader);
}

std::string Leaf2D::leafOperation(){
	Leaf::leafOperation();
	Leaf* nextLeaf = this->parent;
	while(nextLeaf != nullptr && nextLeaf->getLeafType() != LEAF_TYPE_2D){
		nextLeaf = nextLeaf->parent;
	}
	glm::mat4 transform = glm::mat4(1.0f);
	if(nextLeaf != nullptr){
		transform = static_cast<Leaf2D*>(nextLeaf)->globalTransform;
		this->modulate = static_cast<Leaf2D*>(nextLeaf)->modulate;
	}
	this->globalPosition = transform * glm::vec4(this->position, 0.0f, 1.0f);

	transform = glm::translate(transform, glm::vec3(this->position, 0.0f));
	transform = glm::rotate(transform, glm::radians(this->rotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, glm::vec3(this->scale, 1.0));
	this->globalTransform = transform * this->transform;
	
	return "";
}
