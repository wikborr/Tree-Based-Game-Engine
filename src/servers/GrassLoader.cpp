#include "servers/GrassLoader.h"
#include "servers/ResourceManager.h"

#include "Leaf.h"
#include "leaves/Leaf2D.h"
#include "leaves/leaves2D/Sprite.h"
#include "leaves/Timer.h"
#include "leaves/Tween.h"
#include "leaves/UILayer.h"
#include "leaves/BGLayer.h"
#include "leaves/leaves2D/ColorRect.h"
#include "leaves/leaves2D/Camera2D.h"
#include "leaves/leaves2D/CollisionShape2D.h"
#include "leaves/leaves2D/CollisionObject2D.h"
#include "leaves/leaves2D/Area2D.h"
#include "leaves/leaves2D/StaticBody2D.h"
#include "leaves/leaves2D/KinematicBody2D.h"
#include "leaves/leaves2D/TileMap.h"
#include "leaves/leaves2D/ParticleEmitter.h"
#include "leaves/leaves2D/TextLabel.h"
#include "leaves/leaves2D/Model3D.h"
#include "leaves/AudioPlayer.h"

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
		{"Timer", []()->Leaf*{return new Timer;}},
		{"Tween", []()->Leaf*{return new Tween;}},
		{"UILayer", []()->Leaf*{return new UILayer;}},
		{"BGLayer", []()->Leaf*{return new BGLayer;}},
		{"ColorRect", []()->Leaf*{return new ColorRect;}},
		{"Camera2D", []()->Leaf*{return new Camera2D;}},
		{"CollisionShape2D", []()->Leaf*{return new CollisionShape2D;}},
		{"Area2D", []()->Leaf*{return new Area2D;}},
		{"StaticBody2D", []()->Leaf*{return new StaticBody2D;}},
		{"KinematicBody2D", []()->Leaf*{return new KinematicBody2D;}},
		{"TileMap", []()->Leaf*{return new TileMap;}},
		{"ParticleEmitter", []()->Leaf*{return new ParticleEmitter;}},
		{"TextLabel", []()->Leaf*{return new TextLabel;}},
		{"Model3D", []()->Leaf*{return new Model3D;}},
		{"AudioPlayer", []()->Leaf*{return new AudioPlayer;}},
	};

	auto projectLeaves =  LEAVES::mapLeaves();
	this->leafPointers.insert(projectLeaves.begin(), projectLeaves.end());
}

void GrassLoader::propertyIfBlock(Leaf* leaf, std::string propName, std::string propValue){
	if(leaf->propPointers.count(propName) == 0){
		std::cerr<<"ERROR: incorrect property name in the grass file"<<std::endl;
		return;
	}
	std::string propType = leaf->propPointers.at(propName).first;
	void* propPointer = leaf->propPointers.at(propName).second;

	if(propType == "int") std::istringstream(propValue) >> *static_cast<int*>(propPointer);
	else if(propType == "string") *static_cast<std::string*>(propPointer) = propValue;
	else if(propType == "bool") std::istringstream(propValue) >> *static_cast<bool*>(propPointer);
	else if(propType == "unsigned") std::istringstream(propValue) >> *static_cast<unsigned*>(propPointer);
	else if(propType == "float") std::istringstream(propValue) >> *static_cast<float*>(propPointer);
	else if(propType == "double") std::istringstream(propValue) >> *static_cast<double*>(propPointer);
	else if(propType == "vec2"){
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
	else if(propType == "tileset"){
		int sheetRows = 0, sheetColumns = 0;
		std::istringstream(propValue) >> sheetRows >> sheetColumns;
		*static_cast<TileSet**>(propPointer) = ResourceManager::ins().loadTileSet(sheetRows, sheetColumns);
	}
	else if(propType == "tilePositions") static_cast<TileMap*>(leaf)->loadTilePositions(Settings::ins().project_dir+propValue);
	else if(propType == "font") *static_cast<Font**>(propPointer) = ResourceManager::ins().loadFont(Settings::ins().project_dir+propValue);
	else if(propType == "rectangleShape2D"){
		glm::vec2 size;
		std::istringstream(propValue) >> size.x >> size.y;
		*static_cast<RectangleShape2D**>(propPointer) = ResourceManager::ins().loadRectShape(size);
	}
	else if(propType == "circleShape2D"){
		float radius;
		std::istringstream(propValue) >> radius;
		*static_cast<CircleShape2D**>(propPointer) = ResourceManager::ins().loadCircShape(radius);
	}
	else if(propType == "polygonShape2D"){
		std::vector<glm::vec2> points;
		std::istringstream stream(propValue);
		while(stream.rdbuf()->in_avail() > 0){
			glm::vec2 point;
			stream >> point.x >> point.y;
			points.push_back(point);
		}
		*static_cast<PolygonShape2D**>(propPointer) = ResourceManager::ins().loadPolyShape(points);
	}
	else if(propType == "lineShape2D"){
		glm::vec2 point1, point2;
		std::istringstream(propValue) >> point1.x >> point1.y >> point2.x >> point2.y;
		*static_cast<LineShape2D**>(propPointer) = ResourceManager::ins().loadLineShape(point1, point2);
	}
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
				std::string subGrassPath = Settings::ins().project_dir+line.substr(0, colonIndex);
				std::string subGrassString = FileReader::ins().loadAsString(subGrassPath);
				if(subGrassString == ""){
					std::cerr<<"ERROR: incorrect subGrass path in the grass file"<<std::endl;
				}
				else{
					Leaf* subGrass = this->loadGrass(subGrassString);
					if(subGrass == nullptr){
						std::cerr<<"ERROR: incorrect subGrass file"<<std::endl;
					}
					else{
						if(leafStack.size() > 0){
							subGrass->parent = leafStack.top();
							leafStack.top()->children.push_back(subGrass);
						}
						subGrass->name = line.substr(colonIndex+2);
						leafStack.push(subGrass);
					}
				}
			}
			else{
				if(line.size() > 1 && line.substr(0,2) == "- "){
					std::cerr<<"ERROR: incorrect indentation in the grass file"<<std::endl;
				}
				colonIndex = line.find(": ");
				if(this->leafPointers.count(line.substr(0, colonIndex)) == 0){
					std::cerr<<"ERROR: leaf called "<<line.substr(0, colonIndex)<<" doesn't exist"<<std::endl;
					return nullptr;
				}
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