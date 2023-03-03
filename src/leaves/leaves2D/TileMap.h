#pragma once

#include "leaves/Leaf2D.h"
#include "resources/TileSet.h"
#include "resources/Texture.h"

//leaf for placing many of the same objects
class TileMap: public Leaf2D{
public:
	TileMap(Leaf* parent = nullptr);
	virtual ~TileMap();
	virtual std::string leafOperation() override;

	Texture* texture = nullptr;
	TileSet* tileset = nullptr;
	std::vector<std::vector<int>> tilePositions;

	void loadTilePositions(std::string path);
};