#include "resources/Font.h"
#include "servers/ResourceManager.h"

Font::Font(){
	this->emptyChars.at(0) = new Character; //empty char
	this->emptyChars.at(1) = new Character; //new line
	this->emptyChars.at(1)->advance = 5u;
}

Font::~Font(){
	this->destroyFace();
	delete this->emptyChars.at(0);
	delete this->emptyChars.at(1);
}

void Font::initFace(std::string fontPath, unsigned size){
	this->path = fontPath;
	if (FT_New_Face(Settings::ins().ftLibrary, fontPath.c_str(), 0, &(this->fontFace))){
		std::cerr<< "ERROR: freetype: failed to load font: "<<fontPath<<std::endl;
	}
	this->setFontSize(size);
}

void Font::destroyFace(){
	this->path = "";
	for(auto &i: characters){
		ResourceManager::ins().unloadTexture(i.second->texture);
		delete i.second;
	}
	this->characters.clear();
	if(this->fontFace != nullptr){
		FT_Done_Face(this->fontFace);
		this->fontFace = nullptr;
	}
}

void Font::setFontSize(unsigned size){
	if(this->fontFace != nullptr){
		FT_Set_Pixel_Sizes(this->fontFace, 0, size);
	}
}

Character* Font::getCharacter(std::string character){
	if(this->fontFace != nullptr){
		if(this->characters.count(character) > 0){
			return this->characters.at(character);
		}
		FT_ULong characterToLoad = 0;
		if(character.size() > 0){
			characterToLoad = character.at(0);
			if(character.size() > 1 && character.size() < 5){
				for(int i = 1; i<character.size(); i++){
					characterToLoad = characterToLoad<<(i*8) + character.at(i);
				}
			}
		}
		if (FT_Load_Char(this->fontFace, characterToLoad, FT_LOAD_RENDER)){
			std::cerr<<"ERROR: freetype: failed to load letter "<<character<<std::endl;  
		}
		Character* newChar = new Character;
		newChar->size = glm::vec2(this->fontFace->glyph->bitmap.width, this->fontFace->glyph->bitmap.rows);
		newChar->bearing = glm::vec2(this->fontFace->glyph->bitmap_left, this->fontFace->glyph->bitmap_top);
		newChar->advance = this->fontFace->glyph->advance.x;
		newChar->texture = ResourceManager::ins().loadTexture(this->fontFace->glyph->bitmap);
		this->characters.insert({character, newChar});
		return this->characters.at(character);
	}
	return this->emptyChars.at(0);
}

std::vector<Character*> Font::getString(std::string string){
	//> - new line
	if(this->fontFace != nullptr){
		std::vector<Character*> chars;
		std::string curChar;
		for(const auto& i: string){
			if(i > 127){
				curChar.push_back(i);
				if(i < 192){
					Character* newChar;
					newChar = this->getCharacter(curChar);
					chars.push_back(newChar);
					curChar.clear();
				}
			}
			else{
				if(i == '>'){
					chars.push_back(this->emptyChars.at(1));
				}
				else{
					Character* newChar;
					newChar = this->getCharacter(std::string(1, i));
					chars.push_back(newChar);
				}
			}
		}
		return chars;
	}
	return std::vector<Character*>();
}