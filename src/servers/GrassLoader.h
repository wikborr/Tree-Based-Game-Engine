#pragma once

#include "Leaf.h"
#include <map>
#include <functional>

//server singleton for converting grass to a leaf tree
class GrassLoader{
private:
	void propertyIfBlock(Leaf* leaf, std::string propName, std::string propValue);
public:
	//singleton method
	static GrassLoader& ins();

	std::map<std::string, std::function<Leaf*()>> leafPointers;

	GrassLoader();
	Leaf* loadGrass(std::string grassString);
};