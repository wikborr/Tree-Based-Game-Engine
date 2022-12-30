#include "leaves/leaves2D/Sprite.h"
#include "servers/ResourceManager.h"

Sprite::Sprite(Leaf* parent){
	this->name = "Sprite";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"centered", {"bool", &(this->centered)}},
		{"flipV", {"bool", &(this->flipV)}},
		{"flipH", {"bool", &(this->flipH)}},
		{"sheetFrame", {"unsigned", &(this->sheetFrame)}},
		{"sheetRows", {"unsigned", &(this->sheetRows)}},
		{"sheetColumns", {"unsigned", &(this->sheetColumns)}},
		{"texture", {"texture", &(this->texture)}},
	};
	this->propPointers.insert(props.begin(), props.end());
}

Sprite::~Sprite(){
	ResourceManager::ins().unloadTexture(this->texture);
}

std::string Sprite::leafOperation(){
	Leaf2D::leafOperation();
	return "";
}
