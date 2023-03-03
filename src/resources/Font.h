#pragma once

#include "Settings.h"
#include "resources/Texture.h"

struct Character{
	Texture* texture = nullptr;
	glm::ivec2 size = glm::ivec2(0.f);
	glm::ivec2 bearing = glm::ivec2(0.f);
	unsigned advance = 0u;
};

class Font{
private:
	std::array<Character*, 2u> emptyChars;
public:
	int references = 0;
	std::string path = "";

	FT_Face fontFace = nullptr;
	std::map<std::string, Character*> characters;

	Font();
	~Font();
	void initFace(std::string fontPath, unsigned size);
	void destroyFace();
	void setFontSize(unsigned size);
	Character* getCharacter(std::string character);
	std::vector<Character*> getString(std::string string);
};