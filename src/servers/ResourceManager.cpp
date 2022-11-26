#include "servers/ResourceManager.h"

#include <stb_image.h>
#include "servers/FileReader.h"

ResourceManager& ResourceManager::ins(){
	static ResourceManager instance;
	return instance;
}

ResourceManager::ResourceManager(){
	for(auto &i: this->texturePool){
		i = new Texture;
	}
	for(auto &i: this->shaderPool){
		i = new Shader;
	}
	stbi_set_flip_vertically_on_load(true); 
}

ResourceManager::~ResourceManager(){
	for(auto &i: this->texturePool){
		delete i;
	}
	for(auto &i: this->shaderPool){
		delete i;
	}
}

Texture* ResourceManager::loadTexture(std::string path, unsigned filter){
	for(const auto &i: this->texturePool){
		if(i->path == path){
			i->references+=1;
			return i;
		}
	}
	for(const auto &i: this->texturePool){
		if(i->references == 0){
			i->references+=1;
			std::cout<<"loading texture - "+path<<std::endl;

			int nrChannels = 0; 
			unsigned char* data = stbi_load(path.c_str(), &(i->width), &(i->height), &nrChannels, 0);
			if(!data){
				std::cerr<<"ERROR: "+path+": texture not found"<<std::endl;
			}
			else{
				unsigned pixel_format = GL_RGBA;
				if(nrChannels == 3){
					pixel_format = GL_RGB;
				}
				i->path = path;
				i->setTexture(data, filter, pixel_format);
			}
			stbi_image_free(data);
			return i;
		}
	}
	std::cerr<<"ERROR: texturePool: out of space"<<std::endl;
	return this->texturePool.at(0);
}

void ResourceManager::unloadTexture(Texture* texture){
	for(const auto &i: this->texturePool){
		if(i == texture){
			i->references-=1;
			return;
		}
	}
}

Shader* ResourceManager::loadShader(std::string fragmentPath, std::string vertexPath, std::string geometryPath){
	for(const auto &i: this->shaderPool){
		if(i->vertexPath == vertexPath && i->fragmentPath == fragmentPath && i->vertexPath == geometryPath){
			i->references+=1;
			return i;
		}
	}
	for(const auto &i: this->shaderPool){
		if(i->references == 0){
			i->references+=1;
			std::cout<<"loading shaders: "+vertexPath+" "+fragmentPath+" "+geometryPath<<std::endl;

			std::string vertexString = FileReader::ins().loadAsString(vertexPath);
			std::string fragmentString = FileReader::ins().loadAsString(fragmentPath);
			std::string geometryString = FileReader::ins().loadAsString(geometryPath);
			if(vertexString == ""){
				std::cerr<<"ERROR: "+vertexPath+": vertex shader: not found"<<std::endl;
				return i;
			}
			if(fragmentString == ""){
				std::cerr<<"ERROR: "+fragmentPath+": fragment shader: not found"<<std::endl;
				return i;
			}
			i->vertexPath = vertexPath;
			i->fragmentPath = fragmentPath;
			i->geometryPath = geometryPath;
			i->compileShaders(vertexString, fragmentString, geometryString);
			
			return i;
		}
	}
	std::cerr<<"ERROR: shaderPool: out of space"<<std::endl;
	return this->shaderPool.at(0);
}

void ResourceManager::unloadShader(Shader* shader){
	for(const auto &i: this->shaderPool){
		if(i == shader){
			i->references-=1;
			return;
		}
	}
}