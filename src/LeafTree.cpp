#include "LeafTree.h"

#include "servers/ResourceManager.h"
#include "servers/GrassLoader.h"

LeafTree::LeafTree(){
	//initialize the static LeafTree pointer
	if(true){
		Leaf leaf(this);
	}
}

LeafTree::~LeafTree(){
	if(this->grassRoot != nullptr) delete this->grassRoot;
}

void LeafTree::growGrass(std::string grassPath){
	if(this->grassRoot != nullptr) return;
	this->grassPath = Settings::ins().project_dir+grassPath;
	std::cout<<"loading grass: "+this->grassPath<<std::endl;

	std::string grassString = FileReader::ins().loadAsString(this->grassPath);
	if (grassString==""){
		std::cerr<<"ERROR: "+this->grassPath+": grass not found"<<std::endl;
		return;
	}
	this->grassRoot = GrassLoader::ins().loadGrass(grassString);
	if (this->grassRoot == nullptr){
		std::cerr<<"ERROR: incorrect grass file"<<std::endl;
	}
}

void LeafTree::changeGrass(std::string grassPath){
	if(this->grassRoot != nullptr) delete this->grassRoot;
	this->growGrass(this->grassPath);
}

void LeafTree::reloadGrass(){
	this->changeGrass(this->grassPath);
}

std::string LeafTree::doLeafOperations(){
	if(this->grassRoot == nullptr) return "leafTree: no root!";
	std::string error_message = "";

	error_message = grassRoot->leafOperation();
	if(error_message != "") return error_message;
	std::vector<Leaf*> leaves = grassRoot->getAllChildren();
	for (const auto &i: leaves){
		error_message = i->leafOperation();
		if(error_message != "") return error_message;
	}
	return error_message;
}
void LeafTree::doLeafUpdates(double delta){
	if(this->grassRoot == nullptr) return;

	grassRoot->update(delta);
	std::vector<Leaf*> leaves = grassRoot->getAllChildren();
	for (const auto &i: leaves){
		i->update(delta);
	}
}

void LeafTree::doInputEvents(){
	if(this->grassRoot == nullptr) return;

	grassRoot->inputEvent();
	std::vector<Leaf*> leaves = grassRoot->getAllChildren();
	for (const auto &i: leaves){
		i->inputEvent();
	}
}

std::vector<Leaf*> LeafTree::getAllLeaves(){
	std::vector<Leaf*> childLeaves = this->grassRoot->getAllChildren();
	std::vector<Leaf*> allLeaves{this->grassRoot};
	allLeaves.insert(allLeaves.end(), childLeaves.begin(), childLeaves.end());
	return allLeaves;
}