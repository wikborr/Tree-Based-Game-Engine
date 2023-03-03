#include "servers/FileReader.h"

FileReader& FileReader::ins(){
	static FileReader instance;
	return instance;
}

std::string FileReader::loadAsString(std::string path){
	std::ifstream file(path);
	if(!file){
		return "";
	}
	std::stringstream buff;
	buff << file.rdbuf();
	file.close();
	return buff.str();
}

bool FileReader::checkIfFileExists(const std::string& path){
	std::ifstream file(path);
    return file.good();
}