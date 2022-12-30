#pragma once

#include "LeafTree.h"
#include "resources/Shader.h"
#include "leaves/leaves2D/Sprite.h"
#include "leaves/leaves2D/ColorRect.h"

//server singleton for OpenGL rendering
class Renderer{
private:
	glm::mat4 curTransform = glm::mat4(1.f);
	glm::mat4 viewM = glm::mat4(1.f);
	glm::mat4 perspectiveM = glm::mat4(1.f);
	unsigned curShaderID = 0;

	glm::vec2 camPos = glm::vec2(0.f);
	glm::vec2 screenSize = glm::vec2(0.f);

	std::vector<std::pair<std::string, Leaf2D*>> vector2ZIndices(const std::vector<std::pair<std::string, Leaf2D*>>& tVector);
	std::vector<std::pair<std::string, Leaf2D*>> vector2layers(const std::vector<std::pair<std::string, Leaf2D*>>& tVector);
	void renderLeaf2dPart1(Leaf2D* renderable, Leaf2D* prevRenderable = nullptr, glm::vec4 additionalColor = {1.f,1.f,1.f,1.f});
	void renderLeaf2dPart2(Leaf2D* renderable);
	void renderSprites(const std::vector<Sprite*>& sprites);
	void changeTexCoords(Sprite* sprite, Sprite* prev_sprite = nullptr);
	void renderColorRects(const std::vector<ColorRect*>& colorRects);
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