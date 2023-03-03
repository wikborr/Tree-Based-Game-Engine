#include "leaves/leaves2D/TextLabel.h"
#include "servers/ResourceManager.h"

TextLabel::TextLabel(Leaf* parent){
	this->name = "TextLabel";
	std::map<std::string, std::pair<std::string, void*>> props = {
		{"text", {"string", &(this->text)}},
		{"textSize", {"unsigned", &(this->textSize)}},
		{"percentVisible", {"float", &(this->percentVisible)}},
		{"color", {"vec3", &(this->color)}},
		{"outline", {"unsigned", &(this->outline)}},
		{"outLineColor", {"vec3", &(this->outLineColor)}},
		{"font", {"font", &(this->font)}},
	};
	this->propPointers.insert(props.begin(), props.end());
}

TextLabel::~TextLabel(){
	ResourceManager::ins().unloadFont(this->font);
}

std::string TextLabel::leafOperation(){
	Leaf2D::leafOperation();
	if(this->font != nullptr){
		if(this->prevText != this->text){
			this->updateText();
			this->prevText = this->text;
		}
		if(this->prevTextSize != this->textSize){
			this->updateText();
			this->prevTextSize = this->textSize;
		}
	}
	return "";
}

void TextLabel::updateText(){
	this->font->setFontSize(this->textSize);
	this->stringTextures = this->font->getString(this->text);
	this->totalTextureSize = glm::ivec2(0, this->textSize);
	for(const auto& i: this->stringTextures){
		if(i->texture != nullptr){
			this->totalTextureSize.x += i->advance;
		}
		else if(i->advance > 0u){
			this->totalTextureSize.y += this->textSize + this->textSize/i->advance;
		}
	}
}