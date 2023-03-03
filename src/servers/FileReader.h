#pragma once

#include "Utility.h"

//server singleton for file reading
class FileReader{
public:
	//singleton method
	static FileReader& ins();

	std::string loadAsString(std::string path);
	bool checkIfFileExists(const std::string& path);
};