#pragma once

#include "servers/FileReader.h"

#include "resources/Texture.h"
#include "resources/Shader.h"
#include "resources/TileSet.h"
#include "resources/Particle.h"
#include "resources/Font.h"

#include "resources/shapes2d/RectangleShape2D.h"
#include "resources/shapes2d/CircleShape2D.h"
#include "resources/shapes2d/PolygonShape2D.h"
#include "resources/shapes2d/LineShape2D.h"

const int TEXTURE_POOL_SIZE = 300;
const int SHADER_POOL_SIZE = 50;
const int TILESET_POOL_SIZE = 50;
const int PARTICLE_POOL_SIZE = 1000;
const int FONT_POOL_SIZE = 20;

const int RECT_SHAPE_POOL_SIZE = 200;
const int CIRC_SHAPE_POOL_SIZE = 100;
const int POLY_SHAPE_POOL_SIZE = 100;
const int LINE_SHAPE_POOL_SIZE = 100;

//server singleton for managing resources
class ResourceManager{
public:
	//singleton method
	static ResourceManager& ins();

	//properties
	std::array<Texture*, TEXTURE_POOL_SIZE> texturePool;
	std::array<Shader*, SHADER_POOL_SIZE> shaderPool;
	std::array<TileSet*, TILESET_POOL_SIZE> tileSetPool;
	std::array<Particle*, PARTICLE_POOL_SIZE> particlePool;
	std::array<Font*, FONT_POOL_SIZE> fontPool;

	std::array<RectangleShape2D*, RECT_SHAPE_POOL_SIZE> rectShapePool;
	std::array<CircleShape2D*, CIRC_SHAPE_POOL_SIZE> circShapePool;
	std::array<PolygonShape2D*, POLY_SHAPE_POOL_SIZE> polyShapePool;
	std::array<LineShape2D*, LINE_SHAPE_POOL_SIZE> lineShapePool;

	//methods
	ResourceManager();
	~ResourceManager();

	Texture* loadTexture(std::string path, unsigned filter = GL_LINEAR);
	Texture* loadTexture(FT_Bitmap bitmap, unsigned filter = GL_LINEAR);
	void unloadTexture(Texture* texture);
	Shader* loadShader(std::string fragmentPath = "shaders/shader2D.fs", std::string vertexPath = "shaders/shader2D.vs", std::string geometryPath = "");
	void unloadShader(Shader* shader);
	TileSet* loadTileSet(int sheetRows, int sheetColumns);
	void unloadTileSet(TileSet* tileSet);
	Particle* loadParticle(Texture* texture, glm::vec2 velocity, glm::vec2 position, glm::vec2 scale = glm::vec2(1.f), float timeLeft = -1.f);
	void unloadParticle(Particle* particle);
	Font* loadFont(std::string path, unsigned size = 18u);
	void unloadFont(Font* font);

	RectangleShape2D* loadRectShape(glm::vec2 size);
	void unloadRectShape(RectangleShape2D* rectShape);
	CircleShape2D* loadCircShape(float radius);
	void unloadCircShape(CircleShape2D* circShape);
	PolygonShape2D* loadPolyShape(std::vector<glm::vec2> points);
	void unloadPolyShape(PolygonShape2D* polyShape);
	LineShape2D* loadLineShape(glm::vec2 point1, glm::vec2 point2);
	void unloadLineShape(LineShape2D* lineShape);
};