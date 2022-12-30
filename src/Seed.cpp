#include <Seed.h>

#include "Settings.h"
#include "LeafTree.h"
#include "servers/ResourceManager.h"
#include "servers/Renderer.h"
#include "servers/Input.h"

//#include <chrono>
//#include <thread>
//const int MIN_TIME_LEFT = 500000;

std::string Seed::plant(std::string project_dir){
	std::string error_message = "";

	//GLFW initialization (window library)
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	//initializing Settings singleton from project settings
	error_message = Settings::ins().generateSettings(project_dir);
	if(error_message != ""){
		return error_message;
	}

	//main rendering window initialization
	if(Settings::ins().createMainWindow()){
		glfwTerminate();
		return "GLFW: window creation failed";
	}
	//GLAD initialization (OpenGL loader)
	if(Settings::ins().initGLAD()){
		glfwTerminate();
		return "GLAD: initialization failed";
	}
	//Leaf Tree initialization
	LeafTree leafTree;
	
	//Resource Initialization
	ResourceManager::ins();

	//Renderer initialization
	error_message = Renderer::ins().init(&leafTree);
	if(error_message != ""){
		return error_message;
	}

	//Input manager initialization
	Input::ins();

	//creating the leaf tree from the .grass file
	leafTree.growGrass(Settings::ins().main_grass);

	Settings::ins().initialized = true;
	std::cout<<"Starting the main loop..."<<std::endl<<"--------------------------"<<std::endl;
	//main loop
	double lastTime = 0.0;
	while(!glfwWindowShouldClose(Settings::ins().main_window)){
		//delta
		double time = glfwGetTime();
        double deltaTime = time - lastTime;
        lastTime = time;

		//input
		if(Input::ins().checkIfInputEvent()){
			leafTree.doInputEvents();
		}

		//logic updates
		error_message = leafTree.doLeafOperations();
		if(error_message != ""){
			return error_message;
		}
		leafTree.doLeafUpdates(deltaTime);

		//rendering
		error_message = Renderer::ins().render();
		if(error_message != ""){
			return error_message;
		}

		//manage "just pressed" and "just released" input
		Input::ins().removeJustKeys();
		
		//check for GLFW events
		glfwPollEvents();

		//fps limit
		if(Settings::ins().limitFPS > 0){
			int timeLeft = static_cast<int>((lastTime + (1.0/(Settings::ins().limitFPS+1)) - glfwGetTime())*1000000);
			//if(timeLeft >= MIN_TIME_LEFT) std::this_thread::sleep_for(std::chrono::microseconds(timeLeft/2));
			while (glfwGetTime() < lastTime + 1.0/(Settings::ins().limitFPS+1)) {}
		}
	}

	std::cout<<"--------------------------"<<std::endl;
	//deallocation
	leafTree.~LeafTree();
	std::cout<<"Leaf deallocation complete"<<std::endl;
	ResourceManager::ins().~ResourceManager();
	std::cout<<"Resource deallocation complete"<<std::endl;
	Renderer::ins().~Renderer();
	std::cout<<"OpenGL deallocation complete"<<std::endl;
	glfwTerminate();
	std::cout<<"GLFW deallocation complete"<<std::endl;
	return "";
}
