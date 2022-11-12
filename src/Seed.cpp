#include <Seed.h>

#include "Settings.h"

std::string Seed::plant(std::string project_name){
	//initializing Settings singleton from project settings
	

	//GLFW initialization (window library)
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	//main rendering window initialization
	if(Settings::ins().createMainWindow()){
		return "GLFW: window creation failed";
	}
	//GLAD initialization (OpenGL loader)
	if(Settings::ins().initGLAD()){
		return "GLAD: initialization failed";
	}
	//main loop
	while(!glfwWindowShouldClose(Settings::ins().main_window)){
		//(should be done in rendering)
		glClearColor(Settings::ins().bg_color.r, Settings::ins().bg_color.g, Settings::ins().bg_color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(Settings::ins().main_window);
		
		//check for GLFW events
		glfwPollEvents();
	}

	glfwTerminate();
	return "";
}
