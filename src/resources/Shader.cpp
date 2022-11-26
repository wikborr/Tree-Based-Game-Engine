#include "resources/Shader.h"

Shader::~Shader(){
	if(this->ID != 0){
		glDeleteProgram(this->ID);
	}
}

void Shader::compileShaders(std::string vertexString, std::string fragmentString, std::string geometryString){
	const char* vString = vertexString.c_str();
	const char* fString = fragmentString.c_str();
	const char* gString = geometryString.c_str();

	unsigned vShader = 0, fShader = 0, gShader = 0;
	int success;
	char infoLog[1024];

	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vString, nullptr);
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vShader, 1024, NULL, infoLog);
		std::cerr<<"ERROR: vertex shader: compilation error"<<std::endl<<infoLog<<std::endl;
	}

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fString, nullptr);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fShader, 1024, NULL, infoLog);
		std::cerr<<"ERROR: fragment shader: compilation error"<<std::endl<<infoLog<<std::endl;
	}

	if(geometryString != ""){
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &gString, nullptr);
		glCompileShader(gShader);
		glGetShaderiv(gShader, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(gShader, 1024, NULL, infoLog);
			std::cerr<<"ERROR: geometry shader: compilation error"<<std::endl<<infoLog<<std::endl;
		}
	}

	if(this->ID != 0){
		glDeleteProgram(this->ID);
	}
	this->ID = glCreateProgram();
	glAttachShader(this->ID, vShader);
	glAttachShader(this->ID, fShader);
	if(geometryString != ""){
		glAttachShader(this->ID, gShader);
	}
	glLinkProgram(this->ID);
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(this->ID, 1024, NULL, infoLog);
		std::cerr<<"ERROR: shader program: linking error"<<std::endl<<infoLog<<std::endl;
	}
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	if(geometryString != ""){
		glDeleteShader(gShader);
	}
}