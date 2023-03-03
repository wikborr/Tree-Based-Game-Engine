#include "servers/ResourceManager.h"

ResourceManager& ResourceManager::ins(){
	static ResourceManager instance;
	return instance;
}

ResourceManager::ResourceManager(){
	stbi_set_flip_vertically_on_load(true); 
	for(auto &i: this->texturePool){
		i = new Texture;
	}
	for(auto &i: this->shaderPool){
		i = new Shader;
	}
	for(auto &i: this->tileSetPool){
		i = new TileSet;
	}
	for(auto &i: this->particlePool){
		i = new Particle;
	}
	for(auto &i: this->fontPool){
		i = new Font;
	}
	
	for(auto &i: this->rectShapePool){
		i = new RectangleShape2D;
	}
	for(auto &i: this->circShapePool){
		i = new CircleShape2D;
	}
	for(auto &i: this->polyShapePool){
		i = new PolygonShape2D;
	}
	for(auto &i: this->lineShapePool){
		i = new LineShape2D;
	}
}

ResourceManager::~ResourceManager(){
	for(auto &i: this->texturePool){
		delete i;
	}
	for(auto &i: this->shaderPool){
		delete i;
	}
	for(auto &i: this->tileSetPool){
		delete i;
	}
	for(auto &i: this->particlePool){
		delete i;
	}
	for(auto &i: this->fontPool){
		delete i;
	}

	for(auto &i: this->rectShapePool){
		delete i;
	}
	for(auto &i: this->circShapePool){
		delete i;
	}
	for(auto &i: this->polyShapePool){
		delete i;
	}
	for(auto &i: this->lineShapePool){
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
Texture* ResourceManager::loadTexture(FT_Bitmap bitmap, unsigned filter){
	for(const auto &i: this->texturePool){
		if(i->path == std::to_string(reinterpret_cast<long long>(bitmap.buffer))+"_freetype"){
			i->references+=1;
			return i;
		}
	}
	for(const auto &i: this->texturePool){
		if(i->references == 0){
			i->references+=1;
			unsigned pixel_format = GL_RED;
			i->path = std::to_string(reinterpret_cast<long long>(bitmap.buffer))+"_freetype";
			i->setTexture(bitmap.buffer, filter, pixel_format, pixel_format);
			i->width = bitmap.width;
			i->height = bitmap.rows;
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
			i->vertexPath = vertexPath;
			i->fragmentPath = fragmentPath;
			i->geometryPath = geometryPath;
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

TileSet* ResourceManager::loadTileSet(int sheetRows, int sheetColumns){
	for(const auto &i: this->tileSetPool){
		if(i->sheetRows == sheetRows && i->sheetColumns == sheetColumns){
			i->references+=1;
			return i;
		}
	}
	for(const auto &i: this->tileSetPool){
		if(i->references == 0){
			i->references+=1;
			i->sheetRows = sheetRows;
			i->sheetColumns = sheetColumns;
			return i;
		}
	}
	std::cerr<<"ERROR: tileSetPool: out of space"<<std::endl;
	return this->tileSetPool.at(0);
}
void ResourceManager::unloadTileSet(TileSet* tileSet){
	for(const auto &i: this->tileSetPool){
		if(i == tileSet){
			i->references-=1;
			return;
		}
	}
}

Particle* ResourceManager::loadParticle(Texture* texture, glm::vec2 velocity, glm::vec2 position, glm::vec2 scale, float timeLeft){
	for(const auto &i: this->particlePool){
		if(i->timeLeft == timeLeft && i->velocity == velocity && i->spriteLeaf->texture == texture && i->spriteLeaf->globalPosition == position){
			i->references+=1;
			return i;
		}
	}
	for(const auto &i: this->particlePool){
		if(i->references == 0){
			i->references+=1;
			i->timeLeft = timeLeft;
			i->velocity = velocity;
			i->spriteLeaf->texture = texture;
			i->spriteLeaf->position = position;
			i->spriteLeaf->selfModulate = glm::vec4(1.f, 1.f, 1.f, 0.5f);
			i->spriteLeaf->scale = scale;
			i->spriteLeaf->Leaf2D::leafOperation();
			return i;
		}
	}
	std::cerr<<"ERROR: particlePool: out of space"<<std::endl;
	return this->particlePool.at(0);
}
void ResourceManager::unloadParticle(Particle* particle){
	for(const auto &i: this->particlePool){
		if(i == particle){
			if(i->spriteLeaf->parent !=nullptr) i->spriteLeaf->parent->removeChildFromTree(i->spriteLeaf);
			i->references-=1;
			return;
		}
	}
}

Font* ResourceManager::loadFont(std::string path, unsigned size){
	for(const auto &i: this->fontPool){
		if(i->path == path){
			i->references+=1;
			return i;
		}
	}
	for(const auto &i: this->fontPool){
		if(i->references == 0){
			i->references+=1;
			i->initFace(path, size);
			return i;
		}
	}
	std::cerr<<"ERROR: fontPool: out of space"<<std::endl;
	return this->fontPool.at(0);
}
void ResourceManager::unloadFont(Font* font){
	for(const auto &i: this->fontPool){
		if(i == font){
			i->references-=1;
			return;
		}
	}
}

RectangleShape2D* ResourceManager::loadRectShape(glm::vec2 size){
	for(const auto &i: this->rectShapePool){
		if(i->size == size){
			i->references+=1;
			return i;
		}
	}
	for(const auto &i: this->rectShapePool){
		if(i->references == 0){
			i->references+=1;
			i->size = size;
			return i;
		}
	}
	std::cerr<<"ERROR: rectShapePool: out of space"<<std::endl;
	return this->rectShapePool.at(0);
}
CircleShape2D* ResourceManager::loadCircShape(float radius){
	for(const auto &i: this->circShapePool){
		if(i->radius == radius){
			i->references+=1;
			return i;
		}
	}
	for(const auto &i: this->circShapePool){
		if(i->references == 0){
			i->references+=1;
			i->radius = radius;
			return i;
		}
	}
	std::cerr<<"ERROR: circShapePool: out of space"<<std::endl;
	return this->circShapePool.at(0);
}
PolygonShape2D* ResourceManager::loadPolyShape(std::vector<glm::vec2> points){
	for(const auto &i: this->polyShapePool){
		if(i->points == points){
			i->references+=1;
			return i;
		}
	}
	for(const auto &i: this->polyShapePool){
		if(i->references == 0){
			i->references+=1;
			i->points = points;
			return i;
		}
	}
	std::cerr<<"ERROR: polyShapePool: out of space"<<std::endl;
	return this->polyShapePool.at(0);
}

LineShape2D* ResourceManager::loadLineShape(glm::vec2 point1, glm::vec2 point2){
	for(const auto &i: this->lineShapePool){
		if(i->linePoints == std::pair<glm::vec2, glm::vec2>(point1, point2)){
			i->references+=1;
			return i;
		}
	}
	for(const auto &i: this->lineShapePool){
		if(i->references == 0){
			i->references+=1;
			i->linePoints = std::pair<glm::vec2, glm::vec2>(point1, point2);
			return i;
		}
	}
	std::cerr<<"ERROR: lineShapePool: out of space"<<std::endl;
	return this->lineShapePool.at(0);
}
void ResourceManager::unloadRectShape(RectangleShape2D* rectShape){
	for(const auto &i: this->rectShapePool){
		if(i == rectShape){
			i->references-=1;
			return;
		}
	}
}
void ResourceManager::unloadCircShape(CircleShape2D* circShape){
	for(const auto &i: this->circShapePool){
		if(i == circShape){
			i->references-=1;
			return;
		}
	}
}
void ResourceManager::unloadPolyShape(PolygonShape2D* polyShape){
	for(const auto &i: this->polyShapePool){
		if(i == polyShape){
			i->references-=1;
			return;
		}
	}
}
void ResourceManager::unloadLineShape(LineShape2D* lineShape){
	for(const auto &i: this->lineShapePool){
		if(i == lineShape){
			i->references-=1;
			return;
		}
	}
}