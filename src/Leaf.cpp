#include "Leaf.h"
#include "LeafTree.h"

LeafTree* Leaf::leafTree = nullptr;

void Leaf::inputEvent(){}
void Leaf::ready() {}
void Leaf::update(double delta) {}

std::string Leaf::leafOperation() {
	return "";
}

LeafType Leaf::getLeafType(){
	return LEAF_TYPE_NORMAL;
}

Leaf::Leaf(LeafTree* leafTree){
	this->leafTree = leafTree;
}

Leaf::Leaf(Leaf* parent){
	this->name = "Leaf";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"name", {"string", &(this->name)}},
		{"is_paused", {"bool", &(this->is_paused)}},
		{"pause_mode", {"PauseMode", &(this->pause_mode)}},
	};
	this->propPointers.insert(props.begin(), props.end());
}

Leaf::~Leaf(){
	if(!(this->children.empty())){
		for(auto i: this->children){
			delete i;
		}
	}
}

Leaf* Leaf::getChild(std::string name){
	if(!(this->children.empty())){
		for(const auto &i: this->children){
			if(i->name == name){
				return i;
			}
		}
	}
	return nullptr;
}

std::vector<Leaf*> Leaf::getAllChildren(){
	if(this->children.empty()){
		return std::vector<Leaf*>();
	}
	std::vector<Leaf*> descendants;
	for(const auto &i: this->children){
		descendants.push_back(i);
		std::vector<Leaf*> i_vect = i->getAllChildren();
		descendants.insert(descendants.end(), i_vect.begin(), i_vect.end());
	}
	return descendants;
}

