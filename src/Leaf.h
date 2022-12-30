#pragma once

#include <map>
#include "Settings.h"
#include "servers/Input.h"
class LeafTree;

enum PauseMode{
	PAUSE_MODE_INHERIT,
	PAUSE_MODE_STOP,
	PAUSE_MODE_PROCESS
};

enum LeafType{
	LEAF_TYPE_NORMAL,
	LEAF_TYPE_2D
};

//base class for all game objects
class Leaf{
public:
	//attributess
	static LeafTree* leafTree;
	Leaf* parent = nullptr;
	std::vector<Leaf*> children;

	std::string name = "Leaf";
	std::map<std::string, std::pair<std::string, void*>> propPointers;

	bool is_paused = false; //for halting leaf's physics and input
	PauseMode pause_mode = PAUSE_MODE_INHERIT; //what to do if the parent has been paused

	//virtual methods
	virtual void inputEvent(); //called when there's been an input event
	virtual void ready(); //called one time after entering the LeafTree
	virtual void update(double delta); //called every frame for logic operations
	
	virtual std::string leafOperation(); //called every frame for Leaf-specific operations

	virtual LeafType getLeafType(); //checks the type of the leaf
	
	//methods
	Leaf(LeafTree* leafTree);
	Leaf(Leaf* parent = nullptr);
	virtual ~Leaf();
	Leaf* getChild(std::string name);
	std::vector<Leaf*> getAllChildren();
	Leaf* getRoot();

	template <typename T>
	T* getChild(std::string name){
		return static_cast<T*>(getChild(name));
	}
	template <typename T>
	T* getParent(){
		return static_cast<T*>(this->parent);
	}
};