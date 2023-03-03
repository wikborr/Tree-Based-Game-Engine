#pragma once

#include "Settings.h"

class Texture{
public:
	int references = 0;
	std::string path = "";

	unsigned ID = 0;
	int width = 0;
	int height = 0;

	Texture();
	void setTexture(unsigned char* data, unsigned filter = GL_LINEAR, unsigned format = GL_RGBA, unsigned format2 = GL_RGBA);
};