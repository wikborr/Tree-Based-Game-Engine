#pragma once

#include <array>
#include <string>
#include "resources/Texture.h"
#include "resources/Shader.h"

const int TEXTURE_POOL_SIZE = 200;
const int SHADER_POOL_SIZE = 50;

//server singleton for managing resources
class ResourceManager{
public:
	//singleton method
	static ResourceManager& ins();

	//properties
	std::array<Texture*, TEXTURE_POOL_SIZE> texturePool;
	std::array<Shader*, SHADER_POOL_SIZE> shaderPool;

	//methods
	ResourceManager();
	~ResourceManager();

	Texture* loadTexture(std::string path, unsigned filter = GL_LINEAR);
	void unloadTexture(Texture* texture);
	Shader* loadShader(std::string fragmentPath = "shaders/shader2D.fs", std::string vertexPath = "shaders/shader2D.vs", std::string geometryPath = "");
	void unloadShader(Shader* shader);
	
};