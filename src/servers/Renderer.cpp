#include "servers/Renderer.h"

#include "Settings.h"
#include "servers/ResourceManager.h"
#include "leaves/leaves2D/Sprite.h"
#include <algorithm>

Renderer& Renderer::ins(){
	static Renderer instance;
	return instance;
}

Renderer::Renderer(){
	glGenVertexArrays(1, &this->squareVAO);
}
Renderer::~Renderer(){
	glDeleteVertexArrays(1, &this->squareVAO);
}

std::string Renderer::init(LeafTree* leafTree){
	this->leafTree = leafTree;
	return this->init2D();
}
std::string Renderer::init2D(){
	//black screen for initializing the engine
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(Settings::ins().main_window);

	//vertex buffer object data
	float positions[] = { 
        -0.5f, -0.5f,
        0.5f, -0.5f,
		-0.5f, 0.5f,
        0.5f, 0.5f,
    };
	float texCoords[] = { 
        0.0f, 0.0f,
        1.0f, 0.0f,
		0.0f, 1.0f,
        1.0f, 1.0f, 
    };

	//binding the VAO
	glBindVertexArray(this->squareVAO);

	//creating the vertex array objects and attributes
	unsigned posVBO = 0, texVBO = 0;
	glGenBuffers(1, &posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

	glGenBuffers(1, &texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	
	//unbinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	//creating the default shader
	this->defaultShader = ResourceManager::ins().loadShader();

	return "";
}

std::string Renderer::render(){
	return this->render2D();
}
std::string Renderer::render2D(){
	//clearing the buffer
	glClearColor(Settings::ins().bg_color.r, Settings::ins().bg_color.g, Settings::ins().bg_color.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//setting default OpenGL context
	glBindVertexArray(this->squareVAO);
	glUseProgram(this->defaultShader->ID);
	glActiveTexture(GL_TEXTURE0);

	//getting all renderable 2d leaves
	std::vector<Sprite*> sprites;

	std::vector<Leaf*> leaves = this->leafTree->getAllLeaves();
	for(const auto &i: leaves){
		if(i->propPointers.count("texture") > 0) sprites.push_back(static_cast<Sprite*>(i));
	}

	//camera
	float defaultCameraX = 0.0f - static_cast<float>(Settings::ins().game_width)/2;
	float defaultCameraY = 0.0f - static_cast<float>(Settings::ins().game_height)/2;

	//view matrix
	glm::mat4 viewM = glm::lookAt(glm::vec3(defaultCameraX, defaultCameraY, 1.0f), glm::vec3(defaultCameraX, defaultCameraY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	//perspective matrix
	glm::mat4 perspectiveM = glm::ortho(0.0f, static_cast<float>(Settings::ins().game_width), 0.0f, static_cast<float>(Settings::ins().game_height), 0.0f, 2.0f);

	//sprite rendering
	//remove invisible (!visible or not on the screen)
	sprites.erase(std::remove_if(sprites.begin(), sprites.end(), [](Sprite* const& sprite){return !(sprite->visible);}), sprites.end());

	std::vector<std::vector<Sprite*>> zSprites = this->vector2ZIndices<Sprite*>(sprites);
	for(const auto& i: zSprites){
		for(int j = 0; j<i.size(); j++){
			//calculating part 1
			glm::vec4 spriteColor = i[j]->modulate * i[j]->selfModulate;
			unsigned shaderID = this->defaultShader->ID;
			if(i[j]->leafShader != nullptr) shaderID = i[j]->leafShader->ID;
			
			//checking if OpenGL context needs to be changed
			if(j==0){
				if(i[j]->texture != nullptr) glBindTexture(GL_TEXTURE_2D, i[j]->texture->ID);
				else glBindTexture(GL_TEXTURE_2D, 0);

				this->changeTexCoords(i[j]);

				if(shaderID != this->defaultShader->ID) glUseProgram(shaderID);

				glUniform4f(glGetUniformLocation(shaderID, "color"), spriteColor.r, spriteColor.g, spriteColor.b, spriteColor.a);
			}
			else{
				if(i[j]->texture != i[j-1]->texture){
					if(i[j]->texture != nullptr) glBindTexture(GL_TEXTURE_2D, i[j]->texture->ID);
					else glBindTexture(GL_TEXTURE_2D, 0);
				}

				this->changeTexCoords(i[j]);

				if(i[j-1]->leafShader != nullptr){
					if(shaderID != i[j-1]->leafShader->ID) glUseProgram(shaderID);
				}
				else if(shaderID != this->defaultShader->ID) glUseProgram(shaderID);

				if(spriteColor != i[j-1]->modulate * i[j-1]->selfModulate) glUniform4f(glGetUniformLocation(shaderID, "color"), spriteColor.r, spriteColor.g, spriteColor.b, spriteColor.a);
			}
			
			//calculating part 2 and drawing
			glm::mat4 transform = i[j]->globalTransform;
			if(!i[j]->centered && i[j]->texture != nullptr){
				transform = glm::translate(transform, glm::vec3(static_cast<float>(i[j]->texture->width/2), static_cast<float>(i[j]->texture->height/2), 0.0f));
			}
			if(i[j]->flipH){
				transform = glm::scale(transform, glm::vec3(-1.0f, 1.0f, 1.0f));
			}
			if(i[j]->flipV){
				transform = glm::scale(transform, glm::vec3(1.0f, -1.0f, 1.0f));
			}
			//scaling to texture size
			if(i[j]->texture != nullptr) transform = glm::scale(transform, glm::vec3(i[j]->texture->width, i[j]->texture->height, 1.0));

			transform = perspectiveM*viewM*transform;
			glUniformMatrix4fv(glGetUniformLocation(shaderID, "transformation"), 1, GL_FALSE, glm::value_ptr(transform));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
		}
	}

	//buffer swap
	glfwSwapBuffers(Settings::ins().main_window);
	return "";
}

void Renderer::changeTexCoords(Sprite* sprite, Sprite* prev_sprite){
	bool ifBool = false;
	if(prev_sprite == nullptr) ifBool = sprite->sheetFrame != 0 || sprite->sheetColumns != 1 || sprite->sheetRows != 1;
	else ifBool = sprite->sheetFrame != prev_sprite->sheetFrame || sprite->sheetColumns != prev_sprite->sheetColumns || sprite->sheetRows != prev_sprite->sheetRows;

	if(ifBool){
		float posx1 = (1.0f/sprite->sheetColumns)*((sprite->sheetFrame)%(sprite->sheetColumns));
		float posx2 = posx1 + (1.0f/sprite->sheetColumns);
		float posy1 = 1.0f - ((1.0f/sprite->sheetRows)*((sprite->sheetFrame)/(sprite->sheetColumns)));
		float posy2 = posy1 - (1.0f/sprite->sheetRows);
		float texCoords[] = { 
			posx1, posy2,
			posx2, posy2,
			posx1, posy1,
			posx2, posy1, 
		};
		unsigned texVBO = 0;
		glGenBuffers(1, &texVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

template <typename T>
std::vector<std::vector<T>> Renderer::vector2ZIndices(const std::vector<T>& tVector){
	std::vector<std::vector<T>> tVectorsVector;
	for(unsigned z = 0; true; z++){
		std::vector<T> zVector;
		unsigned endLoopCount = 0;
		for(const auto& i: tVector){
			if(i->zIndex == z){
				zVector.push_back(i);
			}
			if(i->zIndex <= z){
				endLoopCount+=1;
			}
		}
		if(zVector.size() > 0) tVectorsVector.push_back(zVector);
		if(endLoopCount == tVector.size()) break;
	}
	return tVectorsVector;
}