#include "leaves/leaves2D/TileMap.h"
#include "servers/ResourceManager.h"

TileMap::TileMap(Leaf* parent){
	this->name = "TileMap";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"texture", {"texture", &(this->texture)}},
		{"tileset", {"tileset", &(this->tileset)}},
		{"tilePositions", {"tilePositions", &(this->tilePositions)}},
	};
	this->propPointers.insert(props.begin(), props.end());
}

TileMap::~TileMap(){
	ResourceManager::ins().unloadTileSet(this->tileset);
	ResourceManager::ins().unloadTexture(this->texture);
}

std::string TileMap::leafOperation(){
	Leaf2D::leafOperation();
	return "";
}

void TileMap::loadTilePositions(std::string path){
	std::string tilesString = FileReader::ins().loadAsString(path);
	if(tilesString == ""){
		std::cerr<<"ERROR: "+path+": tilePositions: not found"<<std::endl;
		return;
	}

	std::istringstream f(tilesString);
	std::string line;
	while (std::getline(f, line)){
		if(line == "") continue;
		std::istringstream f2(line);
		std::string line2;
		std::vector<int> tilePos;
		while(std::getline(f2, line2, ',')){
			if(line2 == "") continue;
			tilePos.push_back(std::stoi(line2) - 1);
		}
		this->tilePositions.push_back(tilePos);
	}
	std::reverse(this->tilePositions.begin(), this->tilePositions.end());
}