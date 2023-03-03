#pragma once

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
	Leaf* getParent();
	std::vector<Leaf*> getAllChildren();
	Leaf* getRoot();
	Leaf* removeChildFromTree(std::string name);
	Leaf* removeChildFromTree(Leaf* leaf);

	template <typename T>
	T* getChild(std::string name){
		return static_cast<T*>(getChild(name));
	}
	template <typename T>
	T* getParent(){
		return static_cast<T*>(this->parent);
	}
	template <typename T>
	T* addChild(std::string name){
		Leaf* newLeaf = new T;
		newLeaf->parent = this;
		this->children.push_back(newLeaf);
		newLeaf->name = name;
		return static_cast<T*>(newLeaf);
	}
	template <typename T>
	T* addChild(T* leaf){
		Leaf* newLeaf = static_cast<Leaf*>(leaf);
		newLeaf->parent = this;
		this->children.push_back(newLeaf);
		return static_cast<T*>(newLeaf);
	}
};