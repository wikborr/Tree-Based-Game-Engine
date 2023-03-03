#include "servers/Renderer.h"

#include "servers/ResourceManager.h"
#include "leaves/UILayer.h"
#include "leaves/BGLayer.h"
#include "leaves/leaves2D/Camera2D.h"
#include "leaves/leaves2D/Sprite.h"
#include "leaves/leaves2D/ColorRect.h"

const float TEX_COORD_PIXEL_FIX = 0.5;

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

	//enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//for font
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

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

	//creating texVBO2
	glGenBuffers(1, &this->texVBO2);
	
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
	std::vector<std::pair<std::string,Leaf2D*>> renderables;

	std::vector<Leaf*> leaves = this->leafTree->getAllLeaves();
	for(const auto &i: leaves){
		if(i->propPointers.count("texture") > 0 && i->propPointers.count("centered") > 0) renderables.push_back({"Sprite",static_cast<Leaf2D*>(i)});
		else if(i->propPointers.count("color") > 0 && i->propPointers.count("width") > 0) renderables.push_back({"ColorRect",static_cast<Leaf2D*>(i)});
		else if(i->propPointers.count("tileset") > 0) renderables.push_back({"TileMap",static_cast<Leaf2D*>(i)});
		else if(i->propPointers.count("text") > 0) renderables.push_back({"TextLabel",static_cast<Leaf2D*>(i)});
	}

	//default camera
	float defaultCameraX = 0.0f - static_cast<float>(Settings::ins().game_width)/2;
	float defaultCameraY = 0.0f - static_cast<float>(Settings::ins().game_height)/2;
	glm::mat4 defaultViewM = glm::lookAt(glm::vec3(defaultCameraX, defaultCameraY, 1.0f), glm::vec3(defaultCameraX, defaultCameraY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//view matrix
	this->viewM = defaultViewM;
	
	//perspective matrix
	this->perspectiveM = glm::ortho(0.0f, static_cast<float>(Settings::ins().game_width), 0.0f, static_cast<float>(Settings::ins().game_height), 0.0f, 2.0f);

	//camera system
	Camera2D* curCam = nullptr;
	for(const auto& i: leaves){
		if(i->propPointers.count("current") > 0 && static_cast<Camera2D*>(i)->current){
			curCam = static_cast<Camera2D*>(i);
			break;
		}
	}
	if(curCam != nullptr && curCam->cameraReady){
		glm::vec2 camPos = glm::vec2(curCam->actualPosition.x, curCam->actualPosition.y);
		glm::vec2 screenSize = glm::vec2(curCam->zoom.x*Settings::ins().game_width, curCam->zoom.y*Settings::ins().game_height);
		
		//drag margins
		if(curCam->actualPosition.y + curCam->dragMarginUp*screenSize.y/2 <= curCam->globalPosition.y){
			camPos.y += (curCam->globalPosition.y - (curCam->actualPosition.y + curCam->dragMarginUp*screenSize.y/2));
		}
		else if(curCam->actualPosition.y - curCam->dragMarginDown*screenSize.y/2 >= curCam->globalPosition.y){
			camPos.y += (curCam->globalPosition.y - (curCam->actualPosition.y - curCam->dragMarginUp*screenSize.y/2));
		}
		if(curCam->actualPosition.x + curCam->dragMarginRight*screenSize.x/2 <= curCam->globalPosition.x){
			camPos.x += (curCam->globalPosition.x - (curCam->actualPosition.x + curCam->dragMarginRight*screenSize.x/2));
		}
		else if(curCam->actualPosition.x - curCam->dragMarginLeft*screenSize.x/2 >= curCam->globalPosition.x){
			camPos.x += (curCam->globalPosition.x - (curCam->actualPosition.x - curCam->dragMarginLeft*screenSize.x/2));
		}
		curCam->actualPosition = camPos;

		//smoothing
		if(curCam->smoothingEnabled) camPos = curCam->smoothedPosition;

		//offsets
		camPos.x += curCam->offset.x;
		camPos.y += curCam->offset.y;

		//limits
		if(curCam->limitDown > camPos.y - screenSize.y/2) camPos.y = curCam->limitDown + screenSize.y/2;
		else if(curCam->limitUp < camPos.y + screenSize.y/2) camPos.y = curCam->limitUp - screenSize.y/2;
		if(curCam->limitLeft > camPos.x - screenSize.x/2) camPos.x = curCam->limitLeft + screenSize.x/2;
		else if(curCam->limitRight < camPos.x + screenSize.x/2) camPos.x = curCam->limitRight - screenSize.x/2;

		this->camPos = camPos;
		this->screenSize = screenSize;

		glm::vec2 corCamPos = glm::vec2(camPos.x - screenSize.x/2, camPos.y - screenSize.y/2);
		this->viewM = glm::lookAt(glm::vec3(corCamPos.x, corCamPos.y, 1.f), glm::vec3(corCamPos.x, corCamPos.y, 0.f), glm::vec3(0.f, 1.f, 0.f));
		this->perspectiveM = glm::ortho(0.0f, screenSize.x, 0.0f, screenSize.y, 0.0f, 2.0f);
	}

	//rendering
	//remove invisible (!visible or not on the screen)
	renderables.erase(std::remove_if(renderables.begin(), renderables.end(), [](std::pair<std::string, Leaf2D*> const& pair){
		return !(pair.second->visible); //camera check here
	}), renderables.end());
	//changing the rendering order
	renderables = this->vector2ZIndices(renderables);
	renderables = this->vector2layers(renderables);

	//organizing the vector into a vector of different type vectors
	std::vector<std::pair<std::string, std::vector<Leaf2D*>>> renderablesVectors;
	std::string curLeaf = "";
	std::vector<Leaf2D*> curVector;
	if(renderables.size()>0){
		curLeaf = renderables[0].first;
		curVector.push_back(renderables[0].second);
		for(int i=1; i<renderables.size(); i++){
			if(renderables[i].first != curLeaf){
				renderablesVectors.push_back(std::pair<std::string, std::vector<Leaf2D*>>(curLeaf, curVector));
				curLeaf = renderables[i].first;
				curVector.clear();
			}
			curVector.push_back(renderables[i].second);
		}
		renderablesVectors.push_back(std::pair<std::string, std::vector<Leaf2D*>>(curLeaf, curVector));
	}

	//rendering
	for(const auto& i: renderablesVectors){
		if(i.first == "Sprite"){
			this->renderSprites(utils::vector2vector<Sprite*, Leaf2D*>(i.second));
		}
		else if(i.first == "ColorRect"){
			this->renderColorRects(utils::vector2vector<ColorRect*, Leaf2D*>(i.second));
		}
		else if(i.first == "TileMap"){
			this->renderTileMaps(utils::vector2vector<TileMap*, Leaf2D*>(i.second));
		}
		else if(i.first == "TextLabel"){
			this->renderTextLabels(utils::vector2vector<TextLabel*, Leaf2D*>(i.second));
		}
	}

	//for debugging collision
	if(Settings::ins().debugCollision){
		std::vector<CollisionShape2D*> collisionShapes;
		for(const auto &i: leaves){
			if(i->propPointers.count("oneWayCollision") > 0) collisionShapes.push_back(static_cast<CollisionShape2D*>(i));
		}
		this->renderCollisionShapes(collisionShapes);
	}

	//buffer swap
	glBindVertexArray(0);
	glfwSwapBuffers(Settings::ins().main_window);
	return "";
}

void Renderer::changeTexCoords(SpriteSheet sprite, SpriteSheet prev_sprite){
	bool ifBool = true;
	if(prev_sprite.sheetColumns != 0) ifBool = sprite.sheetFrame != prev_sprite.sheetFrame || sprite.sheetColumns != prev_sprite.sheetColumns || sprite.sheetRows != prev_sprite.sheetRows;

	if(ifBool){
		double stepX = 1.0/sprite.sheetColumns;
		double stepY = 1.0/sprite.sheetRows;
		double posx1 = stepX*((sprite.sheetFrame)%(sprite.sheetColumns));
		double posx2 = posx1 + stepX;
		double posy1 = 1.0 - (stepY*((sprite.sheetFrame)/(sprite.sheetColumns)));
		double posy2 = posy1 - stepY;
		posx1+=(1.0/sprite.sheetSize.x)*TEX_COORD_PIXEL_FIX;
		posx2-=(1.0/sprite.sheetSize.x)*TEX_COORD_PIXEL_FIX;
		posy1-=(1.0/sprite.sheetSize.y)*TEX_COORD_PIXEL_FIX;
		posy2+=(1.0/sprite.sheetSize.y)*TEX_COORD_PIXEL_FIX;
		float texCoords[] = { 
			static_cast<float>(posx1), static_cast<float>(posy2),
			static_cast<float>(posx2), static_cast<float>(posy2),
			static_cast<float>(posx1), static_cast<float>(posy1),
			static_cast<float>(posx2), static_cast<float>(posy1), 
		};
		glBindBuffer(GL_ARRAY_BUFFER, this->texVBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Renderer::resetTexCoords(){
	float texCoords[] = { 
        0.0f, 0.0f,
        1.0f, 0.0f,
		0.0f, 1.0f,
        1.0f, 1.0f, 
    };
	glBindBuffer(GL_ARRAY_BUFFER, this->texVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

std::vector<std::pair<std::string, Leaf2D*>> Renderer::vector2ZIndices(const std::vector<std::pair<std::string, Leaf2D*>>& tVector){
	std::vector<std::vector<std::pair<std::string, Leaf2D*>>> tVectorsVector;
	for(unsigned z = 0; true; z++){
		std::vector<std::pair<std::string, Leaf2D*>> zVector;
		unsigned endLoopCount = 0;
		for(const auto& i: tVector){
			if(i.second->zIndex == z){
				zVector.push_back(i);
			}
			if(i.second->zIndex <= z){
				endLoopCount+=1;
			}
		}
		if(zVector.size() > 0) tVectorsVector.push_back(zVector);
		if(endLoopCount == tVector.size()) break;
	}
	std::vector<std::pair<std::string, Leaf2D*>> finalVector;
	for(const auto& i: tVectorsVector){
		finalVector.insert(finalVector.end(), i.begin(), i.end());
	}
	return finalVector;
}

std::vector<std::pair<std::string, Leaf2D*>> Renderer::vector2layers(const std::vector<std::pair<std::string, Leaf2D*>>& tVector){
	std::vector<std::vector<std::pair<std::string, Leaf2D*>>> tVectorsVectorBG;
	std::vector<std::vector<std::pair<std::string, Leaf2D*>>> tVectorsVectorFG;
	tVectorsVectorFG.push_back(std::vector<std::pair<std::string, Leaf2D*>>());
	std::vector<std::pair<std::pair<std::string, Leaf2D*>, int>> tVectorReal;
	for(const auto &i: tVector){
		Leaf* parentLayer = i.second->parent;
		while(parentLayer != nullptr && parentLayer->propPointers.count("layer") == 0) parentLayer = parentLayer->parent;
		if(parentLayer != nullptr && static_cast<UILayer*>(parentLayer)->layer != 0){
			if(static_cast<UILayer*>(parentLayer)->visible){
				tVectorReal.push_back({i, static_cast<UILayer*>(parentLayer)->layer});
			}
		}
		else{
			tVectorsVectorFG.at(0).push_back(i);
		}
	}
	for(int layer = 1; true; layer++){
		std::vector<std::pair<std::string, Leaf2D*>> layerVector;
		unsigned endLoopCount = 0;
		for(const auto& i: tVectorReal){
			if(i.second == layer){
				layerVector.push_back(i.first);
			}
			if(i.second <= layer){
				endLoopCount+=1;
			}
		}
		if(layerVector.size() > 0) tVectorsVectorFG.push_back(layerVector);
		if(endLoopCount == tVectorReal.size()) break;
	}
	for(int layer = -1; true; layer--){
		std::vector<std::pair<std::string, Leaf2D*>> layerVector;
		unsigned endLoopCount = 0;
		for(const auto& i: tVectorReal){
			if(i.second == layer){
				layerVector.push_back(i.first);
			}
			if(i.second >= layer){
				endLoopCount+=1;
			}
		}
		if(layerVector.size() > 0) tVectorsVectorBG.push_back(layerVector);
		if(endLoopCount == tVectorReal.size()) break;
	}
	std::vector<std::pair<std::string, Leaf2D*>> finalVector;
	for(int i = tVectorsVectorBG.size()-1; i>=0; i--){
		finalVector.insert(finalVector.end(), tVectorsVectorBG.at(i).begin(), tVectorsVectorBG.at(i).end());
	}
	for(const auto& i: tVectorsVectorFG){
		finalVector.insert(finalVector.end(), i.begin(), i.end());
	}
	return finalVector;
}

void Renderer::renderLeaf2dPart1(Leaf2D* renderable, Leaf2D* prevRenderable, glm::vec4 additionalColor){
	//calculating Leaf2D properties part 1
	glm::vec4 leafColor = renderable->modulate * renderable->selfModulate * additionalColor;
	this->curTransform = renderable->globalTransform;
	this->curShaderID = this->defaultShader->ID;
	if(renderable->leafShader != nullptr) this->curShaderID = renderable->leafShader->ID;

	//checking if OpenGL context needs to be changed
	if(prevRenderable == nullptr){
		if(this->curShaderID != this->defaultShader->ID) glUseProgram(this->curShaderID);
		glUniform4f(glGetUniformLocation(this->curShaderID, "color"), leafColor.r, leafColor.g, leafColor.b, leafColor.a);
	}
	else{
		if(prevRenderable->leafShader != nullptr){
			if(this->curShaderID != prevRenderable->leafShader->ID) glUseProgram(this->curShaderID);
		}
		else if(this->curShaderID != this->defaultShader->ID) glUseProgram(this->curShaderID);

		if(leafColor != prevRenderable->modulate * prevRenderable->selfModulate) glUniform4f(glGetUniformLocation(this->curShaderID, "color"), leafColor.r, leafColor.g, leafColor.b, leafColor.a);
	}
}

void Renderer::renderLeaf2dPart2(Leaf2D* renderable, bool multiple){
	glm::mat4 finalViewM = this->viewM;
	glm::mat4 finalPerspectiveM = this->perspectiveM;
	glm::vec2 mirrorActiveX = glm::vec2(false);
	glm::vec2 mirrorActiveY = glm::vec2(false);
	glm::vec2 mirrorPos = glm::vec2(0);
	glm::vec2 mirrorDelta = glm::vec2(0.f);

	//checking for parent layers
	Leaf* parentLayer = renderable->parent;
	while(parentLayer != nullptr && parentLayer->propPointers.count("layer") == 0) parentLayer = parentLayer->parent;
	if(parentLayer != nullptr){
		float cameraX = 0.0f - static_cast<float>(Settings::ins().defaultGameWidth)/2;
		float cameraY = 0.0f - static_cast<float>(Settings::ins().defaultGameHeight)/2;
		if(parentLayer->propPointers.count("scrollScale") > 0){
			BGLayer* bgLayer = static_cast<BGLayer*>(parentLayer);
			
			//scaling
			glm::vec2 camPos = this->camPos;
			camPos -= bgLayer->startCameraPos;
			camPos *= bgLayer->scrollScale;
			camPos += bgLayer->startCameraPos;
			cameraX = camPos.x - this->screenSize.x/2;
			cameraY = camPos.y - this->screenSize.y/2;

			//mirroring
			if(bgLayer->mirrorOffset.x != 0.f){
				mirrorActiveX.x = true;
				int mirrorModX = static_cast<int>(cameraX+bgLayer->startCameraPos.x) % static_cast<int>(bgLayer->mirrorOffset.x);
				int mirrorDivX = static_cast<int>(cameraX+bgLayer->startCameraPos.x) / static_cast<int>(bgLayer->mirrorOffset.x);
				if(bgLayer->startCameraPos.x < 0 && mirrorModX>=0){
					mirrorDivX+=1;
				}
				else if(bgLayer->startCameraPos.x >= 0 && mirrorModX < 0){
					mirrorDivX-=1;
				}
				if (mirrorModX<0) mirrorModX += bgLayer->mirrorOffset.x;
				mirrorPos.x = mirrorDivX;
				mirrorDelta.x = mirrorModX;
				if(mirrorModX + this->screenSize.x > bgLayer->mirrorOffset.x) mirrorActiveX.y = true;
			}
			if(bgLayer->mirrorOffset.y != 0.f){
				mirrorActiveY.x = true;
				int mirrorModY = static_cast<int>(cameraY+bgLayer->startCameraPos.y) % static_cast<int>(bgLayer->mirrorOffset.y);
				int mirrorDivY = static_cast<int>(cameraY+bgLayer->startCameraPos.y) / static_cast<int>(bgLayer->mirrorOffset.y);
				if(bgLayer->startCameraPos.y < 0 && mirrorModY>=0){
					mirrorDivY+=1;
				}
				else if(bgLayer->startCameraPos.y >= 0 && mirrorModY < 0){
					mirrorDivY-=1;
				}
				if (mirrorModY<0) mirrorModY += bgLayer->mirrorOffset.y;
				mirrorPos.y = mirrorDivY;
				mirrorDelta.y = mirrorModY;
				if(mirrorModY + this->screenSize.y > bgLayer->mirrorOffset.y) mirrorActiveY.y = true;
			}
		}
		else{
			UILayer* uiLayer = static_cast<UILayer*>(parentLayer);
			finalPerspectiveM = glm::ortho(0.0f, static_cast<float>(Settings::ins().defaultGameWidth), 0.0f, static_cast<float>(Settings::ins().defaultGameHeight), 0.0f, 2.0f);
		}
		finalViewM = glm::lookAt(glm::vec3(cameraX, cameraY, 1.0f), glm::vec3(cameraX, cameraY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	//mirroring setup
	if(mirrorActiveX.x){
		this->curTransform = glm::translate(this->curTransform, {mirrorPos.x, 0.f, 0.f});
	}
	if(mirrorActiveY.x){
		this->curTransform = glm::translate(this->curTransform, {0.f, mirrorPos.y, 0.f});
	}
	glm::mat4 mirrorTransform = this->curTransform;

	//drawing
	if(!multiple) this->finalRender(finalPerspectiveM, finalViewM);
	else this->finalRender(finalPerspectiveM, finalViewM, renderable);

	//mirror drawing
	if(mirrorActiveX.y){
		this->curTransform = glm::translate(mirrorTransform, {1.f, 0.f, 0.f});
		if(!multiple) this->finalRender(finalPerspectiveM, finalViewM);
		else this->finalRender(finalPerspectiveM, finalViewM, renderable);
	}
	if(mirrorActiveY.y){
		this->curTransform = glm::translate(mirrorTransform, {0.f, 1.f, 0.f});
		if(!multiple) this->finalRender(finalPerspectiveM, finalViewM);
		else this->finalRender(finalPerspectiveM, finalViewM, renderable);
	}
	if(mirrorActiveX.y && mirrorActiveY.y){
		this->curTransform = glm::translate(mirrorTransform, {1.f, 1.f, 0.f});
		if(!multiple) this->finalRender(finalPerspectiveM, finalViewM);
		else this->finalRender(finalPerspectiveM, finalViewM, renderable);
	}
}

void Renderer::finalRender(glm::mat4 finalPerspectiveM, glm::mat4 finalViewM, Leaf2D* multipleLeaf){
	if(multipleLeaf == nullptr){
		this->curTransform = finalPerspectiveM*finalViewM*this->curTransform;
		glUniformMatrix4fv(glGetUniformLocation(this->curShaderID, "transformation"), 1, GL_FALSE, glm::value_ptr(this->curTransform));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	else if(multipleLeaf->propPointers.count("tileset") > 0){
		TileMap* tilemap = static_cast<TileMap*>(multipleLeaf);
		if(tilemap->texture == nullptr) return;
		SpriteSheet sheet;
		sheet.sheetRows = tilemap->tileset->sheetRows;
		sheet.sheetColumns = tilemap->tileset->sheetColumns;
		sheet.sheetSize = glm::ivec2(tilemap->texture->width, tilemap->texture->height);
		glm::vec2 offset = glm::vec2(tilemap->texture->width / tilemap->tileset->sheetColumns, tilemap->texture->height / tilemap->tileset->sheetRows);
		for(int i = 0; i < tilemap->tilePositions.size(); i++){
			for(int j = 0; j < tilemap->tilePositions.at(i).size(); j++){
				if(tilemap->tilePositions.at(i).at(j) < 0) continue;
				if(i == 0 && j == 0){
					sheet.sheetFrame = tilemap->tilePositions.at(i).at(j);
					this->changeTexCoords(sheet);
				}
				else{
					SpriteSheet sheet2;
					sheet2.sheetRows = sheet.sheetRows;
					sheet2.sheetColumns = sheet.sheetColumns;
					sheet2.sheetFrame = tilemap->tilePositions.at(i).at(j);
					sheet2.sheetSize = glm::ivec2(tilemap->texture->width, tilemap->texture->height);
					this->changeTexCoords(sheet2, sheet);
					sheet.sheetFrame = sheet2.sheetFrame;
				}
				glm::mat4 tileTransform = glm::translate(this->curTransform, glm::vec3(j,i,0.f));
				tileTransform = finalPerspectiveM*finalViewM*tileTransform;
				glUniformMatrix4fv(glGetUniformLocation(this->curShaderID, "transformation"), 1, GL_FALSE, glm::value_ptr(tileTransform));
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}
		}
		this->resetTexCoords();
	}
	else if(multipleLeaf->propPointers.count("text") > 0){
		TextLabel* textLabel = static_cast<TextLabel*>(multipleLeaf);
		if(textLabel->stringTextures.size() > 0){
			std::vector<std::vector<Character*>> finalChars;
			size_t charsVisible = glm::min(static_cast<size_t>(textLabel->stringTextures.size()*textLabel->percentVisible), textLabel->stringTextures.size());
			std::vector<Character*> finalCharsPart;
			unsigned advanceY = 0u;
			for(int i = 0; i < charsVisible; i++){
				if(textLabel->stringTextures.at(i)->texture != nullptr){
					finalCharsPart.push_back(textLabel->stringTextures.at(i));
				}
				else if(textLabel->stringTextures.at(i)->advance > 0u){
					if(advanceY == 0) advanceY = textLabel->textSize + textLabel->textSize/textLabel->stringTextures.at(i)->advance;
					finalChars.push_back(finalCharsPart);
					finalCharsPart.clear();
				}
			}
			if(finalCharsPart.size() > 0){
				finalChars.push_back(finalCharsPart);
			}

			for(int i = 0; i < finalChars.size(); i++){
				unsigned advanceX = 0u;
				for(int j = 0; j < finalChars.at(i).size(); j++){
					glBindTexture(GL_TEXTURE_2D, finalChars.at(i).at(j)->texture->ID);
					glm::mat4 charTransform = this->curTransform;
					charTransform = glm::scale(charTransform, glm::vec3(finalChars.at(i).at(j)->size, 0.f));
					charTransform = glm::translate(charTransform, glm::vec3(
						finalChars.at(i).at(j)->bearing.x + advanceX,
						finalChars.at(i).at(j)->bearing.y + advanceY*i, 
						0.f
					));
					advanceX += (finalChars.at(i).at(j)->advance) >> 6;
					charTransform = finalPerspectiveM*finalViewM*charTransform;
					glUniformMatrix4fv(glGetUniformLocation(this->curShaderID, "transformation"), 1, GL_FALSE, glm::value_ptr(charTransform));
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				}
			}
		}
	}
}

void Renderer::renderSprites(const std::vector<Sprite*>& sprites){
	for(int j = 0; j<sprites.size(); j++){
		//checking if OpenGL context needs to be changed
		if(j==0){
			this->renderLeaf2dPart1(static_cast<Leaf2D*>(sprites[j]));

			if(sprites[j]->texture != nullptr) glBindTexture(GL_TEXTURE_2D, sprites[j]->texture->ID);
			else glBindTexture(GL_TEXTURE_2D, 0);

			if(sprites[j]->texture != nullptr){
				SpriteSheet sheet;
				sheet.sheetFrame = sprites[j]->sheetFrame;
				sheet.sheetRows = sprites[j]->sheetRows;
				sheet.sheetColumns = sprites[j]->sheetColumns;
				sheet.sheetSize = glm::ivec2(sprites[j]->texture->width, sprites[j]->texture->height);
				this->changeTexCoords(sheet);
			}
		}
		else{
			this->renderLeaf2dPart1(static_cast<Leaf2D*>(sprites[j]), static_cast<Leaf2D*>(sprites[j-1]));

			if(sprites[j]->texture != sprites[j-1]->texture){
				if(sprites[j]->texture != nullptr) glBindTexture(GL_TEXTURE_2D, sprites[j]->texture->ID);
				else glBindTexture(GL_TEXTURE_2D, 0);
			}
			if(sprites[j]->texture != nullptr && sprites[j-1]->texture != nullptr){
				SpriteSheet sheet1;
				sheet1.sheetFrame = sprites[j]->sheetFrame;
				sheet1.sheetRows = sprites[j]->sheetRows;
				sheet1.sheetColumns = sprites[j]->sheetColumns;
				sheet1.sheetSize = glm::ivec2(sprites[j]->texture->width, sprites[j]->texture->height);
				SpriteSheet sheet2;
				sheet2.sheetFrame = sprites[j-1]->sheetFrame;
				sheet2.sheetRows = sprites[j-1]->sheetRows;
				sheet2.sheetColumns = sprites[j-1]->sheetColumns;
				sheet2.sheetSize = glm::ivec2(sprites[j-1]->texture->width, sprites[j-1]->texture->height);
				this->changeTexCoords(sheet1, sheet2);
			}
		}
		
		//calculating Sprite properties
		if(!sprites[j]->centered && sprites[j]->texture != nullptr){
			this->curTransform = glm::translate(this->curTransform, glm::vec3(static_cast<float>(sprites[j]->texture->width/2), static_cast<float>(sprites[j]->texture->height/2), 0.0f));
		}
		if(sprites[j]->flipH){
			this->curTransform = glm::scale(this->curTransform, glm::vec3(-1.0f, 1.0f, 1.0f));
		}
		if(sprites[j]->flipV){
			this->curTransform = glm::scale(this->curTransform, glm::vec3(1.0f, -1.0f, 1.0f));
		}
		//scaling to texture size
		if(sprites[j]->texture != nullptr) this->curTransform = glm::scale(this->curTransform, glm::vec3(sprites[j]->texture->width/sprites[j]->sheetColumns, sprites[j]->texture->height/sprites[j]->sheetRows, 1.0));

		//calculating Leaf2D properties part 2 and drawing
		this->renderLeaf2dPart2(static_cast<Leaf2D*>(sprites[j]));
	}
}

void Renderer::renderColorRects(const std::vector<ColorRect*>& colorRects){
	glUniform1i(glGetUniformLocation(this->curShaderID, "colorOnly"), true);
	for(int j = 0; j<colorRects.size(); j++){
		if(j==0){
			this->renderLeaf2dPart1(static_cast<Leaf2D*>(colorRects[j]), nullptr, glm::vec4(colorRects[j]->color, 1.f));
		}
		else{
			this->renderLeaf2dPart1(static_cast<Leaf2D*>(colorRects[j]), static_cast<Leaf2D*>(colorRects[j-1]), glm::vec4(colorRects[j]->color, 1.f));
		}

		this->curTransform = glm::scale(this->curTransform, glm::vec3(colorRects[j]->width, colorRects[j]->height, 1.0));

		this->renderLeaf2dPart2(static_cast<Leaf2D*>(colorRects[j]));
	}
	glUniform1i(glGetUniformLocation(this->curShaderID, "colorOnly"), false);
}

void Renderer::renderTileMaps(const std::vector<TileMap*>& tilemaps){
	for(int j = 0; j<tilemaps.size(); j++){
		if(!tilemaps[j]->tilePositions.empty() && tilemaps[j]->tileset != nullptr){
			if(j==0){
				this->renderLeaf2dPart1(static_cast<Leaf2D*>(tilemaps[j]));

				if(tilemaps[j]->texture != nullptr) glBindTexture(GL_TEXTURE_2D, tilemaps[j]->texture->ID);
				else glBindTexture(GL_TEXTURE_2D, 0);
			}
			else{
				this->renderLeaf2dPart1(static_cast<Leaf2D*>(tilemaps[j]), static_cast<Leaf2D*>(tilemaps[j-1]));

				if(tilemaps[j]->texture != tilemaps[j-1]->texture){
					if(tilemaps[j]->texture != nullptr) glBindTexture(GL_TEXTURE_2D, tilemaps[j]->texture->ID);
					else glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
			
			//scaling to texture size
			if(tilemaps[j]->texture != nullptr && tilemaps[j]->tileset != nullptr) this->curTransform = glm::scale(this->curTransform, glm::vec3(tilemaps[j]->texture->width/tilemaps[j]->tileset->sheetColumns, tilemaps[j]->texture->height/tilemaps[j]->tileset->sheetRows, 1.0));

			this->renderLeaf2dPart2(static_cast<Leaf2D*>(tilemaps[j]), true);
		}
	}
}

void Renderer::renderTextLabels(const std::vector<TextLabel*>& textLabels){
	glUniform1i(glGetUniformLocation(this->curShaderID, "isText"), true);
	for(int j = 0; j<textLabels.size(); j++){
		if(textLabels[j]->text != "" && textLabels[j]->font != nullptr){
			if(j==0){
				this->renderLeaf2dPart1(static_cast<Leaf2D*>(textLabels[j]), nullptr, glm::vec4(static_cast<TextLabel*>(textLabels[j])->color, 1.f));
			}
			else{
				this->renderLeaf2dPart1(static_cast<Leaf2D*>(textLabels[j]), static_cast<Leaf2D*>(textLabels[j-1]), glm::vec4(static_cast<TextLabel*>(textLabels[j])->color, 1.f));
			}
			glUniform1ui(glGetUniformLocation(this->curShaderID, "textOutline"), static_cast<TextLabel*>(textLabels[j])->outline);
			glUniform3f(glGetUniformLocation(
				this->curShaderID, "textOutlineColor"), 
				static_cast<TextLabel*>(textLabels[j])->outLineColor.r,
				static_cast<TextLabel*>(textLabels[j])->outLineColor.g,
				static_cast<TextLabel*>(textLabels[j])->outLineColor.b
			);
			this->renderLeaf2dPart2(static_cast<Leaf2D*>(textLabels[j]), true);
		}
	}
	glUniform1i(glGetUniformLocation(this->curShaderID, "isText"), false);
}

void Renderer::renderCollisionShapes(const std::vector<CollisionShape2D*>& collisionShapes){
	glUniform1i(glGetUniformLocation(this->curShaderID, "colorOnly"), true);
	for(int j = 0; j<collisionShapes.size(); j++){
		if(collisionShapes[j]->shape == nullptr || collisionShapes[j]->disabled) continue;

		if(j==0){
			this->renderLeaf2dPart1(static_cast<Leaf2D*>(collisionShapes[j]), nullptr, glm::vec4(0.1f, 0.6f, 1.f, 0.5f));
		}
		else{
			this->renderLeaf2dPart1(static_cast<Leaf2D*>(collisionShapes[j]), static_cast<Leaf2D*>(collisionShapes[j-1]), glm::vec4(0.1f, 0.6f, 1.f, 0.5f));
		}

		glm::vec2 shapeSize;
		std::vector<glm::vec2> shapePoints = collisionShapes[j]->shape->toPoints(collisionShapes[j]->globalTransform);
		if(shapePoints.size() != 4) continue;
		shapeSize = collisionShapes[j]->shape->baseSize();

		this->curTransform = glm::scale(this->curTransform, glm::vec3(shapeSize.x, shapeSize.y, 1.0));

		this->renderLeaf2dPart2(static_cast<Leaf2D*>(collisionShapes[j]));
	}
	glUniform1f(glGetUniformLocation(this->curShaderID, "circleRadius"), 0.f);
	glUniform1i(glGetUniformLocation(this->curShaderID, "colorOnly"), false);
}