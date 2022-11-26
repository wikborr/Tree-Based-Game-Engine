#include "servers/GrassLoader.h"

#include "Leaf.h"
#include "leaves/Leaf2D.h"
#include "leaves/leaves2D/Sprite.h"
#include "servers/ResourceManager.h"

#include <sstream>
#include <stack>
#include "LEAVES.H" //every custom leaf from the project

GrassLoader& GrassLoader::ins(){
	static GrassLoader instance;
	return instance;
}

GrassLoader::GrassLoader(){
	this->leafPointers = {
		{"Leaf", []()->Leaf*{return new Leaf;}},
		{"Leaf2D", []()->Leaf*{return new Leaf2D;}},
		{"Sprite", []()->Leaf*{return new Sprite;}},
	};

	auto projectLeaves =  LEAVES::mapLeaves();
	this->leafPointers.insert(projectLeaves.begin(), projectLeaves.end());
}

void GrassLoader::propertyIfBlock(Leaf* leaf, std::string propName, std::string propValue){
	if(leaf->propPointers.count(propName) == 0){
		std::cerr<<"ERROR: incorrect property name in the grass file";
		return;
	}
	std::string propType = leaf->propPointers.at(propName).first;
	void* propPointer = leaf->propPointers.at(propName).second;

	if(propType == "int") std::istringstream(propValue) >> *static_cast<int*>(propPointer);
	else if(propType == "string") std::istringstream(propValue) >> *static_cast<std::string*>(propPointer);
	else if(propType == "bool") std::istringstream(propValue) >> *static_cast<bool*>(propPointer);
	else if(propType == "unsigned") std::istringstream(propValue) >> *static_cast<unsigned*>(propPointer);
	else if(propType == "float") std::istringstream(propValue) >> *static_cast<float*>(propPointer);
	else if(propType == "double") std::istringstream(propValue) >> *static_cast<double*>(propPointer);
	else if(propType == "vec2"){
		std::cout<<propValue<<std::endl;
		glm::vec2& vec = *static_cast<glm::vec2*>(propPointer);
		std::istringstream(propValue) >> vec.x >> vec.y;
	}
	else if(propType == "vec3"){
		glm::vec3& vec = *static_cast<glm::vec3*>(propPointer);
		std::istringstream(propValue) >> vec.x >> vec.y >> vec.z;
	}
	else if(propType == "vec4"){
		glm::vec4& vec = *static_cast<glm::vec4*>(propPointer);
		std::istringstream(propValue) >> vec.x >> vec.y >> vec.z >> vec.w;
	}
	else if(propType == "PauseMode"){
		if(propValue == "0") *static_cast<PauseMode*>(propPointer) = PAUSE_MODE_INHERIT;
		else if(propValue == "1") *static_cast<PauseMode*>(propPointer) = PAUSE_MODE_STOP;
		else if(propValue == "2") *static_cast<PauseMode*>(propPointer) = PAUSE_MODE_PROCESS;
	}
	else if(propType == "shader") *static_cast<Shader**>(propPointer) = ResourceManager::ins().loadShader(Settings::ins().project_dir+propValue);
	else if(propType == "texture") *static_cast<Texture**>(propPointer) = ResourceManager::ins().loadTexture(Settings::ins().project_dir+propValue);
}

Leaf* GrassLoader::loadGrass(std::string grassString){
	std::stack<Leaf*> leafStack;

	std::istringstream f(grassString);
	std::string line;
	while (std::getline(f, line)){
		size_t colonIndex = line.find(": ");
		if(line == "" || line.at(0)=='#' || colonIndex == std::string::npos) continue; //comments

		size_t tabCount = 0;
		while(line.at(tabCount) == '\t') tabCount+=1;
		line = line.substr(tabCount);
		
		if(tabCount < leafStack.size()){
			if(line.substr(0,2) == "- "){
				line = line.substr(2);
				colonIndex = line.find(": ");
				this->propertyIfBlock(leafStack.top(), line.substr(0, colonIndex), line.substr(colonIndex+2));
			}
			else{
				while(leafStack.size()>1 && tabCount < leafStack.size()){
					leafStack.top()->ready();
					leafStack.pop();
				}
			}
		}
		if(tabCount == leafStack.size()){
			if(line.substr(0,2) == "> "){
				line = line.substr(2);
				colonIndex = line.find(": ");
				std::string subGrassString = FileReader::ins().loadAsString(line.substr(0, colonIndex));
				if(subGrassString == ""){
					std::cerr<<"ERROR: incorrect subGrass path in the grass file";
				}
				else{
					Leaf* subGrass = this->loadGrass(subGrassString);
					if(subGrass == nullptr){
						std::cerr<<"ERROR: incorrect subGrass file"<<std::endl;
					}
					else{
						subGrass->name = line.substr(colonIndex+2);
					}
				}
			}
			else{
				colonIndex = line.find(": ");
				Leaf* newLeaf = this->leafPointers.at(line.substr(0, colonIndex))();
				if(leafStack.size() > 0){
					newLeaf->parent = leafStack.top();
					leafStack.top()->children.push_back(newLeaf);
				}
				newLeaf->name = line.substr(colonIndex+2);
				leafStack.push(newLeaf);
			}
		}
	}
	while(leafStack.size() > 1){
		leafStack.top()->ready();
		leafStack.pop();
	}
	leafStack.top()->ready();
	return leafStack.top();
}