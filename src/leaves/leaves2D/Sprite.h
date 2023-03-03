#pragma once

#include "leaves/Leaf2D.h"
#include "resources/Texture.h"

//leaf for texture rendering
class Sprite: public Leaf2D{
public:
	Sprite(Leaf* parent = nullptr);
	virtual ~Sprite();
	virtual std::string leafOperation() override;

	//properties
	bool centered = true;
	bool flipV = false;
	bool flipH = false;
	
	//for sheet textures
	unsigned sheetFrame = 0;
	unsigned sheetRows = 1;
	unsigned sheetColumns = 1;

	Texture* texture = nullptr;
};