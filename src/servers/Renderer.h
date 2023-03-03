#pragma once

#include "LeafTree.h"
#include "resources/Shader.h"
#include "leaves/leaves2D/Sprite.h"
#include "leaves/leaves2D/ColorRect.h"
#include "leaves/leaves2D/CollisionShape2D.h"
#include "leaves/leaves2D/TileMap.h"
#include "leaves/leaves2D/TextLabel.h"

struct SpriteSheet{
	SpriteSheet(unsigned frame = 0, unsigned rows = 0, unsigned columns = 0, glm::ivec2 size = {0,0})
		: sheetFrame(frame), sheetRows(rows), sheetColumns(columns), sheetSize(size) {}
	unsigned sheetFrame = 0;
	unsigned sheetRows = 1;
	unsigned sheetColumns = 1;
	glm::ivec2 sheetSize = glm::ivec2(1.f);
};

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
	void renderLeaf2dPart2(Leaf2D* renderable, bool multiple = false);
	void finalRender(glm::mat4 finalPerspectiveM, glm::mat4 finalViewM, Leaf2D* multipleLeaf = nullptr);
	void renderSprites(const std::vector<Sprite*>& sprites);
	void changeTexCoords(SpriteSheet sprite, SpriteSheet prev_sprite = SpriteSheet());
	void resetTexCoords();
	void renderColorRects(const std::vector<ColorRect*>& colorRects);
	void renderTileMaps(const std::vector<TileMap*>& tilemaps);
	void renderTextLabels(const std::vector<TextLabel*>& textLabels);
	void renderCollisionShapes(const std::vector<CollisionShape2D*>& collisionShapes);
public:
	//singleton method
	static Renderer& ins();

	//properties
	unsigned squareVAO = 0;
	unsigned texVBO2 = 0;
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