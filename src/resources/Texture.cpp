#include "resources/Texture.h"

Texture::Texture(){
	glGenTextures(1, &this->ID);
}

void Texture::setTexture(unsigned char* data, unsigned filter, unsigned format, unsigned format2){
	glBindTexture(GL_TEXTURE_2D, this->ID);
	
	//texture properties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	//set the texture in the OpenGL context
	glTexImage2D(GL_TEXTURE_2D, 0, format2, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}