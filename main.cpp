#include <GLFW/glfw3.h>

#include "Seed.h"

int main(){
	//initialize the engine and choose the project to use
	Seed engine;
	std::string error_message = engine.plant("test");

	//logging for extremely bad errors
	if(error_message!=""){
		std::cerr<<error_message<<std::endl;
		return -1;
	}
	return 0;
}