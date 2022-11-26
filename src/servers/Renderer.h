#pragma once

#include "LeafTree.h"
#include "resources/Shader.h"
#include "leaves/leaves2D/Sprite.h"

//server singleton for OpenGL rendering
class Renderer{
private:
	void changeTexCoords(Sprite* sprite, Sprite* prev_sprite = nullptr);
	template <typename T>
	std::vector<std::vector<T>> vector2ZIndices(const std::vector<T>& tVector);
public:
	//singleton method
	static Renderer& ins();

	//properties
	unsigned squareVAO = 0;
	Shader* defaultShader = nullptr;

	LeafTree* leafTree = nullptr;

	//methods
	Renderer();
	~Renderer();

	std::string init(LeafTree* leafTree);
	std::string init2D();
	std::string render();
	std::string render2D();

};