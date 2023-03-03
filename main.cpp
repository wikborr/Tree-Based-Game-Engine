#include "Seed.h"

int main(){
	//initialize the engine and choose the project to use
	Seed engine;
	std::string error_message = engine.plant("project/");

	//logging for critical engine errors
	if(error_message!=""){
		std::cerr<<"\033[31m"+error_message+"\033[0m"<<std::endl;
		return -1;
	}
	return 0;
}