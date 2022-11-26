#pragma once

#include "Settings.h"
#include "Leaf.h"

//structure containing leaves
class LeafTree{
public:
	//attributes
	std::string grassPath;
	Leaf* grassRoot = nullptr;

	//methods
	LeafTree();
	~LeafTree();
	void growGrass(std::string grassPath);
	void changeGrass(std::string grassPath);
	void reloadGrass();

	std::string doLeafOperations();
	void doLeafUpdates(double delta = 0.0);
	void doInputEvents();

	std::vector<Leaf*> getAllLeaves();
};