#pragma once

#include "Settings.h"

class Shader{
public:
	int references = 0;
	std::string vertexPath = "";
	std::string fragmentPath = "";
	std::string geometryPath = "";

	unsigned ID = 0;

	~Shader();
	void compileShaders(std::string vertexString, std::string fragmentString, std::string geometryString = "");
};